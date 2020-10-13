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

#include <object.h>
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
using glm::scale;

using glm::radians;
using glm::value_ptr;

class gsculling_app : public sb7::application
{
    void init()
    {
        static const char title[] = "OpenGL SuperBible - Geometry Shader Culling";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    virtual void startup()
    {
        program = glCreateProgram();

        GLuint vs = sb7::shader::load("media/shaders/gsculling/gsculling.vs.glsl", GL_VERTEX_SHADER);
        GLuint gs = sb7::shader::load("media/shaders/gsculling/gsculling.gs.glsl", GL_GEOMETRY_SHADER);
        GLuint fs = sb7::shader::load("media/shaders/gsculling/gsculling.fs.glsl", GL_FRAGMENT_SHADER);

        glAttachShader(program, vs);
        glAttachShader(program, gs);
        glAttachShader(program, fs);

        glLinkProgram(program);

        mv_location = glGetUniformLocation(program, "mvMatrix");
        mvp_location = glGetUniformLocation(program, "mvpMatrix");
        viewpoint_location = glGetUniformLocation(program, "viewpoint");

        object.load("media/objects/dragon.sbm");

        glDisable(GL_CULL_FACE);

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

        mat4 proj_matrix = perspective(radians(50.0f),
            (float)info.windowWidth / (float)info.windowHeight,
            0.1f,
            1000.0f);
        mat4 mv_matrix = translate(vec3(0.0f, 0.0f, -20.0f)) *
            rotate(radians((float)currentTime * 5.0f), vec3(0.0f, 1.0f, 0.0f)) *
            rotate(radians((float)currentTime * 100.0f), vec3(1.0f, 0.0f, 0.0f));

        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, value_ptr(proj_matrix * mv_matrix));

        glUniformMatrix4fv(mv_location, 1, GL_FALSE, value_ptr(mv_matrix));

        GLfloat vViewpoint[] = { sinf(f * 2.1f) * 70.0f, cosf(f * 1.4f) * 70.0f, sinf(f * 0.7f) * 70.0f };
        glUniform3fv(viewpoint_location, 1, vViewpoint);

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
    GLint           mvp_location;
    GLint           viewpoint_location;

    sb7::object     object;
};

DECLARE_MAIN(gsculling_app)

