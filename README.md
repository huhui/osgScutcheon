# osgScutcheon
osg/osgEarth 标牌管理器

因为原来整个Model类比较大，我只把里面关于标牌的重要信息提取出来。有问题欢迎大家在下面链接的博客中给我留言评论。

使用：

1、在Model类中，将osgScutcheon标牌节点绑定到osg::MatrixTransform模型节点上，

2、然后自定义标牌的一级和二级菜单的显示内容，

3、最后添加一个osg::NodeCallback来更新标牌节点一级和二级菜单上的数据。

效果：

参考博客：
https://blog.csdn.net/hph982775799/article/details/105655733


![image](https://user-images.githubusercontent.com/6800925/114290485-acddb800-9ab2-11eb-8b3d-1d7ceb0da5bc.png)

