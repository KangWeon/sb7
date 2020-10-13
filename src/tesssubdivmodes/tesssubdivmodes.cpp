/*
 * Copyright � 2012-2015 Graham Sellers
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
#include <cmath>

#include <shader.h>

class tesssubdivmodes_app : public sb7::application
{
public:
    tesssubdivmodes_app()
        : program_index(0)
    {

    }

    void init()
    {
        static const char title[] = "OpenGL SuperBible - Subdivision Modes";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    virtual void startup()
    {

        GLuint vs = sb7::shader::load("media/shaders/tesssubdivmodes/tesssubdivmodes.vs.glsl", GL_VERTEX_SHADER);
        GLuint tcs_tri = sb7::shader::load("media/shaders/tesssubdivmodes/tesssubdivmodes_triangles.tcs.glsl", GL_TESS_CONTROL_SHADER);
        GLuint tes_equal = sb7::shader::load("media/shaders/tesssubdivmodes/tesssubdivmodes_equal.tes.glsl", GL_TESS_EVALUATION_SHADER);
        GLuint tes_fract_even = sb7::shader::load("media/shaders/tesssubdivmodes/tesssubdivmodes_fract_even.tes.glsl", GL_TESS_EVALUATION_SHADER);
        GLuint tes_fract_odd = sb7::shader::load("media/shaders/tesssubdivmodes/tesssubdivmodes_fract_odd.tes.glsl", GL_TESS_EVALUATION_SHADER);
        GLuint fs = sb7::shader::load("media/shaders/tesssubdivmodes/tesssubdivmodes.fs.glsl", GL_FRAGMENT_SHADER);

        int i;

        static GLuint vs_handles[] =
        {
            vs, vs, vs
        };

        static GLuint tcs_handles[] =
        {
            tcs_tri, tcs_tri, tcs_tri
        };

        static GLuint tes_handles[] =
        {
            tes_equal, tes_fract_even, tes_fract_odd
        };

        static GLuint fs_handles[] =
        {
            fs, fs, fs
        };

        for (i = 0; i < 3; i++)
        {
            program[i] = glCreateProgram();
            
            GLuint vs = vs_handles[i];

            GLuint tcs = tcs_handles[i];

            GLuint tes = tes_handles[i];

            GLuint fs = fs_handles[i];

            glAttachShader(program[i], vs);
            glAttachShader(program[i], tcs);
            glAttachShader(program[i], tes);
            glAttachShader(program[i], fs);
            glLinkProgram(program[i]);

            glDeleteShader(vs);
            glDeleteShader(tcs);
            glDeleteShader(tes);
            glDeleteShader(fs);
        }

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glPatchParameteri(GL_PATCH_VERTICES, 4);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    virtual void render(double currentTime)
    {
        static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, black);

        glUseProgram(program[program_index]);
        // glUniform1f(0, sinf((float)currentTime) * 5.0f + 6.0f);
        glUniform1f(0, 5.3f);
        glDrawArrays(GL_PATCHES, 0, 4);
    }

    virtual void shutdown()
    {
        int i;
        glDeleteVertexArrays(1, &vao);

        for (i = 0; i < 3; i++)
        {
            glDeleteProgram(program[i]);
        }
    }

    void onKey(int key, int action)
    {
        if (!action)
            return;

        switch (key)
        {
            case 'M': program_index = (program_index + 1) % 3;
                break;
        }
    }

private:
    GLuint          program[3];
    int             program_index;
    GLuint          vao;
};

DECLARE_MAIN(tesssubdivmodes_app)
