#include <stdio.h>

// TODO: maybe replace this with own implementation later
// NOTE: generated with: https://gen.glad.sh
#include "include/glad/glad.h"
#include "src/glad.c"
#include <GLFW/glfw3.h>

#include "include/general.h"
#include "include/math.h"

#include "include/platform.h"
#include "src/linux_platform.c"

#include "include/memory.h"

#define GB_SCREEN_WIDTH     160
#define GB_SCREEN_HEIGHT    144

v4f32 rgb(u8 red, u8 green, u8 blue, u8 alpha)
{
    v4f32 color = {};
    color.r = red / 255.0;
    color.g = green / 255.0;
    color.b = blue / 255.0;
    color.a = alpha / 255.0;
    return color;
}

typedef struct
{
    GLFWwindow *window;
    v2u32 window_size;
    b8 should_close;
}program_state;

global_variable program_state state = {};

typedef struct
{
    const char* vertex_source;
    const char* fragment_source;
}shader_source_files;

u32 
create_texture(temp_memory memory)
{
    u32 texture = 0;
    u32 texture_width = GB_SCREEN_WIDTH;
    u32 texture_height = GB_SCREEN_HEIGHT;
    u32 texture_mem_size = sizeof(v4u8) * texture_width * texture_height;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureStorage2D(texture, 1, GL_RGBA32F, texture_width, texture_height);

    v4u8 *pixels = (v4u8*)temp_memory_push(&memory, texture_mem_size);
    for(u32 y = 0; y < texture_height; ++y)
    {
        for(u32 x = 0; x < texture_width; x++)
        {
            u32 i = y * texture_width + x;
            pixels[i].r = (i % 2) ? 255 : 0;
            pixels[i].g = 0;
            pixels[i].b = 0;
            pixels[i].a = (i % 2) ? 0: 255;
        }
    }
    glTextureSubImage2D(
        texture,
        0,
        0, 0,
        texture_width, texture_height,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels
    );
    return(texture);
}

void
compile_and_attach_shader(u32 program, GLenum shader_type, const char *source, temp_memory memory)
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
        u8 *info = (u8*)temp_memory_push(&memory, info_length);
        glGetShaderInfoLog(
            shader,
            info_length,
            NULL,
            (GLchar*)info
        );
        //TODO: printf should be replaced
        printf("Shader Compilation Error:\t\n%s\n",info);
    }
}

u32 
create_program(shader_source_files sources, temp_memory memory)
{
    u32 program = glCreateProgram();

    u64 vertex_source_size = get_file_size(sources.vertex_source);
    u8 *vertex_source = (u8*)temp_memory_push(&memory, vertex_source_size);
    read_file(sources.vertex_source,  vertex_source_size, vertex_source);
    compile_and_attach_shader(program, GL_VERTEX_SHADER, TO_CONST_CSTRING(vertex_source), memory); 

    u64 fragment_source_size = get_file_size(sources.fragment_source);
    u8 *fragment_source = (u8*)temp_memory_push(&memory, fragment_source_size);
    read_file(sources.fragment_source,  fragment_source_size, fragment_source);
    compile_and_attach_shader(program, GL_FRAGMENT_SHADER, TO_CONST_CSTRING(fragment_source), memory);

    glLinkProgram(program);

    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        GLint info_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length);
        u8 *info = (u8*)temp_memory_push(&memory, info_length);
        glGetProgramInfoLog(
            program,
            info_length,
            NULL,
            (GLchar*)info
        );
        printf("Shader Linking Error:\t\n%s\n",info);
        program = 0;
    }

    return(program);
}

void frame_buffer_size_callback(GLFWwindow *window, int width, int height)
{
    state.window_size = (v2u32){(u32)width, (u32)height};

    u32 scale_x = state.window_size.width / GB_SCREEN_WIDTH;
    u32 scale_y = state.window_size.height / GB_SCREEN_HEIGHT;

    u32 scale = MIN(scale_x, scale_y);

    v2u32 viewport = {
        GB_SCREEN_WIDTH * scale,
        GB_SCREEN_HEIGHT * scale,
    };

    v2u32 border = {
        width - viewport.width,
        height - viewport.height,
    };

    border.width /= 2;
    border.height /= 2;

    glViewport(border.width, border.height, viewport.width, viewport.height);
}

s32 
main (void)
{
    state.window_size = (v2u32){
        GB_SCREEN_WIDTH * 2,
        GB_SCREEN_HEIGHT * 2,
    };

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
        /// INIT BLOCK
        ///
        glfwSetFramebufferSizeCallback(state.window, frame_buffer_size_callback);

        temp_memory scratch = create_temp_memory(10 * MB);

        u32 program = create_program((shader_source_files){"src/vertex.glsl", "src/fragment.glsl"}, scratch);
        glUseProgram(program);

        v4f32 color_0 = rgb(42, 69, 59, 0);
        v4f32 color_1 = rgb(54, 93, 72, 0);
        v4f32 color_2 = rgb(87, 124, 68, 0);
        v4f32 color_3 = rgb(127, 134, 15, 0);
        glUniform4f(0, color_0.r, color_0.g, color_0.b, color_0.a);
        glUniform4f(1, color_1.r, color_1.g, color_1.b, color_1.a);
        glUniform4f(2, color_2.r, color_2.g, color_2.b, color_2.a);
        glUniform4f(3, color_3.r, color_3.g, color_3.b, color_3.a);

        u32 texture = create_texture(scratch);
        glBindTexture(GL_TEXTURE_2D, texture);

        /// Main Loop

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
        destroy_temp_memory(&scratch);
    }
    return(0);
}
