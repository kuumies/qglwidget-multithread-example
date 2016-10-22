/* ---------------------------------------------------------------- *
   Author: Antti Jumpponen <kuumies@gmail.com>
   Desc:   Implmentation of kuu::opengl::Thread class.
 * ---------------------------------------------------------------- */

#include "opengl_thread.h"
#include <iostream>
#include "opengl_quad.h"

namespace kuu
{
namespace opengl
{

/* ---------------------------------------------------------------- *
   The data of the thread.
 * ---------------------------------------------------------------- */
struct Thread::Data
{
    Data(Widget::WeakPtr openglWidget)
        : openglWidget(openglWidget)
        , initialized(false)
        , render(true)
    {}

    Widget::WeakPtr openglWidget;
    bool initialized;
    bool render;
};

/* ---------------------------------------------------------------- *
   Constructs the thread.
 * -----------------------------------------------------------------*/
Thread::Thread(Widget::WeakPtr openglWidget)
    : d(std::make_shared<Data>(openglWidget))
{}

/* ---------------------------------------------------------------- *
   Starts the rendering thread if it is not running already.
 * ---------------------------------------------------------------- */
void Thread::start()
{
    if (isRunning())
        return;

    d->render = true;
    QThread::start();
}

/* ---------------------------------------------------------------- *
   Stops the rendering thread from the run state to idle state.
 * ---------------------------------------------------------------- */
void Thread::stop()
{
    d->render = false;

    quit();
    wait();
}

/* ---------------------------------------------------------------- *
   Runs the OpenGL rendering until the user stops it by calling
   stop() or the widget pointer goes to nullptr. If the operating
   system is Windows then the GLEW is initialized before rendering.
* ---------------------------------------------------------------- */
void Thread::run()
{
    Quad::Ptr quad;

    // Render untill the thread is stopped or widget is deleted.
    while(d->render)
    {
        // Get the widget pointer.
        Widget::Ptr widget = d->openglWidget.lock();
        if (!widget)
            break;

        // Make the widget context current.
        widget->makeCurrent();

        // Initialize OpenGL if needed.
        if (!d->initialized)
        {
#ifdef _WIN32
            glewExperimental = GL_TRUE;
            GLenum result = glewInit();
            if (result != GLEW_OK)
            {
                std::cerr << "Failed to initialize GLEW."
                          << std::endl;
                return;
            }
#endif
            quad = std::make_shared<Quad>();
            d->initialized = true;
        }

        // Clear the color buffer
        glViewport(0, 0, 720, 576);
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the quad
        quad->render();

        // Swap buffers and we're done.
        widget->swapBuffers();
        widget->doneCurrent();
    }
}

} // namespace opengl
} // namespace kuu
