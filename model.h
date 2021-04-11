/**
* @brief 所有模型的基类
* @author hph
* @date 2018/05/08
*/
#ifndef MODEL_H
#define MODEL_H
#include <iostream>
#include <QObject>
#include <qmath.h>
#include "Model/osgScutcheon.h"
#include "Model/findnodevisitor.h"
#include "3DEngine/graphicsview.h"
#include "Xml/xmlparse.h"
#include <QMutexLocker>





namespace NCSEModel {

///////////////////////////////
// Quaternion struct
// Simple incomplete quaternion struct for demo purpose
///////////////////////////////
struct Quaternion
{
    Quaternion():x(0), y(0), z(0), w(1){}
    Quaternion(double x, double y, double z, double w):x(x), y(y), z(z), w(w){}

    void normalize()
    {
        double norm = std::sqrt(x*x + y*y + z*z + w*w);
        x /= norm;
        y /= norm;
        z /= norm;
        w /= norm;
    }

    double norm()
    {
        return std::sqrt(x*x + y*y + z*z + w*w);
    }
    ///////////////////////////////
    // Quaternion to Euler
    ///////////////////////////////
    enum RotSeq{zyx, zyz, zxy, zxz, yxz, yxy, yzx, yzy, xyz, xyx, xzy,xzx};

    void twoaxisrot(double r11, double r12, double r21, double r31, double r32, double res[])
    {
        res[0] = atan2( r11, r12 );
        res[1] = acos ( r21 );
        res[2] = atan2( r31, r32 );
    }

    void threeaxisrot(double r11, double r12, double r21, double r31, double r32, double res[])
    {
        res[0] = atan2( r31, r32 );
        res[1] = asin ( r21 );
        res[2] = atan2( r11, r12 );
    }

    void quaternion2Euler(const Quaternion& q, double res[], RotSeq rotSeq)
    {
        switch(rotSeq){
        case zyx:
            threeaxisrot( 2*(q.x*q.y + q.w*q.z),
                          q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                          -2*(q.x*q.z - q.w*q.y),
                          2*(q.y*q.z + q.w*q.x),
                          q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                          res);
            break;

        case zyz:
            twoaxisrot( 2*(q.y*q.z - q.w*q.x),
                        2*(q.x*q.z + q.w*q.y),
                        q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                        2*(q.y*q.z + q.w*q.x),
                        -2*(q.x*q.z - q.w*q.y),
                        res);
            break;

        case zxy:
            threeaxisrot( -2*(q.x*q.y - q.w*q.z),
                          q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                          2*(q.y*q.z + q.w*q.x),
                          -2*(q.x*q.z - q.w*q.y),
                          q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                          res);
            break;

        case zxz:
            twoaxisrot( 2*(q.x*q.z + q.w*q.y),
                        -2*(q.y*q.z - q.w*q.x),
                        q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                        2*(q.x*q.z - q.w*q.y),
                        2*(q.y*q.z + q.w*q.x),
                        res);
            break;

        case yxz:
            threeaxisrot( 2*(q.x*q.z + q.w*q.y),
                          q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                          -2*(q.y*q.z - q.w*q.x),
                          2*(q.x*q.y + q.w*q.z),
                          q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                          res);
            break;

        case yxy:
            twoaxisrot( 2*(q.x*q.y - q.w*q.z),
                        2*(q.y*q.z + q.w*q.x),
                        q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                        2*(q.x*q.y + q.w*q.z),
                        -2*(q.y*q.z - q.w*q.x),
                        res);
            break;

        case yzx:
            threeaxisrot( -2*(q.x*q.z - q.w*q.y),
                          q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                          2*(q.x*q.y + q.w*q.z),
                          -2*(q.y*q.z - q.w*q.x),
                          q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                          res);
            break;

        case yzy:
            twoaxisrot( 2*(q.y*q.z + q.w*q.x),
                        -2*(q.x*q.y - q.w*q.z),
                        q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                        2*(q.y*q.z - q.w*q.x),
                        2*(q.x*q.y + q.w*q.z),
                        res);
            break;

        case xyz:
            threeaxisrot( -2*(q.y*q.z - q.w*q.x),
                          q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z,
                          2*(q.x*q.z + q.w*q.y),
                          -2*(q.x*q.y - q.w*q.z),
                          q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                          res);
            break;

        case xyx:
            twoaxisrot( 2*(q.x*q.y + q.w*q.z),
                        -2*(q.x*q.z - q.w*q.y),
                        q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                        2*(q.x*q.y - q.w*q.z),
                        2*(q.x*q.z + q.w*q.y),
                        res);
            break;

        case xzy:
            threeaxisrot( 2*(q.y*q.z + q.w*q.x),
                          q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z,
                          -2*(q.x*q.y - q.w*q.z),
                          2*(q.x*q.z + q.w*q.y),
                          q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                          res);
            break;

        case xzx:
            twoaxisrot( 2*(q.x*q.z - q.w*q.y),
                        2*(q.x*q.y + q.w*q.z),
                        q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z,
                        2*(q.x*q.z + q.w*q.y),
                        -2*(q.x*q.y - q.w*q.z),
                        res);
            break;
        default:
            std::cout << "Unknown rotation sequence" << std::endl;
            break;
        }
    }

//    ///////////////////////////////
//    // Helper functions
//    ///////////////////////////////
//    Quaternion operator*(Quaternion& q1, Quaternion& q2)
//    {
//      Quaternion q;
//      q.w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
//      q.x = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y;
//      q.y = q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x;
//      q.z = q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w;
//      return q;
//    }

//    std::ostream& operator <<(std::ostream& stream, const Quaternion& q)
//    {
//      std::cout << q.w << " " << q.x << "i " << q.y << "j " << q.z << "k";
//    }

//    double rad2deg(double rad){
//      return rad*180.0/M_PI;
//    }


