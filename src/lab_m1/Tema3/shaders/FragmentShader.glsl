#version 330

// Struct light type
struct light_source
{
   int  type;
   vec3 light_position;
   vec3 light_direction;
   vec3 light_color;

   // Spotlight component
   float cut_off;

   // Difuze component
   float material_kd;

   // Specular component
   float material_ks;
   float material_shininess;
};

// Input
in vec2 texcoord;
in vec3 frag_color;
in vec3 world_position;
in vec3 world_normal;

// Uniform properties
uniform sampler2D texture;
uniform int has_texture;
uniform vec3 u_color;

uniform vec3 eye_position;

uniform vec3 colorAmbientalGlobal;
uniform vec3 color_ke;
uniform float material_ka;

uniform light_source lights[50];
uniform int light_number;


// Output
layout(location = 0) out vec4 out_color;

vec3 light_contribution(light_source light) {
    vec3 diffuse_specular;
    float dist;
    float att_fact;

    float cut_off;
    float spot_light;
    float spot_light_limit;
    float linear_att = 0;
    float light_att_factor = 0;

    vec3 L;
    if (light.type == 0) {
        L = vec3(0, 1.f, 0);
    } else {
        L = normalize( light.light_position - world_position );
    }


    // verify is att fact are too low to calculate light
    if (light.type == 1) {
        dist = distance(light.light_position, world_position);
        att_fact = 1.0 / ( pow(dist, 2) * 0.1f + dist * 0.5f + 1 );

        if (att_fact < 0.05f)
            return vec3(0, 0, 0);
    } else if (light.type == 2) {
            cut_off = radians(light.cut_off);
            spot_light = dot(-L, light.light_direction);
            spot_light_limit = cos(cut_off);

            if (spot_light > spot_light_limit) {
                linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
                light_att_factor = pow(linear_att, 2);
            }

            if (light_att_factor < 0.005f)
                return vec3(0, 0, 0);
    }

    vec3 V = normalize( eye_position - world_position );
    vec3 H = normalize( L + V );
    vec3 R =  reflect (-L, normalize(world_normal));

    float receive_light = dot( normalize(world_normal), L); 

    vec3 diffuse_light = light.material_kd * light.light_color * max( receive_light, 0 );
    vec3 specular_light = light.material_ks * light.light_color * receive_light * pow(max(dot(V, R), 0), light.material_shininess );

    // Directional
    if (light.type == 0) {
        diffuse_specular = diffuse_light + specular_light;

    // Punctiform
    } else if (light.type == 1) {
        diffuse_specular = att_fact * ( diffuse_light + specular_light );

    // Spotlight
    } else if (light.type == 2) {
        diffuse_specular = light_att_factor * ( diffuse_light + specular_light );
    }

    return diffuse_specular;

}

void main()
{
    // Choose color mode (texture or solid color)
    vec4 color;
    if (has_texture == 1) {
        color = texture2D(texture, texcoord);
    } else {
        color = vec4(u_color, 1);
    }

    if (color.a < 0.5) {
	    discard;
    }

    vec3 ambient_light = material_ka * colorAmbientalGlobal;
    vec3 emisive_light = color_ke;
    vec3 final_diffuse_specular = vec3(0.f, 0.f, 0.f);

    for (int i = 0; i < light_number; i++) {
        vec3 diffuse_specular = light_contribution(lights[i]);
        final_diffuse_specular += diffuse_specular;
    }

    vec3 final_light = emisive_light + ambient_light + final_diffuse_specular;

	out_color = color * vec4(final_light, 1);
}
