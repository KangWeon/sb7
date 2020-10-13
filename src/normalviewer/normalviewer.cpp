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
#include <object.h>

using glm::mat4;
using glm::vec3;
using glm::vec4;

using glm::perspective;
using glm::lookAt;
//using glm::frustum;

using glm::identity;
using glm::translate;
//using glm::rotate;
//using glm::scale;

using glm::radians;
using glm::value_ptr;

class sb6mrender_app : public sb7::application
{
    void init()
    {
        static const char title[] = "OpenGL SuperBible - Normal Viewer";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    virtual void startup()
    {
        GLuint vs = sb7::shader::load("media/shaders/normalviewer/normalviewer.vs.glsl", GL_VERTEX_SHADER);
        GLuint gs = sb7::shader::load("media/shaders/normalviewer/normalviewer.gs.glsl", GL_GEOMETRY_SHADER);
        GLuint fs = sb7::shader::load("media/shaders/normalviewer/normalviewer.fs.glsl", GL_FRAGMENT_SHADER);

        program = glCreateProgram();
        
        glAttachShader(program, vs);
        glAttachShader(program, gs);
        glAttachShader(program, fs);

        glLinkProgram(program);

        glDeleteShader(fs);
        glDeleteShader(gs);
        glDeleteShader(vs);

        mv_location = glGetUniformLocation(program, "mv_matrix");
        proj_location = glGetUniformLocation(program, "proj_matrix");
        normal_length_location = glGetUniformLocation(program, "normal_length");

        object.load("media/objects/torus.sbm");

        // glEnable(GL_CULL_FACE);
        //glCullFace(GL_FRONT);

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
        glUniformMatrix4fv(proj_location, 1, GL_FALSE, value_ptr(proj_matrix));

         mat4 mv_matrix =  translate(vec3(0.0f, 0.0f, -3.0f)) *
                                 rotate(radians((float)currentTime * 15.0f), vec3(0.0f, 1.0f, 0.0f)) *
                                 rotate(radians((float)currentTime * 21.0f), vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(mv_location, 1, GL_FALSE, value_ptr(mv_matrix));

        glUniform1f(normal_length_location, sinf((float)currentTime * 8.0f) * cosf((float)currentTime * 6.0f) * 0.03f + 0.05f);

        object.render();
    }

    virtual void shutdown()
    {
        object.free();
        glDeleteProgram(program);
    }

private:
    GLuint          program;
    GLint           mv_location;
    GLint           proj_location;
    GLint           normal_length_location;

    sb7::object     object;
};

DECLARE_MAIN(sb6mrender_app)
