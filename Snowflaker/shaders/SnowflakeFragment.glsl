#version 410 core

layout (location = 0) out vec4 fragColor;

in vec3 pointColour;

void main() {
    fragColor = vec4(pointColour, 1.0);
}
