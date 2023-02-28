#shader #vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 frag_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    normal = aNormal;
    frag_position = vec3(model * vec4(aPos, 1.0f));
}

#shader #fragment
#version 330 core
in vec3 normal;
in vec3 frag_position;

out vec4 frag_color;

uniform vec4 pipe_color; 
uniform vec3 camera_position;
uniform int shininess;

void main() {
    float ambient_strength  = 0.1f;
    float specular_strength = 0.6f;
    
    vec3 light_position = vec3(4.0f, 4.0f, 0.0f);
    vec3 light_color    = vec3(1.0f, 1.0f, 1.0f);
    vec3 ambient        = ambient_strength * light_color;

    vec3 norm              = normalize(normal);
    vec3 light_direction   = normalize(light_position - frag_position);
    vec3 view_direction    = normalize(camera_position - frag_position);
    vec3 reflect_direction = reflect(-light_direction, norm);

    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
    vec3 specular = specular_strength * spec * light_color;
    
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 result = (ambient + diffuse + specular) * vec3(pipe_color.xyz);
    frag_color = vec4(result, 1.0f);
}
