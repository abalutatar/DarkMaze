#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
    vec3 ambient = 0.1 * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); // wektor od fragmentu do zrodla swiatla
    float diff = max(dot(norm, lightDir), 0.0); // iloczyn skalarny: kąt padania: 1 prostopadle, 0 równolegle lub z tyłu
    vec3 diffuse = diff * lightColor; //obliczenie koloru

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}
