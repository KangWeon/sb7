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
//using glm::scale;

using glm::radians;
using glm::value_ptr;

// Remove this to draw only a single cube!
#define MANY_CUBES

class singlepoint_app : public sb7::application
{
    void init()
    {
        static const char title[] = "OpenGL SuperBible - Spinny Cube";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    virtual void startup()
    {
        GLuint vs, fs;

        vs = sb7::shader::load("media/shaders/spinnycube/spinnycube.vs.glsl", GL_VERTEX_SHADER);
        fs = sb7::shader::load("media/shaders/spinnycube/spinnycube.fs.glsl", GL_FRAGMENT_SHADER);

        program = glCreateProgram();
        
        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        mv_location = glGetUniformLocation(program, "mv_matrix");
        proj_location = glGetUniformLocation(program, "proj_matrix");

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        static const GLfloat vertex_positions[] =
        {
            -0.25f,  0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f,  0.25f,

            -0.25f,  0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
             0.25f,  0.25f,  0.25f,

             0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f, -0.25f
        };

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertex_positions),
                     vertex_positions,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);


        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        onResize(info.windowWidth, info.windowHeight);
    }

    virtual void render(double currentTime)
    {
        vec3 view_position = vec3(0.0f, 0.0f, -10.0f); // Camera Position
        mat4 view_matrix = lookAt(view_position,    // Camera Space = Camera Coordination System
            vec3(0.0f, 0.0f, 0.0f),
            vec3(0.0f, 1.0f, 0.0f));
        
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        static const GLfloat one = 1.0f;
        
        glClearBufferfv(GL_COLOR, 0, green);
        glClearBufferfv(GL_DEPTH, 0, &one);

        glUseProgram(program);
        glUniformMatrix4fv(proj_location, 1, GL_FALSE, value_ptr(proj_matrix));
       
#ifdef MANY_CUBES
        for (int i = 0; i < 32; i++)
        {
            float f = (float)i + (float)currentTime * 0.3f;
            mat4 mv_matrix = translate(vec3(0.0f, 0.0f, -4.0f)) *
                                    rotate(radians((float)currentTime * 45.0f), vec3(0.0f, 1.0f, 0.0f)) *
                                    rotate(radians((float)currentTime * 21.0f), vec3(1.0f, 0.0f, 0.0f)) *
                                    translate(vec3(sinf(2.1f * f) * 2.0f,
                                                     cosf(1.7f * f) * 2.0f,
                                                     sinf(1.3f * f) * cosf(1.5f * f) * 2.0f));
            glUniformMatrix4fv(mv_location, 1, GL_FALSE, value_ptr(view_matrix * mv_matrix));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
#else
        float f = (float)currentTime * 0.3f;
        mat4 mv_matrix = translate(vec3(0.0f, 0.0f, -4.0f)) *
                                translate(vec3(sinf(2.1f * f) * 0.5f,
                                                    cosf(1.7f * f) * 0.5f,
                                                    sinf(1.3f * f) * cosf(1.5f * f) * 2.0f)) *
                                rotate(radians((float)currentTime * 45.0f), vec3(0.0f, 1.0f, 0.0f)) *
                                rotate(radians((float)currentTime * 81.0f), vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(mv_location, 1, GL_FALSE, value_ptr(view_matrix * mv_matrix));
        glDrawArrays(GL_TRIANGLES, 0, 36);
#endif
    }

    virtual void shutdown()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(program);
        glDeleteBuffers(1, &buffer);
    }

    virtual void onResize(int w, int h)
    {
        glViewport(0, 0, w, h);
        aspect = (float)w / (float)h;
#ifdef MANY_CUBES
        proj_matrix = perspective(radians(50.0f), aspect, 0.1f, 1000.0f);
#else
        proj_matrix = perspective(radians(25.0f), aspect, 0.1f, 1000.0f);
#endif
    }

private:
    GLuint          program;
    GLuint          vao;
    GLuint          buffer;
    GLint           mv_location;
    GLint           proj_location;

    float           aspect;
    mat4     proj_matrix;
};

DECLARE_MAIN(singlepoint_app)
