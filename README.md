# qglwidget-multithread-example
A QGLWidget OpenGL multithread example

This is a simple example how to render into QGLWidget's surface from non-UI thread. The rendering result is a basic rotating quad mest where the shading is done with vertex colors. The example uses OpenGL 3.3 pipeline for rendering. The rendering is done using native OpenGL API, not with Qt OpenGL classes.
