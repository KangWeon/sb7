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
#include <cmath>

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

class gstessellate_app : public sb7::application
{
    void init()
    {
        static const char title[] = "OpenGL SuperBible - Geometry Shader Tessellation";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    virtual void startup()
    {
       
        GLuint vs = sb7::shader::load("media/shaders/gstessellate/gstessellate.vs.glsl", GL_VERTEX_SHADER);
        GLuint gs = sb7::shader::load("media/shaders/gstessellate/gstessellate.gs.glsl", GL_GEOMETRY_SHADER);
        GLuint fs = sb7::shader::load("media/shaders/gstessellate/gstessellate.fs.glsl", GL_FRAGMENT_SHADER);


        program = glCreateProgram();
       

        glAttachShader(program, vs);
        glAttachShader(program, gs);
        glAttachShader(program, fs);

        glLinkProgram(program);

        mv_location = glGetUniformLocation(program, "mvMatrix");
        mvp_location = glGetUniformLocation(program, "mvpMatrix");
        stretch_location = glGetUniformLocation(program, "stretch");

        static const GLfloat tetrahedron_verts[] =
        {
             0.000f,  0.000f,  1.000f,
             0.943f,  0.000f, -0.333f,
            -0.471f,  0.816f, -0.333f,
            -0.471f, -0.816f, -0.333f
        };

        static const GLushort tetrahedron_indices[] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 3, 1,
            3, 2, 1
        };

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetrahedron_verts) + sizeof(tetrahedron_indices), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(tetrahedron_indices), tetrahedron_indices);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetrahedron_indices), sizeof(tetrahedron_verts), tetrahedron_verts);

        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(tetrahedron_indices));
        glEnableVertexAttribArray(0);

        glEnable(GL_CULL_FACE);
        // glDisable(GL_CULL_FACE);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }

    virtual void render(double currentTime)
    {
        static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        static const GLfloat one = 1.0f;
        float f = (float)currentTime;

        glViewport(0, 0, info.windowWidth, info.windowHeight);
        glClearBufferfv(GL_COLOR, 0, black);
        glClearBufferfv(GL_DEPTH, 0, &one);

        glUseProgram(program);

         mat4 proj_matrix =  perspective(radians(50.0f),
                                                     (float)info.windowWidth / (float)info.windowHeight,
                                                     0.1f,
                                                     1000.0f);
         mat4 mv_matrix =  translate(vec3(0.0f, 0.0f, -8.0f)) *
                                 rotate(radians((float)currentTime * 71.0f), vec3(0.0f, 1.0f, 0.0f)) *
                                 rotate(radians((float)currentTime * 10.0f), vec3(1.0f, 0.0f, 0.0f));

        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, value_ptr(proj_matrix * mv_matrix));

        glUniformMatrix4fv(mv_location, 1, GL_FALSE, value_ptr(mv_matrix));

        glUniform1f(stretch_location, sinf(f * 4.0f) * 0.75f + 1.0f);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, NULL);
    }

    virtual void shutdown()
    {
        glDeleteProgram(program);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &buffer);
    }

private:
    GLuint          program;
    GLint           mv_location;
    GLint           mvp_location;
    GLint           stretch_location;
    GLuint          vao;
    GLuint          buffer;
};

DECLARE_MAIN(gstessellate_app)
