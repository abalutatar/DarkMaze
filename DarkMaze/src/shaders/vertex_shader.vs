#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;    //skalowanie, rotacja , translacja w 3D
uniform mat4 view;     // ruch i orientacja kamery
uniform mat4 projection; // rzutowanie 3D na 2D

out vec3 FragPos; //pozycja wierzchołka po transformacji modelem
out vec3 Normal; //normalna wierzchołka



out vec2 TexCoords;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0)); //transformacja pozycji
    Normal = mat3(transpose(inverse(model))) * aNormal; //przekszatłcenie wektora normalnego przez macierz normalną
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0); 
}