    double x;
    double y;
    double z;
    double w;

};


class ModelLockCallback;
class ModelScaleCallback;
class ModelScutcheonCallback;
class Model : public QObject, public osg::Referenced
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    ~Model();

   
    /**
     * @brief addModel 添加模型
     * @param modelPath 模型文件所在路径
     * @param modelName 模型名称
     */
    void addModelNode(QString modelPath, QString modelName);

   
    /**
	 *创建标牌
     * @brief createScutcheon 
     */
    void createScutcheon();

	/**
	 *隐藏标牌
     * @brief hideScutcheon 
     */
    void hideScutcheon();
	/**
	 *显示标牌
     * @brief showScutcheon 
     */
    void showScutcheon();
    /**
	 *添加模型标牌的回调事件更新标牌
     * @brief addModelScaleCallback 
     */
    void addModelScutcheonCallback();

   

private:

    //一个完整的模型Model包含一个模型根节点；
    //模型根节点:包含一个模型类型节点；
    //模型类型节点：包含模型节点、模型轨迹线节点等；
    //模型节点：如果模型不需要缩放旋转等操作，则模型节点下包含原始模型节点；
    //—————————如果需要缩放等操作，则按照按照先Scale/Rotate/Translate顺序，模型节点下包含平移节点，
    //—————————平移节点下包含旋转节点，旋转节点下包含缩放节点，缩放节点下才包含原始模型节点；
    //标牌节点：标牌节点与对应的模型节点绑定，表示是具体某个模型节点的标牌。一个标牌节点包含多个一级标牌菜单，每个一级菜单又可以包含多个二级菜单
    //标牌管理器节点：标牌管理器节点统一管理所有标牌，标牌管理器全局唯一
    GraphicsView* m_pMap3D;
    ModelConfig m_modelConfig;
    int m_modelID;
    osg::ref_ptr<osg::Node> m_pNode;//原始模型节点
    osg::ref_ptr<osg::Group> m_pRootModel;//模型根节点

    osg::ref_ptr<osg::MatrixTransform> m_pModelNode;//模型节点

    osg::ref_ptr<osgScutcheonManager> m_pScutcheonManager;//标牌管理器，一个标牌管理器包含多个标牌
    osg::ref_ptr<osgScutcheon> m_pScutcheon;//模型标牌节点

};

 /**
  *添加模型标牌的回调事件更新标牌
 */
class ModelScutcheonCallback : public osg::NodeCallback
{
public:
    ModelScutcheonCallback(int menuIndex):
        m_menuIndex(menuIndex)
    {
        m_pNewScutcheonMenu = nullptr;
        m_pScutcheon = nullptr;
    }
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        m_pScutcheon = dynamic_cast<osgScutcheon*>(node);
        if(m_pScutcheon->getVisibility())
        {
            m_pScutcheon->setVisibility(true);
            m_pNewScutcheonMenu = m_pScutcheon->getLabelItemManager().at(m_menuIndex);//找到更新的一级标牌菜单
            updateScutcheonText();
        }
        else
        {
            m_pScutcheon->setVisibility(false);
        }

