/* ---------------------------------------------------------------- *
   Author: Kuumies <kuumies@gmail.com>
   Desc:   Definition of kuu::opengl::Thread class.
 * ---------------------------------------------------------------- */

#pragma once

#include <QtCore/QThread>
#include "opengl_widget.h"

namespace kuu
{
namespace opengl
{

class Widget;

/* ---------------------------------------------------------------- *
   A thread class for OpenGL multi-thread rendering. This thread
   uses the context of the OpenGL widget that is given in during
   construction. The rendering result will be displayed on the
   widget's surface.

   The thread is stopped when user calls stop() or the given in
   widget pointer goes to nullptr.

   The rendering is a simple rotating quad where shading is done
   with the vertex colors.
 * ---------------------------------------------------------------- */
class Thread : public QThread
{
public:
    // Defines a shared pointer of thread.
    using Ptr = std::shared_ptr<Thread>;

    // Constructs the thread from the widget.
    Thread(Widget::WeakPtr openGLWidget);

    // Sets the viewport size
    void setViewportSize(int width, int height);

    // Starts the rendering thread.
    void start();

    // Stops the rendering thread.
    void stop();

protected:
    virtual void run();

private:
    struct Data;
    std::shared_ptr<Data> d;
};

} // namespace opengl
} // namespace kuu
