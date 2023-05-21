#version 410 core

layout(location = 0) in vec3  inPos;
uniform mat4 MVP;
uniform vec3 inColour;
uniform float pointSize;

out vec3 pointColour;

void main() {
    gl_PointSize = pointSize;
    pointColour = inColour;
    gl_Position = MVP * vec4(inPos, 1);
}
