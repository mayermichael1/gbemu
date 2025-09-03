#include <stdio.h>

#include "include/general.h"
#include "include/math.h"

// TODO: maybe replace this with own implementation later
// NOTE: generated with: https://gen.glad.sh
#include "include/glad/glad.h"
#include "src/glad.c"

#include <GLFW/glfw3.h>

struct program_state
{
    GLFWwindow *window;
    v2u32 window_size;
    b8 should_close;
};

u32 
create_texture(program_state *state)
{
    u32 texture = 0;
    u32 texture_width = state->window_size.width;
    u32 texture_height = state->window_size.height;
    u32 texture_mem_size = sizeof(v4u8) * texture_width * texture_height;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    // NOTE: here GL_NEAREST has to be used instead of GL_LINEAR.
    //       linearly interpolating can only be used for float internal formats
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTextureStorage2D(texture, 1, GL_RGBA8UI, texture_width, texture_height);

    v4u8 *pixels = (v4u8*)malloc(texture_mem_size);
    for(u32 y = 0; y < texture_height; ++y)
    {
        for(u32 x = 0; x < texture_width; x++)
        {
            u32 i = y * texture_width + x;
            pixels[i].r = (i % 2) ? x : 0;
            pixels[i].g = 0;
            pixels[i].b = 0;
            pixels[i].a = 255;
        }
    }
    // NOTE: here GL_RGBA_INTEGER is also needed when integral format should be 
    //       used. This is not documented in docs.gl
    glTextureSubImage2D(
        texture,
        0,
        0, 0,
        texture_width, texture_height,
        GL_RGBA_INTEGER,
        GL_UNSIGNED_BYTE,
        pixels
    );
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

layout (binding = 0) uniform usampler2D image;
out vec4 color;

void
main(void)
{
    uvec4 color_value = texelFetch(image, ivec2(gl_FragCoord.xy), 0);
    color = color_value / 255.0;
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
    program_state state = {};
    state.window_size = {160, 144};

    if(glfwInit())
    {
        state.window = glfwCreateWindow(state.window_size.width, state.window_size.height, "GB-Screen", NULL, NULL);
    }

    glfwMakeContextCurrent(state.window);

    if(state.window)
    {
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            glfwDestroyWindow(state.window);
            state.window = NULL;
        }
    }

    if(state.window)
    {
        u32 program = create_program();
        glUseProgram(program);

        u32 texture = create_texture(&state);
        glBindTexture(GL_TEXTURE_2D, texture);

        while(!state.should_close)
        {
            glfwPollEvents();
            if( glfwGetKey(state.window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
                glfwWindowShouldClose(state.window))
            {
                state.should_close = true;
            }
            glClear(GL_COLOR_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glfwSwapBuffers(state.window);
        }
    }
    return(0);
}
