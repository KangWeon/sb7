/*
 * Copyright¢â 2012-2015 Graham Sellers
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

#include <sb7ktx.h>
#include <shader.h>

#include <cmath>

// Random number generator
static unsigned int seed = 0x13371337;

static inline float random_float()
{
    float res;
    unsigned int tmp;

    seed *= 16807;

    tmp = seed ^ (seed >> 4) ^ (seed << 15);

    *((unsigned int *) &res) = (tmp >> 9) | 0x3F800000;

    return (res - 1.0f);
}

enum
{
    NUM_STARS           = 2000
};

class starfield_app : public sb7::application
{
    void init()
    {
        static const char title[] = "OpenGL SuperBible - Shaped Points";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void startup()
    {
       

        GLuint vs = sb7::shader::load("media/shaders/shapedpoints/shapedpoints.vs.glsl", GL_VERTEX_SHADER);
        GLuint fs = sb7::shader::load("media/shaders/shapedpoints/shapedpoints.fs.glsl", GL_FRAGMENT_SHADER);
       
        render_prog = glCreateProgram();
        glAttachShader(render_prog, vs);
        glAttachShader(render_prog, fs);
        glLinkProgram(render_prog);

        glDeleteShader(vs);
        glDeleteShader(fs);

        glGenVertexArrays(1, &render_vao);
        glBindVertexArray(render_vao);
    }

    void render(double currentTime)
    {
        static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        static const GLfloat one[] = { 1.0f };
        float t = (float)currentTime;

        glViewport(0, 0, info.windowWidth, info.windowHeight);
        glClearBufferfv(GL_COLOR, 0, black);
        glClearBufferfv(GL_DEPTH, 0, one);

        glUseProgram(render_prog);

        glPointSize(200.0f);
        glBindVertexArray(render_vao);
        glDrawArrays(GL_POINTS, 0, 4);
    }

protected:
    GLuint          render_prog;
    GLuint          render_vao;
};

DECLARE_MAIN(starfield_app)
