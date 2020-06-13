#version 410 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUV;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 normal;
out vec2 uv;

void main()
{
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
    normal=vNormal;
    uv=vUV;
}
