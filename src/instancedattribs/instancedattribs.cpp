/*
 * Copyright™ 2012-2015 Graham Sellers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <sb7.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <shader.h>

using glm::mat4;
using glm::vec3;
using glm::vec4;

using glm::perspective;
using glm::lookAt;
//using glm::frustum;

using glm::identity;
using glm::translate;
using glm::rotate;
using glm::scale;

using glm::radians;
using glm::value_ptr;


class instancing_app : public sb7::application
{
public:
    void init()
    {
        static const char title[] = "OpenGL SuperBible - Instanced Attributes";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void startup(void)
    {
        static const GLfloat square_vertices[] =
        {
            -1.0f, -1.0f, 0.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f,
             1.0f,  1.0f, 0.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 1.0f
        };

        static const GLfloat instance_colors[] =
        {
            1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f
        };

        static const GLfloat instance_positions[] =
        {
            -2.0f, -2.0f, 0.0f, 0.0f,
             2.0f, -2.0f, 0.0f, 0.0f,
             2.0f,  2.0f, 0.0f, 0.0f,
            -2.0f,  2.0f, 0.0f, 0.0f
        };

        GLuint offset = 0;

        glGenVertexArrays(1, &square_vao);
        glGenBuffers(1, &square_buffer);
        glBindVertexArray(square_vao);
        glBindBuffer(GL_ARRAY_BUFFER, square_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices) + sizeof(instance_colors) + sizeof(instance_positions), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(square_vertices), square_vertices);
        offset += sizeof(square_vertices);
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(instance_colors), instance_colors);
        offset += sizeof(instance_colors);
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(instance_positions), instance_positions);
        offset += sizeof(instance_positions);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(square_vertices));
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(square_vertices) + sizeof(instance_colors)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);

        GLuint square_vs = sb7::shader::load("media/shaders/instancedattribs/instancedattribs.vs.glsl", GL_VERTEX_SHADER);
        GLuint square_fs = sb7::shader::load("media/shaders/instancedattribs/instancedattribs.fs.glsl", GL_FRAGMENT_SHADER);

        square_program = glCreateProgram();

        glAttachShader(square_program, square_vs);
        glAttachShader(square_program, square_fs);

        glLinkProgram(square_program);

        //proj_location = glGetUniformLocation(square_program, "project_matrix");

        glDeleteShader(square_vs);
        glDeleteShader(square_fs);
    }

    void shutdown(void)
    {
        glDeleteProgram(square_program);
        glDeleteBuffers(1, &square_buffer);
        glDeleteVertexArrays(1, &square_vao);
    }

    void render(double t)
    {
        glViewport(0, 0, info.windowWidth, info.windowHeight);

        //mat4 project_matrix = perspective(radians(50.f), (float)info.windowWidth / info.windowHeight, 0.1f, 1000.f);


        static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        glClearBufferfv(GL_COLOR, 0, black);

        glUseProgram(square_program);
        //glUniformMatrix4fv(proj_location, 1, GL_FALSE, value_ptr(project_matrix));
        glBindVertexArray(square_vao);
        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 4);
    }

private:
    GLuint      square_buffer;
    GLuint      square_vao;

    GLuint      square_program;
    //GLint           proj_location;
};

DECLARE_MAIN(instancing_app);
