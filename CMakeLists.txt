#-------------------------------------------------------------------------------
#   Author: Antti Jumpponen <kuumies@gmail.com>
#   Desc:   Main make file of the QGLWidget multi-thread example
#-------------------------------------------------------------------------------

PROJECT(qglwidget-multithread-example)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)

#-------------------------------------------------------------------------------
# Setup compiler and linker to support C++ 11 etc.

if(MSVC)
    # TODO: add MSVC options...
else(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ggdb -g3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(MSVC)

#-------------------------------------------------------------------------------
# Find packages and libraries. For windows use the GLEW from the external
# directory.

if(MSVC)
    set(GLEW_DIR ${CMAKE_CURRENT_LIST_DIR}/external/glew)
    set(GLEW_INCLUDE_DIR ${GLEW_DIR}/include/)
    set(GLEW_LIBRARIES ${GLEW_DIR}/lib/glew32.lib)
    include_directories(${GLEW_INCLUDE_DIR})
endif(MSVC)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(OpenGL REQUIRED)

#-------------------------------------------------------------------------------
# Set sources. The Qt files are auto-moc'd.

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

QT5_ADD_RESOURCES(RCC_RESOURCES
    resource/qglwidget-multithread-example.qrc
)

SET(SOURCE
    src/main.cpp
    src/opengl.h
    src/opengl_thread.cpp
    src/opengl_widget.cpp
)

#-------------------------------------------------------------------------------
# Add executable and link the needed libraries (Qt, OpenGL (+GLEW on windows)

add_executable(${PROJECT_NAME} ${SOURCE} ${RCC_RESOURCES})
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::OpenGL
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
)
