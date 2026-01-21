#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 2) in vec2 aTex;

out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoord = aTex;
    gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
}
