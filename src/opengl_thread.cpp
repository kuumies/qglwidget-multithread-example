/* ---------------------------------------------------------------- *
   Author: Antti Jumpponen <kuumies@gmail.com>
   Desc:   Implmentation of kuu::opengl::Thread class.
 * ---------------------------------------------------------------- */

#include "opengl_thread.h"
#include <chrono>
#include <iostream>

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

   The rendering will just change the background clear color.
   TODO: render a rotating quad
* ---------------------------------------------------------------- */
void Thread::run()
{
    using namespace std::chrono;
    time_point<steady_clock> startClock = steady_clock::now();
    float r = 0.0f, g = 0.0, b = 0.0f;
    bool rNeg= false, gNeg = false, bNeg = false;

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
            d->initialized = true;
        }

        // Clear the color buffer
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers and we're done.
        widget->swapBuffers();
        widget->doneCurrent();

        time_point<steady_clock> endClock = steady_clock::now();

        const int elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                endClock - startClock).count();
         startClock = endClock;

        const float rChangePerSecond = 0.12f;
        const float rChange = rChangePerSecond * elapsed / 1000.0f;
        const float gChangePerSecond = 0.13f;
        const float gChange = gChangePerSecond * elapsed / 1000.0f;
        const float bChangePerSecond = 0.14f;
        const float bChange = bChangePerSecond * elapsed / 1000.0f;

        // Adjust the color components to see that the thread
        // really is running.
        r = rNeg ? r - rChange : r + rChange;
        g = gNeg ? g - gChange : g + gChange;
        b = gNeg ? b - bChange : b + bChange;
        if (r > 1.0f) rNeg = !rNeg;
        if (r < 0.0f) rNeg = !rNeg;
        if (g > 1.0f) gNeg = !gNeg;
        if (g < 0.0f) gNeg = !gNeg;
        if (b > 1.0f) bNeg = !bNeg;
        if (b < 0.0f) bNeg = !bNeg;
    }
}

} // namespace opengl
} // namespace kuu
