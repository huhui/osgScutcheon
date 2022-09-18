#include "graphicsview.h"

GraphicsView* GraphicsView::m_pInstance = NULL;

GraphicsView::GraphicsView(const QString& strFile, QWidget *parent)
{
	//...
	//...

    m_pWindowManager = nullptr;

	init();
	//...
	//...
	//加载.earth配置文件
    setEarthFile(strFile);
}

GraphicsView::~GraphicsView()
{
    release();
}

GraphicsView *GraphicsView::instance()
{
    if ( NULL == m_pInstance )
    {
        m_pInstance = new GraphicsView(QStringLiteral("../publish/data/gisdata/earthfile/earth_map/planets/earth_map.earth"));
    }

    return m_pInstance;
}

void GraphicsView::destroy()
{
    if ( NULL != m_pInstance )
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}


void GraphicsView::init()
{
	//初始化m_pViewer
	
    //...
	//...
	
	//初始化m_pRoot
    m_pRoot = new osg::Group;
    m_pRoot->setName("root");


    //GL_DEPTH_TEST开启深度检测，也就是说，后绘制的物体覆盖先绘制的物体
    //m_pRoot->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    //m_pRoot->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);

}


void GraphicsView::initSky()
{
    //一般方法初始化天空
    m_pSkyNode = osgEarth::Util::SkyNode::create(m_pMapNode);
    m_pSkyNode->setName("SkyNode");
    // 设置时间;默认当前时间
    osgEarth::DateTime dateTime(2018, 4, 26, 2);//dateTime(2018, 3, 8, 5)
    qDebug() << dateTime.year() << ":" << dateTime.month() << ":" << dateTime.day() << ":" << dateTime.hours();
    osgEarth::Util::Ephemeris* ephemeris = new osgEarth::Util::Ephemeris;
    m_pSkyNode->setEphemeris(ephemeris);
    m_pSkyNode->setAtmosphereVisible(true);
    m_pSkyNode->setStarsVisible(true);
    m_pSkyNode->setMoonVisible(true);
    m_pSkyNode->setSunVisible(true);
    m_pSkyNode->setDateTime(dateTime);
    m_pSkyNode->attach(m_pViewer, 0);
    m_pSkyNode->setLighting(true);
    m_pSkyNode->addChild(m_pMapNode);
    m_pRoot->addChild(m_pSkyNode);

    //使用silverlining初始化天空
    //initSilverlining();

    //使用triton初始化海洋模块
    //initTriton();

    
}

void GraphicsView::initWindowManager()
{
    if(m_pWindowManager == nullptr)
    {

        m_pWindowManager = new osgWidget::WindowManager(m_pViewer, 50.f, 50.f, 1, /*0xF0000000*/
                                                        osgWidget::WindowManager::WM_USE_RENDERBINS);
        osg::Camera* camera = m_pWindowManager->createParentOrthoCamera();
        m_pRoot->addChild(camera);
        m_pViewer->addEventHandler(new osgWidget::MouseHandler(m_pWindowManager));//响应鼠标事件
        m_pViewer->addEventHandler(new osgWidget::KeyboardHandler(m_pWindowManager));//响应键盘事件
        m_pViewer->addEventHandler(new osgWidget::ResizeHandler(m_pWindowManager,camera));//防止WindowManager随着窗口改变而改变
        m_pViewer->addEventHandler(new osgWidget::CameraSwitchHandler(m_pWindowManager,camera));

    }

}

//加载.earth配置文件
void GraphicsView::setEarthFile(const QString& str)
{

    if (!str.isEmpty())
    {
        // 初始化地球数据;
        osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(str.toLocal8Bit().data());
        if (node)
        {
            m_pMapNode = osgEarth::MapNode::findMapNode(node);
            m_pMapNode->setName("MapNode");
            m_pMapSRS = m_pMapNode->getMapSRS();

            m_pViewer = new osgViewer::Viewer;
            m_pEarthManipulator = new osgEarth::Util::EarthManipulator;
            //m_pEarthWalkManipulator = new EarthWalkManipulator();
            m_pEarthManipulator->getSettings()->setMinMaxPitch(-360.0, 360.0);
            m_pEarthManipulator->getSettings()->setArcViewpointTransitions(true);
            m_pEarthManipulator->getSettings()->bindMouseDoubleClick(osgEarth::Util::EarthManipulator::ACTION_GOTO,
                                                                     osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON );
            m_pEarthManipulator->setNode(m_pMapNode);
            m_pViewer->setCameraManipulator(m_pEarthManipulator);

            m_pViewer->addEventHandler(new osgViewer::StatsHandler);
            m_pViewer->addEventHandler(new osgViewer::WindowSizeHandler());
            m_pViewer->addEventHandler(new osgViewer::ThreadingHandler());
            m_pViewer->addEventHandler(new osgViewer::LODScaleHandler());//运行用户即时调整场景细节层次(LOD)缩放比例。按*键场景中所有模型距离视点更近才显示，按/键模型再更大范围内被渲染显示
            m_pViewer->addEventHandler(new osgViewer::RecordCameraPathHandler);//记录相机运行路径，先按小z开始记录 再次按下z停止记录，最后按Z开始回放
            m_pViewer->addEventHandler(new osgGA::StateSetManipulator(m_pViewer->getCamera()->getOrCreateStateSet()));//按l键整个地球变亮，再按l又变回原来亮度
            //设置裁剪，避免屏幕下方出现黑边
            m_pViewer->getCamera()->addCullCallback(new osgEarth::Util::AutoClipPlaneCullCallback(m_pMapNode));
            m_pViewer->getCamera()->setNearFarRatio(0.0000003);
            m_pViewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
            m_pViewer->setUpViewerAsEmbeddedInWindow(0, 0, width(), height());
            //优化场景数据
            osgUtil::Optimizer optimizer;
            optimizer.optimize(m_pRoot.get());
            m_pViewer->setSceneData(m_pRoot.get());
            m_pGraphicsWindow = dynamic_cast<osgViewer::GraphicsWindow*>(m_pViewer->getCamera()->getGraphicsContext());
            m_pViewer->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);
            //视点跟随
            m_pEarthManipulator->setViewpoint(osgEarth::Util::Viewpoint("China", 112.971, 31.85, 444.02, -15.0, -60.0, 9000000), 5.0);

			//初始化天空
            initSky();
			//初始化标牌WindowManager
			initWindowManager();
        }
    }
}
