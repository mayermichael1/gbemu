#version 450 core

layout (binding = 0) uniform usampler2D image;
out vec4 color;

in vec2 texture_coordinate;

void
main(void)
{
    uvec4 color_value = texture(image, texture_coordinate); 
    color = color_value / 255.0;
}
