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

#include <shader.h>

// Undefine this to take color from screen space
//#define INTERPOLATE_COLOR 

class colorfromposition_app : public sb7::application
{
    void init()
    {
        static const char title[] = "OpenGL SuperBible - Simple Triangle";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    virtual void startup()
    {
        GLuint vs, fs;

#ifndef INTERPOLATE_COLOR
        vs = sb7::shader::load("media/shaders/fragcolorfrompos/fragcolorfrompos_not_inter.vs.glsl", GL_VERTEX_SHADER);
        fs = sb7::shader::load("media/shaders/fragcolorfrompos/fragcolorfrompos_not_inter.fs.glsl", GL_FRAGMENT_SHADER);
#else
        vs = sb7::shader::load("media/shaders/fragcolorfrompos/fragcolorfrompos_inter.vs.glsl", GL_VERTEX_SHADER);
        fs = sb7::shader::load("media/shaders/fragcolorfrompos/fragcolorfrompos_inter.fs.glsl", GL_FRAGMENT_SHADER);
#endif

        program = glCreateProgram();
        
        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
    }

    virtual void render(double currentTime)
    {
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, green);

        glUseProgram(program);
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

DECLARE_MAIN(colorfromposition_app)
