/*
 * Copyright¢â 2012-2015 Graham Sellers
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
#include <sb7ktx.h>
#include <shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

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

#define DEPTH_TEXTURE_SIZE      4096
#define FRUSTUM_DEPTH           1000

class stereo_app : public sb7::application
{
public:
    stereo_app()
        : view_program(0),
          show_light_depth_program(0),
          mode(RENDER_FULL),
          paused(false),
          separation(2.0f)
    {
    }

protected:
    void init();
    void startup();
    void render(double currentTime);
    void render_scene(double currentTime);
    void onKey(int key, int action);

    void load_shaders();

    GLuint          view_program;
    GLint           show_light_depth_program;

    struct
    {
        struct
        {
            GLint   mvp;
        } light;
        struct
        {
            GLint   mv_matrix;
            GLint   proj_matrix;
            GLint   shadow_matrix;
            GLint   full_shading;
            GLint   specular_albedo;
            GLint   diffuse_albedo;
        } view;
    } uniforms;

    enum { OBJECT_COUNT = 4 };
    struct
    {
        sb7::object     obj;
        mat4     model_matrix;
    } objects[OBJECT_COUNT];

    mat4     light_view_matrix;
    mat4     light_proj_matrix;

    mat4     camera_view_matrix[2];
    mat4     camera_proj_matrix;

    GLuint          quad_vao;

    float           separation;

    enum
    {
        RENDER_FULL,
        RENDER_LIGHT,
        RENDER_DEPTH
    } mode;

    bool paused;
};

void stereo_app::init()
{
    sb7::application::init();

    info.flags.fullscreen = 1;
    info.flags.stereo = 1;
    info.windowWidth = 0;
    info.windowHeight = 0;

    static const char title[] = "OpenGL SuperBible - Texture Coordinates";

    memcpy(info.title, title, sizeof(title));
}

void stereo_app::startup()
{
    load_shaders();

    int i;

    static const char * const object_names[] =
    {
        "media/objects/dragon.sbm",
        "media/objects/sphere.sbm",
        "media/objects/cube.sbm",
        "media/objects/torus.sbm"
    };

    for (i = 0; i < OBJECT_COUNT; i++)
    {
        objects[i].obj.load(object_names[i]);
    }

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &quad_vao);
    glBindVertexArray(quad_vao);
}

void stereo_app::render(double currentTime)
{
    static const GLfloat zeros[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    static double last_time = 0.0;
    static double total_time = 0.0;

    if (!paused)
        total_time += (currentTime - last_time);
    last_time = currentTime;

    const float f = (float)total_time + 30.0f;

    vec3 light_position = vec3(20.0f, 20.0f, 20.0f);
    vec3 view_position = vec3(0.0f, 0.0f, 40.0f);

    light_proj_matrix = frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 200.0f);
    light_view_matrix = lookAt(light_position,
                                      vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));

    camera_proj_matrix = perspective(radians(50.0f),
                                            (float)info.windowWidth / (float)info.windowHeight,
                                            1.0f,
                                            200.0f);

    camera_view_matrix[0] = lookAt(view_position - vec3(separation, 0.0f, 0.0f),
                                          vec3(0.0f, 0.0f, -50.0f),
                                          vec3(0.0f, 1.0f, 0.0f));

    camera_view_matrix[1] = lookAt(view_position + vec3(separation, 0.0f, 0.0f),
                                          vec3(0.0f, 0.0f, -50.0f),
                                          vec3(0.0f, 1.0f, 0.0f));

    objects[0].model_matrix = rotate(radians(f * 14.5f), vec3(0.0f, 1.0f, 0.0f)) *
                              rotate(radians(20.0f), vec3(1.0f, 0.0f, 0.0f)) *
                              translate(vec3(0.0f, -4.0f, 0.0f));

    objects[1].model_matrix = rotate(radians(f * 3.7f), vec3(0.0f, 1.0f, 0.0f)) *
                              translate(vec3(sinf(f * 0.37f) * 12.0f, cosf(f * 0.37f) * 12.0f, 0.0f)) *
                              scale(vec3(2.0f));

    objects[2].model_matrix = rotate(radians(f * 6.45f), vec3(0.0f, 1.0f, 0.0f)) *
                              translate(vec3(sinf(f * 0.25f) * 10.0f, cosf(f * 0.25f) * 10.0f, 0.0f)) *
                              rotate(radians(f * 99.0f), vec3(0.0f, 0.0f, 1.0f)) *
                              scale(vec3(2.0f));

    objects[3].model_matrix = rotate(radians(f * 5.25f), vec3(0.0f, 1.0f, 0.0f)) *
                              translate(vec3(sinf(f * 0.51f) * 14.0f, cosf(f * 0.51f) * 14.0f, 0.0f)) *
                              rotate(radians(f * 120.3f), vec3(0.707106f, 0.0f, 0.707106f)) *
                              scale(vec3(2.0f));

    glEnable(GL_DEPTH_TEST);

    render_scene(total_time);
}

void stereo_app::render_scene(double currentTime)
{
    static const GLfloat ones[] = { 1.0f };
    static const GLfloat zero[] = { 0.0f };
    static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
    static const mat4 scale_bias_matrix = mat4(vec4(0.5f, 0.0f, 0.0f, 0.0f),
                                                             vec4(0.0f, 0.5f, 0.0f, 0.0f),
                                                             vec4(0.0f, 0.0f, 0.5f, 0.0f),
                                                             vec4(0.5f, 0.5f, 0.5f, 1.0f));
    mat4 light_vp_matrix = light_proj_matrix * light_view_matrix;
    mat4 shadow_sbpv_matrix = scale_bias_matrix * light_proj_matrix * light_view_matrix;

    glViewport(0, 0, info.windowWidth, info.windowHeight);
    glClearBufferfv(GL_COLOR, 0, gray);
    glUseProgram(view_program);
    glActiveTexture(GL_TEXTURE0);
    glUniformMatrix4fv(uniforms.view.proj_matrix, 1, GL_FALSE, value_ptr(camera_proj_matrix));
    glDrawBuffer(GL_BACK);

    int i, j;

    static const vec3 diffuse_colors[] =
    {
        vec3(1.0f, 0.6f, 0.3f),
        vec3(0.2f, 0.8f, 0.9f),
        vec3(0.3f, 0.9f, 0.4f),
        vec3(0.5f, 0.2f, 1.0f)
    };

    for (j = 0; j < 2; j++)
    {
        static const GLenum buffs[] = { GL_BACK_LEFT, GL_BACK_RIGHT };
        glDrawBuffer(buffs[j]);
        glClearBufferfv(GL_COLOR, 0, gray);
        glClearBufferfv(GL_DEPTH, 0, ones);
        for (i = 0; i < 4; i++)
        {
            mat4& model_matrix = objects[i].model_matrix;
            mat4 shadow_matrix = shadow_sbpv_matrix * model_matrix;
            glUniformMatrix4fv(uniforms.view.shadow_matrix, 1, GL_FALSE, value_ptr(shadow_matrix));
            glUniformMatrix4fv(uniforms.view.mv_matrix, 1, GL_FALSE, value_ptr(camera_view_matrix[j] * objects[i].model_matrix));
            glUniform1i(uniforms.view.full_shading, mode == RENDER_FULL ?  1 : 0);
            glUniform3fv(uniforms.view.diffuse_albedo, 1, value_ptr(diffuse_colors[i]));
            objects[i].obj.render();
        }
    }
}

void stereo_app::onKey(int key, int action)
{
    if (action)
    {
        switch (key)
        {
            case '1':
                mode = RENDER_FULL;
                break;
            case '2':
                mode = RENDER_LIGHT;
                break;
            case '3':
                mode = RENDER_DEPTH;
                break;
            case 'Z':
                separation += 0.05f;
                break;
            case 'X':
                separation -= 0.05f;
                break;
            case 'R': 
                load_shaders();
                break;
            case 'P':
                paused = !paused;
                break;
        }
    }
}

void stereo_app::load_shaders()
{
    GLuint vs;
    GLuint fs;

    vs = sb7::shader::load("media/shaders/stereo/stereo-render.vs.glsl", GL_VERTEX_SHADER);
    fs = sb7::shader::load("media/shaders/stereo/stereo-render.fs.glsl", GL_FRAGMENT_SHADER);

    if (view_program)
        glDeleteProgram(view_program);

    view_program = glCreateProgram();
    glAttachShader(view_program, vs);
    glAttachShader(view_program, fs);
    glLinkProgram(view_program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    uniforms.view.proj_matrix = glGetUniformLocation(view_program, "proj_matrix");
    uniforms.view.mv_matrix = glGetUniformLocation(view_program, "mv_matrix");
    uniforms.view.shadow_matrix = glGetUniformLocation(view_program, "shadow_matrix");
    uniforms.view.full_shading = glGetUniformLocation(view_program, "full_shading");
    uniforms.view.specular_albedo = glGetUniformLocation(view_program, "specular_albedo");
    uniforms.view.diffuse_albedo = glGetUniformLocation(view_program, "diffuse_albedo");
}

DECLARE_MAIN(stereo_app)
