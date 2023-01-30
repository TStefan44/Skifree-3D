#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float Time;

uniform vec2 u_offsetUV;

// Output
out vec2 texcoord;
out vec3 frag_color;
out vec3 world_position;
out vec3 world_normal;

void main()
{
    vec4 model_position = Model * vec4(v_position, 1.0);
    vec2 pos = vec2(model_position.x, model_position.z) + vec2(cos(model_position.x + Time*6.2831) + sin(model_position.y + Time*6.2831) * 1.25f, sin(model_position.z + Time*6.2831)) * 0.15f;
    model_position.x = pos.x; model_position.z = pos.y;

    gl_Position = Projection * View * model_position;
    world_position = ( Model * vec4(v_position, 1) ).xyz;
    world_normal = normalize( mat3(Model) * v_normal );

    // Pass v_texture_coord as output to fragment shader
    texcoord = v_texture_coord + u_offsetUV;
}
