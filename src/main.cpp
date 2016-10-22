/* -----------------------------------------------------------------*
    Author: Kuumies <kuumies@gmail.com>
    Desc:   Definition of GLWidget multithread example main entry.
 * -----------------------------------------------------------------*/

#include <QtGui/QIcon>
#include <QtOpengl/QGLFormat>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include "opengl_widget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    using namespace kuu;
    using namespace kuu::opengl;

    // Create the OpenGL format without fixed pipeline
    QGLFormat openglFormat;
    openglFormat.setVersion(3, 3);
    openglFormat.setProfile(QGLFormat::CoreProfile);
    openglFormat.setDoubleBuffer(true);
    openglFormat.setSampleBuffers(true);

    // Calculate the position of the widget. The widget should be
    // located so that the center is also at the center of desktop.
    const QDesktopWidget* desktop = QApplication::desktop();
    const QSize size(720, 576);
    const QPoint position(
        desktop->width()  / 2 - size.width()  / 2,
        desktop->height() / 2 - size.height() / 2);

    // Create the OpenGL widget
    Widget::Ptr widget = std::make_shared<Widget>(openglFormat);
    widget->setWindowIcon(QIcon("://icons/application_icon.png"));
    widget->resize(size);
    widget->move(position);
    widget->show();
    widget->startThread();

    return app.exec();
}
