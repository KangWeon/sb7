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

#include <sb7ktx.h>

#include <shader.h>

using glm::mat4;
using glm::vec3;
using glm::vec4;

using glm::perspective;
using glm::lookAt;
//using glm::frustum;

//using glm::identity;
//using glm::translate;
//using glm::rotate;
//using glm::scale;

using glm::radians;
using glm::value_ptr;

class grass_app : public sb7::application
{
public:
    void init()
    {
        static const char title[] = "OpenGL SuperBible - Grass";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void startup(void)
    {
        static const GLfloat grass_blade[] =
        {
            -0.3f, 0.0f,
             0.3f, 0.0f,
            -0.20f, 1.0f,
             0.1f, 1.3f,
            -0.05f, 2.3f,
             0.0f, 3.3f
        };

        glGenBuffers(1, &grass_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, grass_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(grass_blade), grass_blade, GL_STATIC_DRAW);

        glGenVertexArrays(1, &grass_vao);
        glBindVertexArray(grass_vao);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);


        GLuint vs, fs;

        vs = sb7::shader::load("media/shaders/grass/grass.vs.glsl", GL_VERTEX_SHADER);
        fs = sb7::shader::load("media/shaders/grass/grass.fs.glsl", GL_FRAGMENT_SHADER);

        grass_program = glCreateProgram();

        glAttachShader(grass_program, vs);
        glAttachShader(grass_program, fs);

        glLinkProgram(grass_program);
        glDeleteShader(fs);
        glDeleteShader(vs);

        uniforms.mvpMatrix = glGetUniformLocation(grass_program, "mvpMatrix");

        glActiveTexture(GL_TEXTURE1);
        tex_grass_length = sb7::ktx::file::load("media/textures/grass_length.ktx");
        glActiveTexture(GL_TEXTURE2);
        tex_grass_orientation = sb7::ktx::file::load("media/textures/grass_orientation.ktx");
        glActiveTexture(GL_TEXTURE3);
        tex_grass_color = sb7::ktx::file::load("media/textures/grass_color.ktx");
        glActiveTexture(GL_TEXTURE4);
        tex_grass_bend = sb7::ktx::file::load("media/textures/grass_bend.ktx");
    }

    void shutdown(void)
    {
        glDeleteProgram(grass_program);
    }

    void render(double currentTime)
    {
        float t = (float)currentTime * 0.02f;
        float r = 550.0f;

        static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        static const GLfloat one = 1.0f;
        glClearBufferfv(GL_COLOR, 0, black);
        glClearBufferfv(GL_DEPTH, 0, &one);

        mat4 mv_matrix = lookAt(vec3(sinf(t) * r, 25.0f, cosf(t) * r),
                                              vec3(0.0f, -50.0f, 0.0f),
                                              vec3(0.0, 1.0, 0.0));
        mat4 prj_matrix = perspective(radians(45.0f), (float)info.windowWidth / (float)info.windowHeight, 0.1f, 1000.0f);

        glUseProgram(grass_program);
        glUniformMatrix4fv(uniforms.mvpMatrix, 1, GL_FALSE, value_ptr(prj_matrix * mv_matrix));

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glViewport(0, 0, info.windowWidth, info.windowHeight);

        glBindVertexArray(grass_vao);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, 1024 * 1024);
    }

private:
    GLuint      grass_buffer;
    GLuint      grass_vao;

    GLuint      grass_program;

    GLuint      tex_grass_color;
    GLuint      tex_grass_length;
    GLuint      tex_grass_orientation;
    GLuint      tex_grass_bend;

    struct
    {
        GLint   mvpMatrix;
    } uniforms;
};

DECLARE_MAIN(grass_app);
