# osgScutcheon
osg/osgEarth 标牌管理器

使用：

1、在Model类中，将osgScutcheon标牌节点绑定到osg::MatrixTransform模型节点上，

2、然后自定义标牌的一级和二级菜单的显示内容，

3、最后添加一个osg::NodeCallback来更新标牌节点一级和二级菜单上的数据。
