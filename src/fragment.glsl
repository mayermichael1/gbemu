#version 450 core

layout (binding = 0) uniform sampler2D image;
out vec4 color;

in vec2 texture_coordinate;

layout (location = 0) uniform vec4 color_0;
layout (location = 1) uniform vec4 color_1;
layout (location = 2) uniform vec4 color_2;
layout (location = 3) uniform vec4 color_3;

void
main(void)
{
    vec4 color_weights = texture(image, texture_coordinate);
    color = vec4(0.0);
    color += color_0 * color_weights.x;
    color += color_1 * color_weights.y;
    color += color_2 * color_weights.z;
    color += color_3 * color_weights.a;
}
