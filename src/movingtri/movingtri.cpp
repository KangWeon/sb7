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
#include <shader.h>

using glm::vec4;
using glm::value_ptr;


class singlepoint_app : public sb7::application
{
    void init()
    {
        static const char title[] = "OpenGL SuperBible - Single Point";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    virtual void startup()
    {
        GLuint vs = sb7::shader::load("media/shaders/movingtri/movingtri.vs.glsl", GL_VERTEX_SHADER);
        GLuint fs = sb7::shader::load("media/shaders/movingtri/movingtri.fs.glsl", GL_FRAGMENT_SHADER);

        program = glCreateProgram();

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);

        glDeleteShader(fs);
        glDeleteShader(vs);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
    }

    virtual void render(double currentTime)
    {
        // Listing 3.2 ~ 3.6
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, green);

        glUseProgram(program);

        //GLfloat attrib[] = { (float)sin(currentTime) * 0.5f,
                             //(float)cos(currentTime) * 0.6f,
                             //0.0f, 0.0f };
        vec4 attrib = vec4(
            sinf(currentTime) * 0.5f,
            cosf(currentTime) * 0.6f,
            0.0f,
            0.0f);

        vec4 color = vec4(sinf(currentTime) * 0.5f + 0.5f,
            cosf(currentTime) * 0.5f + 0.5f,
            0.5f, //tanf(currentTime) * 0.25f + 0.5f,
            1.0f);

        glVertexAttrib4fv(0, value_ptr(attrib));
        glVertexAttrib4fv(1, value_ptr(color));

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    virtual void shutdown()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(program);
    }

private:
    GLuint          program;
    GLuint          vao;
};

DECLARE_MAIN(singlepoint_app)