        traverse(node, nv);
    }

    void updateScutcheonText()
    {
        if(!m_pScutcheon || !m_pNewScutcheonMenu)
            return;
        m_pModelNode = m_pScutcheon->getTargetObject();
        osg::Vec3d currentPos = getModelPosition(m_pModelNode);

        osg::Vec3d scale = osg::Vec3d();
        osg::Quat so;
        osg::Vec3d translate = osg::Vec3d();
        //经过缩放后的模型最好使用decompose矩阵分解，不然getRotate所得旋转量可能不正确
        osg::Quat rotation;
        m_pModelNode->getMatrix().decompose(translate, rotation, scale, so);
        //        osg::Quat rotation = m_pModelNode->getMatrix().getRotate();
        //新方法
        Quaternion q(rotation.x(), rotation.y(), rotation.z(), rotation.w());
        q.normalize();
        double res[3];
        q.quaternion2Euler(q, res, Quaternion::zyx);
        double pitchAngle = res[0];
        double headingAngle = res[1];
        double rollAngle = res[2];
        //老方法
        //        double headingAngle = 0.0;
        //        double pitchAngle = 0.0;
        //        double rollAngle = 0.0;
        //        QuatToHPR(rotation, headingAngle, pitchAngle, rollAngle);

        QString lon = QStringLiteral("经度:") + QString::number(currentPos.x(), 'f', 6);
        QString lan = QStringLiteral("纬度:") + QString::number(currentPos.y(), 'f', 6);
        QString alt = QStringLiteral("高度:") + QString::number(currentPos.z(), 'f', 6);
        QString heading = QStringLiteral("航向:") + QString::number(osg::RadiansToDegrees(headingAngle), 'f', 6);
        QString pitch = QStringLiteral("俯仰:") + QString::number(osg::RadiansToDegrees(pitchAngle), 'f', 6);
        QString roll = QStringLiteral("滚转:") + QString::number(osg::RadiansToDegrees(rollAngle), 'f', 6);

        int menuItemIndex = 0;
        m_pNewScutcheonMenu->updateChildMenuText(menuItemIndex++, lon);//更新二级菜单
        m_pNewScutcheonMenu->updateChildMenuText(menuItemIndex++, lan);
        m_pNewScutcheonMenu->updateChildMenuText(menuItemIndex++, alt);
        m_pNewScutcheonMenu->updateChildMenuText(menuItemIndex++, heading);
        m_pNewScutcheonMenu->updateChildMenuText(menuItemIndex++, pitch);
        m_pNewScutcheonMenu->updateChildMenuText(menuItemIndex++, roll);

    }

    osg::Vec3d getModelPosition(osg::ref_ptr<osg::MatrixTransform> m_pModelNode)
    {
        osg::Vec3d posXYZ = osg::Vec3d();
        osg::Vec3d currentPosition = osg::Vec3d();
        if (m_pModelNode)
        {
            osg::Matrix mat = m_pModelNode->getMatrix();
            posXYZ = posXYZ * mat;
//            posXYZ = m_pModelNode->getMatrix().getTrans();
        }

        GraphicsView::instance()->getSRS()->getGeodeticSRS()->getEllipsoid()->convertXYZToLatLongHeight(posXYZ.x(), posXYZ.y(), posXYZ.z(),
                                                                                                        currentPosition.y(), currentPosition.x(),
                                                                                                        currentPosition.z());

        currentPosition.x() = osg::RadiansToDegrees(currentPosition.x());
        currentPosition.y() = osg::RadiansToDegrees(currentPosition.y());
        return currentPosition;
    }
    //ZXY
    void QuatToHPR(osg::Quat q, double& heading, double& pitch, double& roll)
    {
        double test = q.y() * q.z() + q.x() * q.w();

        if (test > 0.4999)
        { // singularity at north pole
            heading = 2.0 * atan2(q.y(), q.w());
            pitch   = osg::PI_2;
            roll    = 0.0;
            return;
        }
        if (test < -0.4999)
        { // singularity at south pole
            heading = 2.0 * atan2(q.y(), q.w());
            pitch   = -osg::PI_2;
            roll    = 0.0;
            return;
        }
        double sqx = q.x() * q.x();
        double sqy = q.y() * q.y();
        double sqz = q.z() * q.z();
        heading = atan2(2.0 * q.z() * q.w() - 2.0 * q.y() * q.x(), 1.0 - 2.0 * sqz - 2.0 * sqx);
        pitch   = asin(2.0 * test);
        roll    = atan2(2.0 * q.y() * q.w() - 2.0 * q.z() * q.x(), 1.0 - 2.0 * sqy - 2.0 * sqx);
    }

public:
    int m_menuIndex;//标牌一级菜单标识，判断是哪个菜单需要更新
    osg::ref_ptr<osgScutcheon> m_pScutcheon;//需要增加信息的标牌
    osg::ref_ptr<osgScutcheonMenu> m_pNewScutcheonMenu;//要更新的标牌菜单
    osg::ref_ptr<osg::MatrixTransform> m_pModelNode;//标牌绑定的模型节点
};



}
#endif // MODEL_H
