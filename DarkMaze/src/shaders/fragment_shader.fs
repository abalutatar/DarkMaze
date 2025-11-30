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

// zasiêg latarki
uniform float cutoff;
// miêkka krawêdŸ latarki (0 = twardo)
uniform float softCutoff;

// fog params
uniform vec3 fogColor;   // kolor mg³y
uniform float fogNear;   // dystans, od którego zaczyna siê mg³a
uniform float fogFar;    // dystans, przy którym jest pe³na mg³a

// intensywnoœæ œwiat³a (0.0–1.0)
uniform float lightIntensity;

void main() {
    // dystans od œwiat³a
    float dist2 = dot(lightPos - FragPos, lightPos - FragPos);
    float cutoff2 = cutoff * cutoff;

    // poza zasiêgiem latarki – czarno (przed fogiem)
    if (dist2 > cutoff2) {
        float camDist = distance(viewPos, FragPos);
        float fogFactor = clamp((fogFar - camDist) / (fogFar - fogNear), 0.0, 1.0);
        vec3 finalFogged = mix(fogColor, vec3(0.0), fogFactor);
        FragColor = vec4(finalFogged, 1.0);
        return;
    }

    // --- Phong lighting ---
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

    float distanceToLight = sqrt(dist2);
    float attenuation = 1.0 / (constant + linear * distanceToLight + quadratic * (distanceToLight * distanceToLight));

    // intensywnoœæ wp³ywa na ca³e oœwietlenie
    vec3 lighting = (ambient * objectColor + diffuse * objectColor * attenuation + specular * attenuation) * lightIntensity;

    // miêkki cutoff latarki
    if (softCutoff > 0.0) {
        float edgeStart = max(0.0, cutoff - softCutoff);
        float factor = smoothstep(edgeStart, cutoff, distanceToLight);
        lighting *= factor;
    }

    // --- Fog (liniowy) ---
    float camDist = distance(viewPos, FragPos);
    float fogFactor = clamp((fogFar - camDist) / (fogFar - fogNear), 0.0, 1.0);
    vec3 finalColor = mix(fogColor, lighting, fogFactor);

    FragColor = vec4(finalColor, 1.0);
}
