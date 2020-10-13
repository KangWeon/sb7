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
#include <sb7textoverlay.h>
#include <sb7color.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <shader.h>

using glm::mat4;
using glm::vec3;
//using glm::vec4;

//using glm::perspective;
//using glm::lookAt;
//using glm::frustum;

//using glm::identity;
//using glm::translate;
//using glm::rotate;
//using glm::scale;

using glm::radians;
using glm::value_ptr;

class tessmodes_app : public sb7::application
{
public:
    tessmodes_app()
        : program_index(0)
    {

    }

    void init()
    {
        static const char title[] = "OpenGL SuperBible - Tessellation Modes";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    virtual void startup()
    {
 

        /*
        static const char * tes_source_isolines[] =
        {
            "#version 420 core                                                                    \n"
            "                                                                                     \n"
            "layout (isolines, equal_spacing, cw) in;                                             \n"
            "                                                                                     \n"
            "void main(void)                                                                      \n"
            "{                                                                                    \n"
            "    vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);       \n"
            "    vec4 p2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);       \n"
            "    gl_Position = mix(p1, p2, gl_TessCoord.y);                                       \n"
            "}                                                                                    \n"
        };
        */

        GLuint vs = sb7::shader::load("media/shaders/tessmodes/tessmodes.vs.glsl", GL_VERTEX_SHADER);
        
        GLuint tcs_quads = sb7::shader::load("media/shaders/tessmodes/tessmodes_quards.tcs.glsl", GL_TESS_CONTROL_SHADER);
        GLuint tcs_triangles = sb7::shader::load("media/shaders/tessmodes/tessmodes_triangles.tcs.glsl", GL_TESS_CONTROL_SHADER);
        GLuint tcs_isolines = sb7::shader::load("media/shaders/tessmodes/tessmodes_isolines.tcs.glsl", GL_TESS_CONTROL_SHADER);
        
        GLuint tes_quads = sb7::shader::load("media/shaders/tessmodes/tessmodes_quards.tes.glsl", GL_TESS_EVALUATION_SHADER);
        GLuint tes_triangles = sb7::shader::load("media/shaders/tessmodes/tessmodes_triangles.tes.glsl", GL_TESS_EVALUATION_SHADER);
        GLuint tes_triangles_as_points = sb7::shader::load("media/shaders/tessmodes/tessmodes_triangles_aspoints.tes.glsl", GL_TESS_EVALUATION_SHADER);
        GLuint tes_isolines = sb7::shader::load("media/shaders/tessmodes/tessmodes_isolines.tes.glsl", GL_TESS_EVALUATION_SHADER);
        
        GLuint fs = sb7::shader::load("media/shaders/tessmodes/tessmodes.fs.glsl", GL_FRAGMENT_SHADER);

        int i;

        static GLuint vs_handles[] =
        {
            vs, vs, vs, vs
        };

        static GLuint tcs_handles[] =
        {
            tcs_quads, tcs_triangles, tcs_triangles, tcs_isolines
        };

        static GLuint tes_handles[] =
        {
            tes_quads, tes_triangles, tes_triangles_as_points, tes_isolines
        };

        static GLuint fs_handles[] =
        {
            fs, fs, fs, fs
        };

        overlay.init(80, 50);

        for (i = 0; i < 4; i++)
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
        glClearBufferfv(GL_COLOR, 0, value_ptr(sb7::color::Black));

        glUseProgram(program[program_index]);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_PATCHES, 0, 4);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        static const char * mode_names[] =
        {
            "QUADS", "TRIANGLES", "QUADS AS POINTS", "ISOLINES"
        };

        overlay.clear();
        overlay.print("Mode: ");
        overlay.print(mode_names[program_index]);
        overlay.print(" (M toggles)");

        overlay.draw();
    }

    virtual void shutdown()
    {
        int i;
        glDeleteVertexArrays(1, &vao);

        for (i = 0; i < 4; i++)
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
            case 'M': program_index = (program_index + 1) % 4;
                break;
        }
    }

private:
    GLuint          program[4];
    int             program_index;
    GLuint          vao;
    sb7::text_overlay   overlay;
};

DECLARE_MAIN(tessmodes_app)
