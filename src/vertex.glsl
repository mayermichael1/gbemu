#version 450 core

out vec2 texture_coordinate;
void
main(void)
{
    /*
    vec4[4] vertices = vec4[](
        vec4(-0.5, -0.5, 0.0, 1.0),
        vec4( 0.5, -0.5, 0.0, 1.0),
        vec4( 0.5,  0.5, 0.0, 1.0),
        vec4(-0.5,  0.5, 0.0, 1.0)
    );
    */
    vec4[4] vertices = vec4[](
        vec4(-1.0, -1.0, 0.0, 1.0),
        vec4( 1.0, -1.0, 0.0, 1.0),
        vec4( 1.0,  1.0, 0.0, 1.0),
        vec4(-1.0,  1.0, 0.0, 1.0)
    );
    gl_Position = vertices[gl_VertexID];
    texture_coordinate = (vertices[gl_VertexID].xy + 1.0)/ 2.0;
}
