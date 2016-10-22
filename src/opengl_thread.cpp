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
   A simple timer for getting the elapsed time in milliseconds.
 * ---------------------------------------------------------------- */
class ElapsedTimer
{
public:
    // Shorthand aliases of clock
    using Clock = std::chrono::steady_clock;
    using ClockTimePoint = Clock::time_point;

    // Constructs the elapsed timer
    ElapsedTimer()
    {
        prevTime_ = Clock::now();
    }

    // Returns the elapsed time in milliseconds since the function
    // was previously called.
    int elapsed()
    {

        ClockTimePoint currentTime = Clock::now();
        auto diffTime = currentTime - prevTime_;
        prevTime_ = currentTime;

        using namespace std::chrono;
        return duration_cast<milliseconds>(diffTime).count();
    }

private:
    ClockTimePoint prevTime_; // previous sampling time
};

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
    // Quad that is going to be render
    Quad::Ptr quad;
    // Timer for rotating the quad.
    ElapsedTimer timer;

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
            quad = std::make_shared<Quad>(2.0f, 2.0f);
            d->initialized = true;
        }

        // Clear the color buffer
        glViewport(0, 0, 720, 576);
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the quad
        quad->update(timer.elapsed());
        quad->render();

        // Swap buffers and we're done.
        widget->swapBuffers();
        widget->doneCurrent();
    }
}

} // namespace opengl
} // namespace kuu
