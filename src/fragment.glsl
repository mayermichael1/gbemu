#version 450 core

layout (binding = 0) uniform usampler2D image;
out vec4 color;

void
main(void)
{
    uvec4 color_value = texelFetch(image, ivec2(gl_FragCoord.xy), 0);
    color = color_value / 255.0;
    color = vec4(1.0);
}
