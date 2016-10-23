# qglwidget-multithread-example
A QGLWidget OpenGL multithread example

This is a simple example how to render into QGLWidget's surface from non-UI thread. The rendering result is a basic rotating quad mesh where the shading is done with vertex colors. The example uses OpenGL 3.3 pipeline for rendering. The rendering is done using native OpenGL API, not with Qt OpenGL classes.

## Building

This example requires c++11 support from the compiler. The example can be build by using cmake. See examples below.

**Create build directory**

1. mkdir example
2. cd example

**Clone the source**

1. git clone https://github.com/kuumies/qglwidget-multithread-example.git

**Run cmake**

1. mkdir build
2. cd build
3. cmake -DBUILD_TYPE=release ../.

**Build**

1. make

## Issues
- resizing window causes flickering.
