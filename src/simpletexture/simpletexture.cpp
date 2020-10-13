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

#include <shader.h>

#include <string>

static void print_shader_log(GLuint shader)
{
    std::string str;
    GLint len;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len != 0)
    {
        str.resize(len);
        glGetShaderInfoLog(shader, len, NULL, &str[0]);
    }

#ifdef _WIN32
    OutputDebugStringA(str.c_str());
#endif
}

class simpletexture_app : public sb7::application
{
public:
    void init()
    {
        static const char title[] = "OpenGL SuperBible - Simple Texturing";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void startup(void)
    {
        // Generate a name for the texture
        glGenTextures(1, &texture);

        // Now bind it to the context using the GL_TEXTURE_2D binding point
        glBindTexture(GL_TEXTURE_2D, texture);

        // Specify the amount of storage we want to use for the texture
        glTexStorage2D(GL_TEXTURE_2D,   // 2D texture
                       8,               // 8 mipmap levels
                       GL_RGBA32F,      // 32-bit floating-point RGBA data
                       256, 256);       // 256 x 256 texels

        // Define some data to upload into the texture
        float * data = new float[256 * 256 * 4];

        // generate_texture() is a function that fills memory with image data
        generate_texture(data, 256, 256);

        // Assume the texture is already bound to the GL_TEXTURE_2D target
        glTexSubImage2D(GL_TEXTURE_2D,  // 2D texture
                        0,              // Level 0
                        0, 0,           // Offset 0, 0
                        256, 256,       // 256 x 256 texels, replace entire image
                        GL_RGBA,        // Four channel data
                        GL_FLOAT,       // Floating point data
                        data);          // Pointer to data

        // Free the memory we allocated before - \GL now has our data
        delete [] data;

        program = glCreateProgram();
        
        GLuint fs = sb7::shader::load("media/shaders/simpletexture/simpletexture.fs.glsl", GL_FRAGMENT_SHADER);

        print_shader_log(fs);

        GLuint vs = sb7::shader::load("media/shaders/simpletexture/simpletexture.vs.glsl", GL_VERTEX_SHADER);

        print_shader_log(vs);

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);

        glDeleteShader(fs);
        glDeleteShader(vs);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
    }

    void shutdown(void)
    {
        glDeleteProgram(program);
        glDeleteVertexArrays(1, &vao);
        glDeleteTextures(1, &texture);
    }

    void render(double t)
    {
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, green);

        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

private:
    void generate_texture(float * data, int width, int height)
    {
        int x, y;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                data[(y * width + x) * 4 + 0] = (float)((x & y) & 0xFF) / 255.0f;
                data[(y * width + x) * 4 + 1] = (float)((x | y) & 0xFF) / 255.0f;
                data[(y * width + x) * 4 + 2] = (float)((x ^ y) & 0xFF) / 255.0f;
                data[(y * width + x) * 4 + 3] = 1.0f;
            }
        }
    }

private:
    GLuint      texture;
    GLuint      program;
    GLuint      vao;
};

DECLARE_MAIN(simpletexture_app);
