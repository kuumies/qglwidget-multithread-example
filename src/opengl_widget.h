/* ---------------------------------------------------------------- *
   Author: Antti Jumpponen <kuumies@gmail.com>
   Desc:   Definition of kuu::opengl::Widget class.
 * ---------------------------------------------------------------- */

#pragma once

#include <memory>
#ifdef _WIN32
    #include "opengl.h"
    #include <QtOpenGL/QGLWidget>
#else
    #include <QtOpenGL/QGLWidget>
    #include "opengl.h"
#endif

namespace kuu
{
namespace opengl
{

/* ---------------------------------------------------------------- *
   An OpenGL widget whose OpenGL context is updated by a non-UI
   rendering thread that does all the drawing on widget's surface.
   See kuu::opengl::Thread class for information about the render-
   ing thread.

   The code below creates the widget with 3.3 core version of the
   OpenGL and starts the rendering thread. Note that the widget
   needs to be set visible before starting the thread.

        QGLFormat openglFormat;
        openglFormat.setVersion(3, 3);
        openglFormat.setProfile(QGLFormat::CoreProfile);
        openglFormat.setSampleBuffers(true);

        using namespace kuu::opengl;
        Widget::Ptr widget = std::make_shared<Widget>(openglFormat);
        widget->show();
        widget->startThread();

    The rendering thread must be stopped before the widget is de-
    stroyed. The rendering thread is stopped on close event.

 * ---------------------------------------------------------------- */
class Widget
    : public QGLWidget
    , public std::enable_shared_from_this<Widget>
{
public:
    // Defines a shared and weak pointers of widget.
    using Ptr     = std::shared_ptr<Widget>;
    using WeakPtr = std::weak_ptr<Widget>;

    // Constructs the widget.
    Widget(const QGLFormat& openglFormat);

    // Starts the rendering thread.
    void startThread();

    // Stops the rendering thread.
    void stopThread();

protected:
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent* event);
    void closeEvent(QCloseEvent* event);

private:
    struct Data;
    std::shared_ptr<Data> d;
};

} // namespace opengl
} // namespace kuu
