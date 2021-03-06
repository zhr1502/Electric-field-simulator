# Electric-field-simulator
一个可绘制电场线的C++程序


~~凭借物理直觉直接画出含有多个带电粒子电场的电场线不是一件容易的事，所以我搓了这个玩意( )~~

## 简介
可输入多个带电粒子的二维坐标，带电量。程序会自动给出二维平面内的电场线。

最终给出的图中蓝色代表靠近负电荷，红色代表靠近正电荷

由于程序的不完善，电场线疏密/颜色渐变还不能很好地表示电场强度大小及电势高低

## 我该如何使用？

### 编译
程序依赖于opengl/freeglut

**你可能需要不同平台下freeglut的依赖**

vscode_config中给出了windows和linux下的编译选项及其他配置文件

如果你想要自行编译，需编译src中的draw.cpp,编译指令如下：

在windows下的编译命令:

``` g++ draw.cpp -o draw.exe -lfreeglut -lopengl32 -lwinmm ```

请注意，你可能需要加入-L来添加链接器的目录，编译完成后需要lib文件夹中的freeglut.dll与draw.exe在同一目录下才能运行。

在linux下的编译命令:

``` g++ draw.cpp -o draw -lGL -lglut ```

请注意，你需要在linux下安装freeglut，对于debian系的linux，使用apt安装即可，其他类型请自行查阅资料。

### 使用

运行可执行文件后应当按以下格式输入：

```
<带电粒子个数> <电场线数量系数>
<带电粒子的x坐标> <带电粒子的y坐标> <带电粒子的带电量> 
...(重复第二行内容直至输入完成)
```
电场线数量系数决定了电场线条数，若该系数较小则最终图样不明显，若系数过大会消耗性能。一般建议取20~50之间的数。

带电粒子的x坐标和y坐标最好限定在[-2,2]之间，不然可能超出边界

## Future Plan

完善电场线疏密问题

实现不考虑电磁波情况下点电荷的运动

优化效率
