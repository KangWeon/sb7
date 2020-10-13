/*
 * Copyright™ 2012-2015 Graham Sellers
 *
 * This code is part of the OpenGL SuperBible, 6th Edition.
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
#include <object.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::mat4;
using glm::vec3;
using glm::vec4;

using glm::perspective;
using glm::lookAt;
using glm::frustum;

using glm::identity;
using glm::translate;
using glm::rotate;
using glm::scale;

using glm::radians;
using glm::value_ptr;

class clipdistance_app : public sb7::application
{
public:
    clipdistance_app()
        : render_program(0),
          paused(false)
    {

    }

    void init()
    {
        static const char title[] = "OpenGL SuperBible - Clip Distance";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void startup();
    void render(double currentTime);
    void onKey(int key, int action);

protected:
    sb7::object     object;
    GLuint          render_program;
    bool            paused;

    struct
    {
        GLint   proj_matrix;
        GLint   mv_matrix;
        GLint   clip_plane;
        GLint   clip_sphere;
    } uniforms;

    void load_shaders();
};

void clipdistance_app::startup()
{
    object.load("media/objects/dragon.sbm");

    load_shaders();
}

void clipdistance_app::render(double currentTime)
{
    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static const GLfloat one = 1.0f;

    static double last_time = 0.0;
    static double total_time = 0.0;

    if (!paused)
        total_time += (currentTime - last_time);
    last_time = currentTime;

    float f = (float)total_time;

    glClearBufferfv(GL_COLOR, 0, black);
    glClearBufferfv(GL_DEPTH, 0, &one);

    glUseProgram(render_program);

    mat4 proj_matrix = perspective(radians(50.0f),
                                                 (float)info.windowWidth / (float)info.windowHeight,
                                                 0.1f,
                                                 1000.0f);

    mat4 mv_matrix = translate(identity<mat4>(), vec3(0.0f, 0.0f, -15.0f)) *
                            rotate(identity<mat4>(), radians(f * 0.34f), vec3(0.0f, 1.0f, 0.0f)) *
                            translate(identity<mat4>(), vec3(0.0f, -4.0f, 0.0f));

    mat4 plane_matrix = rotate(identity<mat4>(), radians(f * 6.0f), vec3(1.0f, 0.0f, 0.0f)) *
                               rotate(identity<mat4>(), radians(f * 7.3f), vec3(0.0f, 1.0f, 0.0f));

    vec4 plane = plane_matrix[0];
    plane[3] = 0.0f;
    plane = normalize(plane);

    vec4 clip_sphere = vec4(sinf(f * 0.7f) * 3.0f, cosf(f * 1.9f) * 3.0f, sinf(f * 0.1f) * 3.0f, cosf(f * 1.7f) + 2.5f);

    glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, value_ptr(proj_matrix));
    glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, value_ptr(mv_matrix));
    glUniform4fv(uniforms.clip_plane, 1, value_ptr(plane));
    glUniform4fv(uniforms.clip_sphere, 1, value_ptr(clip_sphere));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CLIP_DISTANCE0);
    glEnable(GL_CLIP_DISTANCE1);

    object.render();
}

void clipdistance_app::load_shaders()
{
    if (render_program)
        glDeleteProgram(render_program);

    GLuint shaders[] =
    {
        sb7::shader::load("media/shaders/clipdistance/render.vs.glsl", GL_VERTEX_SHADER),
        sb7::shader::load("media/shaders/clipdistance/render.fs.glsl", GL_FRAGMENT_SHADER)
    };

    render_program = sb7::program::link_from_shaders(shaders, 2, true);

    uniforms.proj_matrix = glGetUniformLocation(render_program, "proj_matrix");
    uniforms.mv_matrix = glGetUniformLocation(render_program, "mv_matrix");
    uniforms.clip_plane = glGetUniformLocation(render_program, "clip_plane");
    uniforms.clip_sphere = glGetUniformLocation(render_program, "clip_sphere");
}

void clipdistance_app::onKey(int key, int action)
{
    if (action)
    {
        switch (key)
        {
            case 'P':
                paused = !paused;
                break;
            case 'R': 
                load_shaders();
                break;
        }
    }
}

DECLARE_MAIN(clipdistance_app)

