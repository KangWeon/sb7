/*
 * Copyright � 2012-2013 Graham Sellers
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
#include <sb7ktx.h>
#include <shader.h>
#include <arcball.h>

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


class sb7mrender_app : public sb7::application
{
public:
    sb7mrender_app()
        : mouseDown(false)
    {
        mat_rotation = identity<mat4>();
    }

protected:
    void init()
    {
        static const char title[] = "OpenGL SuperBible - sb6 Model Rendering";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    virtual void startup()
    {
        
        loadShaders();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        object.load("media/objects/ladybug.sbm");

        glGenTextures(1, &tex_color);
        glGenTextures(1, &tex_normal);
        glActiveTexture(GL_TEXTURE0);
        sb7::ktx::file::load("media/textures/ladybug_co.ktx", tex_color);
        glActiveTexture(GL_TEXTURE1);
        sb7::ktx::file::load("media/textures/ladybug_nm.ktx", tex_normal);
    }

    virtual void render(double currentTime)
    {
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        static const GLfloat one = 1.0f;
        float f = (float)currentTime;

        glViewport(0, 0, info.windowWidth, info.windowHeight);
        glClearBufferfv(GL_COLOR, 0, green);
        glClearBufferfv(GL_DEPTH, 0, &one);

        glUseProgram(program);

        mat4 proj_matrix = perspective(radians(50.0f),
                                                     (float)info.windowWidth / (float)info.windowHeight,
                                                     0.1f,
                                                     1000.0f);
        glUniformMatrix4fv(proj_location, 1, GL_FALSE, value_ptr(proj_matrix));

        mat4 mv_matrix = translate(identity<mat4>(), vec3(0.0f, -0.5f, -7.0f)) *
            rotate(identity<mat4>(), radians((float)currentTime * 5.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(mv_location, 1, GL_FALSE, value_ptr(mv_matrix));

        object.render();
    }

    virtual void shutdown()
    {
        object.free();
        glDeleteProgram(program);
    }

    void onMouseButton(int button, int action)
    {
        int x, y;

        if (action == 1)
        {
            getMousePosition(x, y);
            arcball.onMouseDown(float(x), float(y));
            mouseDown = true;
        }
        else
        {
            mouseDown = false;
        }
    }

    void onMouseMove(int x, int y)
    {
        if (mouseDown)
        {
            arcball.onMouseMove(float(x), float(y));
            // mat_rotation = arcball.getRotation().asMatrix();
            mat_rotation = arcball.getRotationMatrix();
        }
    }

    void onResize(int w, int h)
    {
        sb7::application::onResize(w, h);

        arcball.setDimensions(float(w), float(h));
    }

    void onKey(int key, int action)
    {
        if (action)
        {
            switch (key)
            {
                case 'R': 
                    loadShaders();
                    break;
            }
        }
    }

    void loadShaders()
    {
        GLuint vs;
        GLuint fs;

        vs = sb7::shader::load("media/shaders/sb6mrender/render.vs.glsl", GL_VERTEX_SHADER);
        fs = sb7::shader::load("media/shaders/sb6mrender/render.fs.glsl", GL_FRAGMENT_SHADER);

        if (program != 0)
            glDeleteProgram(program);

        program = glCreateProgram();

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);

        mv_location = glGetUniformLocation(program, "mv_matrix");
        proj_location = glGetUniformLocation(program, "proj_matrix");

    }

private:
    GLuint          program;
    GLint           mv_location;
    GLint           proj_location;

    mat4         mat_rotation;

    GLuint              tex_color;
    GLuint              tex_normal;
    sb7::object         object;
    sb7::utils::arcball arcball;
    bool                mouseDown;
};

DECLARE_MAIN(sb7mrender_app)
