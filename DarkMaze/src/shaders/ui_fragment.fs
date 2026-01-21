#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform bool useTexture;

uniform vec3 color;

void main()
{
    if (useTexture) {
        FragColor = texture(texture1, TexCoord);
    } else {
        FragColor = vec4(color, 1.0);
    }
}
