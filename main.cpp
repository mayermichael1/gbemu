#include <stdio.h>

#include "include/general.h"

// TODO: maybe replace this with own implementation later
// NOTE: generated with: https://gen.glad.sh
#include "include/glad/glad.h"
#include "src/glad.c"

#include <GLFW/glfw3.h>

struct v4u8
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

struct v4f32
{
    f32 r;
    f32 g;
    f32 b;
    f32 a;
};

u32 
create_texture()
{
    u32 texture = 0;
    u32 texture_width = 160;
    u32 texture_height = 144;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTextureStorage2D(texture, 1, GL_RGBA8, texture_width, texture_height);

    v4f32 *pixels = (v4f32*)malloc(sizeof(v4f32) * texture_width * texture_height);
    for(u32 i = 0; i < texture_width * texture_height; i++)
    {
        pixels[i].r = 0.0;
        pixels[i].g = 1.0;
        pixels[i].b = 0.0;
        pixels[i].a = 1.0;
    }
    glTextureSubImage2D(
        texture,
        0,
        0, 0,
        texture_width, texture_height,
        GL_RGBA,
        GL_FLOAT,
        pixels
    );
    s32 error = glGetError();
    printf("OpenGL error: %i\n ", error);
    free(pixels);

    return(texture);
}

void
compile_and_attach_shader(u32 program, GLenum shader_type, const char *source)
{
    u32 shader = 0;
    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glAttachShader(program, shader);
    glDeleteShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        GLint info_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
        u8 *info = (u8*)malloc(info_length);
        glGetShaderInfoLog(
            shader,
            info_length,
            NULL,
            (GLchar*)info
        );
        //TODO: printf and free should be removed
        printf("Shader Compilation Error:\t\n%s\n",info);
        free(info);
    }
}

u32 
create_program()
{
    u32 program = glCreateProgram();

    compile_and_attach_shader(program, GL_VERTEX_SHADER, R"STRING(
#version 450 core

void
main(void)
{
    vec4[4] vertices = vec4[](
        vec4(-0.5, -0.5, 0.0, 1.0),
        vec4( 0.5, -0.5, 0.0, 1.0),
        vec4( 0.5,  0.5, 0.0, 1.0),
        vec4(-0.5,  0.5, 0.0, 1.0)
    );
    gl_Position = vertices[gl_VertexID];
}
    )STRING");

compile_and_attach_shader(program, GL_FRAGMENT_SHADER, R"STRING(
#version 450 core

layout (binding = 0) uniform sampler2D image;
out vec4 color;

void
main(void)
{
    //color = vec4(1.0);
    color = texelFetch(image, ivec2(gl_FragCoord.xy), 0);
}
    )STRING");

    glLinkProgram(program);

    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        GLint info_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length);
        u8 *info = (u8*)malloc(info_length);
        glGetProgramInfoLog(
            program,
            info_length,
            NULL,
            (GLchar*)info
        );
        printf("Shader Linking Error:\t\n%s\n",info);
        free(info);
        program = 0;
    }

    return(program);
}

s32 
main (void)
{
    GLFWwindow *window = NULL;
    b8 running = true;

    if(glfwInit())
    {
        window = glfwCreateWindow(160, 144, "GB-Screen", NULL, NULL);
    }

    glfwMakeContextCurrent(window);

    if(window)
    {
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            glfwDestroyWindow(window);
            window = NULL;
        }
    }

    if(window)
    {
        u32 program = create_program();
        glUseProgram(program);

        u32 texture = create_texture();
        glBindTexture(GL_TEXTURE_2D, texture);

        while(running)
        {
            glfwPollEvents();
            if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
                glfwWindowShouldClose(window))
            {
                running = false;
            }
            glClear(GL_COLOR_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glfwSwapBuffers(window);
        }
    }
    return 0;
}
