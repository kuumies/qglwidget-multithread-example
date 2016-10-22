/* ---------------------------------------------------------------- *
   Author: Antti Jumpponen <kuumies@gmail.com>
   Desc:   Implementation of kuu::opengl::Widget class.
 * ---------------------------------------------------------------- */

#include "opengl_widget.h"
#include "opengl_thread.h"
#include <iostream>
#include <QtGui/QResizeEvent>

namespace kuu
{
namespace opengl
{

/* ---------------------------------------------------------------- *
   The data of the widget. Note that the thread is constructed as
   a nullptr. See startThread() for the creation of the thread and
   stopThread() for its destruction.
 * ---------------------------------------------------------------- */
struct Widget::Data
{
    Thread::Ptr thread;
};

/* ---------------------------------------------------------------- *
   Constructs the widget from the given in OpenGL format.
 * -----------------------------------------------------------------*/
Widget::Widget(const QGLFormat& openglFormat)
    : QGLWidget(openglFormat)
    , d(std::make_shared<Data>())
{
    // Disable automatic buffer swap (swap is done in rendering thread)
    setAutoBufferSwap(false);
}

/* ---------------------------------------------------------------- *
   Starts the rendering thread. Before the thread can be start the
   current OpenGL context of the widget's surface must be moved from
   UI thread into the new rendering thread.
 * -----------------------------------------------------------------*/
void Widget::startThread()
{
    // Set the current context to be "no context".
    // Tell OpenGL to set "no context" as current context.
    doneCurrent();

    // Create the rendering thread. Give in pointer to this widget
    // as a shared pointer.
    d->thread = std::make_shared<Thread>(shared_from_this());

    // Move the OpenGL context into rendering thread.
    QGLContext* ctx = context();
    if (!ctx)
    {
        std::cerr << "No OpenGL context" << std::endl;
        return;
    }
    if (!ctx->isValid())
    {
        std::cerr << "OpenGL context is not valid" << std::endl;
        return;
    }
    ctx->moveToThread(d->thread.get());

    // Start the rendering thread.
    d->thread->start();
}

/* ---------------------------------------------------------------- *
   Stops the rendering thread in case it has been created. The
   shared pointer is reset so that this widget can be destroyed
   properly.
 * -----------------------------------------------------------------*/
void Widget::stopThread()
{
    if (d->thread)
    {
        d->thread->stop();
        d->thread.reset();
    }
}

/* ---------------------------------------------------------------- *
   Resize event is disabled for the rendering thread to work.
 * ---------------------------------------------------------------- */
void Widget::resizeEvent(QResizeEvent* event)
{
    const QSize newSize = event->size();
    if (d->thread)
        d->thread->setViewportSize(
            newSize.width(),
            newSize.height());
}

/* ---------------------------------------------------------------- *
   Paint event is disabled for the rendering thread to work.
 * ---------------------------------------------------------------- */
void Widget::paintEvent(QPaintEvent* /*event*/)
{}

/* ---------------------------------------------------------------- *
   Close event stops the thread.
 * ---------------------------------------------------------------- */
void Widget::closeEvent(QCloseEvent* /*event*/)
{
    stopThread();
}

} // namespace opengl
} // namespace kuu
