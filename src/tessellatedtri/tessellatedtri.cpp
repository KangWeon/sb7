/*
 * CopyrightĘ‚ 2012-2015 Graham Sellers
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

class tessellatedtri_app : public sb7::application
{
    void init()
    {
        static const char title[] = "OpenGL SuperBible - Tessellated Triangle";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    virtual void startup()
    {
        GLuint vs = sb7::shader::load("media/shaders/tessellatedtri/tessellatedtri.vs.glsl", GL_VERTEX_SHADER);
        GLuint tcs = sb7::shader::load("media/shaders/tessellatedtri/tessellatedtri.tcs.glsl", GL_TESS_CONTROL_SHADER);
        GLuint tes = sb7::shader::load("media/shaders/tessellatedtri/tessellatedtri.tes.glsl", GL_TESS_EVALUATION_SHADER);
        GLuint fs = sb7::shader::load("media/shaders/tessellatedtri/tessellatedtri.fs.glsl", GL_FRAGMENT_SHADER);

        program = glCreateProgram();
        

        glAttachShader(program, vs);
        glAttachShader(program, tcs);
        glAttachShader(program, tes);
        glAttachShader(program, fs);

        glLinkProgram(program);

        glDeleteShader(fs);
        glDeleteShader(tes);
        glDeleteShader(tcs);
        glDeleteShader(vs);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    virtual void render(double currentTime)
    {
        // Listing 3.7, 3.8
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, green);

        glUseProgram(program);
        glDrawArrays(GL_PATCHES, 0, 3);
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

DECLARE_MAIN(tessellatedtri_app)
