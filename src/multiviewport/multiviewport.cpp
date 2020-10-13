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

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

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
// #define MANY_CUBES

class multiviewport_app : public sb7::application
{
    void init()
    {
        static const char title[] = "OpenGL SuperBible - Multiple Viewports";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    virtual void startup()
    {
        GLuint vs = sb7::shader::load("media/shaders/multiviewport/multiviewport.vs.glsl", GL_VERTEX_SHADER);
        GLuint gs = sb7::shader::load("media/shaders/multiviewport/multiviewport.gs.glsl", GL_GEOMETRY_SHADER);
        GLuint fs = sb7::shader::load("media/shaders/multiviewport/multiviewport.fs.glsl", GL_FRAGMENT_SHADER);

        program = glCreateProgram();
        
        glAttachShader(program, vs);
        glAttachShader(program, gs);
        glAttachShader(program, fs);

        glLinkProgram(program);

        glDeleteShader(fs);
        glDeleteShader(gs);
        glDeleteShader(vs);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        static const GLushort vertex_indices[] =
        {
            0, 1, 2,
            2, 1, 3,
            2, 3, 4,
            4, 3, 5,
            4, 5, 6,
            6, 5, 7,
            6, 7, 0,
            0, 7, 1,
            6, 0, 2,
            2, 4, 6,
            7, 5, 3,
            7, 3, 1
        };

        static const GLfloat vertex_positions[] =
        {
            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,
             0.25f, -0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,
            -0.25f, -0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
        };

        glGenBuffers(1, &position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertex_positions),
                     vertex_positions,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(vertex_indices),
                     vertex_indices,
                     GL_STATIC_DRAW);

        glGenBuffers(1, &uniform_buffer);
        glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer);
        glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof( mat4), NULL, GL_DYNAMIC_DRAW);

        glEnable(GL_CULL_FACE);
        // glFrontFace(GL_CW);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }

    virtual void render(double currentTime)
    {
        int i;
        static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        static const GLfloat one = 1.0f;

        glViewport(0, 0, info.windowWidth, info.windowHeight);
        glClearBufferfv(GL_COLOR, 0, black);
        glClearBufferfv(GL_DEPTH, 0, &one);

        // Each rectangle will be 7/16 of the screen
        float viewport_width = (float)(7 * info.windowWidth) / 16.0f;
        float viewport_height = (float)(7 * info.windowHeight) / 16.0f;

        // Four rectangles - lower left first...
        glViewportIndexedf(0, 0, 0, viewport_width, viewport_height);

        // Lower right...
        glViewportIndexedf(1,
                           info.windowWidth - viewport_width, 0,
                           viewport_width, viewport_height);

        // Upper left...
        glViewportIndexedf(2,
                           0, info.windowHeight - viewport_height,
                           viewport_width, viewport_height);

        // Upper right...
        glViewportIndexedf(3,
                           info.windowWidth - viewport_width,
                           info.windowHeight - viewport_height,
                           viewport_width, viewport_height);

         mat4 proj_matrix =  perspective(radians(50.0f),
                                                     (float)info.windowWidth / (float)info.windowHeight,
                                                     0.1f,
                                                     1000.0f);

        float f = (float)currentTime * 0.3f;

        glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buffer);
         mat4 * mv_matrix_array = ( mat4 *)glMapBufferRange(GL_UNIFORM_BUFFER,
                                                                        0,
                                                                        4 * sizeof( mat4),
                                                                        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

        for (i = 0; i < 4; i++)
        {
            mv_matrix_array[i] = proj_matrix *
                                  translate(vec3(0.0f, 0.0f, -2.0f)) *
                                  rotate(radians((float)currentTime * 45.0f * (float)(i + 1)), vec3(0.0f, 1.0f, 0.0f)) *
                                  rotate(radians((float)currentTime * 81.0f * (float)(i + 1)), vec3(1.0f, 0.0f, 0.0f));
        }

        glUnmapBuffer(GL_UNIFORM_BUFFER);

        glUseProgram(program);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    }

    virtual void shutdown()
    {
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(program);
        glDeleteBuffers(1, &position_buffer);
    }

private:
    GLuint          program;
    GLuint          vao;
    GLuint          position_buffer;
    GLuint          index_buffer;
    GLuint          uniform_buffer;
    GLint           mv_location;
    GLint           proj_location;
};

DECLARE_MAIN(multiviewport_app)
