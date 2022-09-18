#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsScene>
#include <QtGui/QPaintEngine>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QInputEvent>



class GraphicsView
{
    Q_OBJECT

protected:
    GraphicsView(const QString& strFile, QWidget* parent = 0);
    ~GraphicsView();

public:
    static GraphicsView *instance();

	//...
	//...
private:
    void init();
    void initSky();
    void initWindowManager();


private:
    osg::ref_ptr<osg::Group> m_pRoot;
    osg::ref_ptr<osgViewer::Viewer> m_pViewer;
    //....
	//....
	//....
	//标牌windowManger
    osg::ref_ptr<osgWidget::WindowManager> m_pWindowManager;


private:
    static GraphicsView* m_pInstance;

};

//GraphicsView g_pMap3D;

#endif // GRAPHICSVIEW_H
