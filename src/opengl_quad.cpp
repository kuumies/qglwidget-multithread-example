/* ---------------------------------------------------------------- *
   Author: Antti Jumpponen <kuumies@gmail.com>
   Desc:   Implmentation of kuu::opengl::Quad class.
 * ---------------------------------------------------------------- */

#include "opengl_quad.h"
#include <QtGui/QMatrix4x4>
#include <iostream>
#include <string>
#include <vector>
#include "opengl.h"

namespace kuu
{
namespace opengl
{

/* ---------------------------------------------------------------- *
   The data of the quad.
 * ---------------------------------------------------------------- */
struct Quad::Data
{
    Data(float width, float height)
        : width(width)
        , height(height)
    { createQuad(); }

    ~Data()
    { destroyQuad(); }

    void createQuad()
    {
        // Handle quad mesh
        {
            // Create vertices
            float w = width  * 0.5f;
            float h = height * 0.5f;
            const std::vector<float> vertexData =
            {
                -w, -h, 0.0f,
                 w, -h, 0.0f,
                 w,  h, 0.0f,
                -w,  h, 0.0f,
            };

            // Create triangle indices
            const std::vector<unsigned int> indexData =
            {
                0u, 1u, 2u,
                2u, 3u, 0u
            };

            // Create vertex buffer object
            glGenBuffers(1, &vbo);
            if (vbo == 0)
                std::cerr << "Failed to generate VBO" << std::endl;
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            GLint current = 0;
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &current);
            if (current != vbo)
                std::cerr << "Failed to bind VBO" << std::endl;

            glBufferData(GL_ARRAY_BUFFER,
                         vertexData.size() * sizeof(float),
                         &vertexData[0],
                         GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // Create index buffer object
            glGenBuffers(1, &ibo);
            if (ibo == 0)
                std::cerr << "Failed to generate IBO" << std::endl;
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &current);
            if (current != ibo)
                std::cerr << "Failed to bind IBO" << std::endl;
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         indexData.size() * sizeof(unsigned int),
                         &indexData[0],
                         GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // Create vertex array object
            glGenVertexArrays(1, &vao);
            if (vao == 0)
                std::cerr << "Failed to generate VAO" << std::endl;
            glBindVertexArray(vao);
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current);
            if (current != vao)
                std::cerr << "Failed to bind VAO" << std::endl;

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                0, 3, GL_FLOAT, GL_FALSE,
                3 * sizeof(float), (const GLvoid*) 0);
            glBindVertexArray(0);
        }

        // Handle quad shading
        {
            const std::string vshSource =
                "#version 330 core "
                "layout (location = 0) in vec3 position;"
                "uniform mat4 cameraMatrix;"
                "void main(void)"
                "{"
                   " gl_Position = cameraMatrix * vec4(position, 1.0);"
                "}";

            vsh = glCreateShader(GL_VERTEX_SHADER);
            if (vsh == 0)
                std::cerr << "Failed to create vertex shader"
                          << std::endl;
            const char* vshPtr = vshSource.c_str();
            glShaderSource(vsh, 1, &vshPtr, 0);
            glCompileShader(vsh);
            GLint status = 0;
            glGetShaderiv(vsh, GL_COMPILE_STATUS, &status);
            if (status != GL_TRUE)
                std::cerr << "Failed to compile vertex shader"
                          << std::endl;

            const std::string fshSource =
                "#version 330 core "

                "uniform vec3 color;"
                "out vec4 colorOut;"

                "void main(void)"
                "{"
                    "colorOut = vec4(color, 1.0);"
                "}";

            fsh = glCreateShader(GL_FRAGMENT_SHADER);
            if (fsh == 0)
                std::cerr << "Failed to create fragment shader"
                          << std::endl;
            const char* fshPtr = fshSource.c_str();
            glShaderSource(fsh, 1, &fshPtr, 0);
            glCompileShader(fsh);
            glGetShaderiv(fsh, GL_COMPILE_STATUS, &status);
            if (status != GL_TRUE)
                std::cerr << "Failed to compile fragment shader"
                          << std::endl;

            pgm = glCreateProgram();
            if (pgm == 0)
                std::cerr << "Failed to create shader program"
                          << std::endl;
            glAttachShader(pgm, vsh);
            glAttachShader(pgm, fsh);
            glLinkProgram(pgm);
            glGetProgramiv(pgm, GL_LINK_STATUS, &status);
            if (status != GL_TRUE)
                std::cerr << "Failed to ling shader program"
                          << std::endl;
        }
    }

    void destroyQuad()
    {
        // Destroy vertex and index buffers
        glDeleteBuffers(1, &ibo);
        glDeleteBuffers(1, &vbo);
        // Destroy vertex array
        glDeleteVertexArrays(1, &vao);
        // Destroy shader
        glDeleteShader(vsh);
        glDeleteShader(fsh);
        glDeleteProgram(pgm);
    }

    void drawQuad()
    {
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            3 * sizeof(float), (const GLvoid*) 0);

        glUseProgram(pgm);
        glValidateProgram(pgm);
        GLint status = 0;
        glGetProgramiv(pgm, GL_VALIDATE_STATUS, &status);
        if (status != GL_TRUE)
            std::cout << "Shader program is not valid" << std::endl;

        GLint uniformLocation = glGetUniformLocation(pgm, "color");
        if (uniformLocation == -1)
        {
            std::cerr << "Failed to find color "
                         "uniform location."
                      << std::endl;
            return;
        }
        glUniform3f(uniformLocation, 1.0f, 0.0f, 0.0f);

        uniformLocation = glGetUniformLocation(pgm, "cameraMatrix");
        if (uniformLocation == -1)
        {
            std::cerr << "Failed to find cameraMatrix "
                         "uniform location."
                      << std::endl;
            return;
        }

        QMatrix4x4 projection, view;
        projection.setToIdentity();
        projection.perspective(45.0f, 1.0f, 0.1f, 100.0f);
        view.setToIdentity();
        view.translate(0.0f, 0.0f, -5.0f);
        QMatrix4x4 camera = projection * view;

        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, camera.data());

        glDrawElements(
            GL_TRIANGLES,
            6, GL_UNSIGNED_INT, 0);

        glUseProgram(0);
        glBindVertexArray(0);
    }

    float width;  // width of the quad
    float height; // height of the quad

    GLuint vbo = 0;
    GLuint ibo = 0;
    GLuint vao = 0;
    GLuint vsh = 0;
    GLuint fsh = 0;
    GLuint pgm = 0;
};

/* ---------------------------------------------------------------- *
   Constructs the quad.
 * -----------------------------------------------------------------*/
Quad::Quad(float width, float height)
    : d(std::make_shared<Data>(width, height))
{}

/* ---------------------------------------------------------------- *
   Updates the quad.
 * -----------------------------------------------------------------*/
void Quad::update(float elapsed)
{

}

/* ---------------------------------------------------------------- *
   Renders the quad.
 * -----------------------------------------------------------------*/
void Quad::render()
{
    d->drawQuad();
}

} // namespace opengl
} // namespace kuu
