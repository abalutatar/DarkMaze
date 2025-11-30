#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

// attenuation params
uniform float constant;
uniform float linear;
uniform float quadratic;

// maksymalny zasiêg latarki (poza tym: ca³kowita czerñ)
uniform float cutoff;
// opcjonalne wyg³adzenie krawêdzi (0 = twardy cutoff, >0 = soft edge in world units)
uniform float softCutoff;

void main() {
    float dist2 = dot(lightPos - FragPos, lightPos - FragPos);
    float cutoff2 = cutoff * cutoff;

    // poza zasiêgiem - nic nie widaæ (twardy)
    if (dist2 > cutoff2) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // obliczenia oœwietlenia
    vec3 ambient = 0.07 * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specStrength = 0.5;
    float shininess = 32.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specStrength * spec * lightColor;

    float distance = sqrt(dist2);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    vec3 lighting = ambient * objectColor + (diffuse + specular) * objectColor * attenuation;

    // opcjonalne wyg³adzenie: przybli¿amy do czerni gdy jesteœmy tu¿ przed cutoff
    if (softCutoff > 0.0) {
        float edgeStart = max(0.0, cutoff - softCutoff);
        // poprawna kolejnoœæ: edgeStart -> cutoff
        float factor = smoothstep(edgeStart, cutoff, distance); // 1.0 przy bliskich, 0.0 przy cutoff
        lighting *= factor;
    }

    FragColor = vec4(lighting, 1.0);
}