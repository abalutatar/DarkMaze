#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./lighting/Shader.h"
#include "./labyrinth/Labyrinth.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
Labyrinth labyrinth;

// funkcja przygotowująca VAO/VBO dla sześcianu 
unsigned int createCubeVAO() {
    float vertices[] = {
        // pos               // normal          // texcoords
        // tył
        -0.5f,-0.5f,-0.5f,   0.0f, 0.0f,-1.0f,    0.0f, 0.0f,
         0.5f, 0.5f,-0.5f,   0.0f, 0.0f,-1.0f,    1.0f, 1.0f,
         0.5f,-0.5f,-0.5f,   0.0f, 0.0f,-1.0f,    1.0f, 0.0f,
         0.5f, 0.5f,-0.5f,   0.0f, 0.0f,-1.0f,    1.0f, 1.0f,
        -0.5f,-0.5f,-0.5f,   0.0f, 0.0f,-1.0f,    0.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,   0.0f, 0.0f,-1.0f,    0.0f, 1.0f,

        // przód
        -0.5f,-0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
         0.5f,-0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
         0.5f, 0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
        -0.5f,-0.5f, 0.5f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,

        // lewa
        -0.5f, 0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
        -0.5f,-0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
        -0.5f,-0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
        -0.5f,-0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,    1.0f, 0.0f,

        // prawa
         0.5f, 0.5f, 0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
         0.5f,-0.5f,-0.5f,   1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
         0.5f, 0.5f,-0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
         0.5f,-0.5f,-0.5f,   1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
         0.5f, 0.5f, 0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
         0.5f,-0.5f, 0.5f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f,

         // dół
         -0.5f,-0.5f,-0.5f,   0.0f,-1.0f, 0.0f,    0.0f, 1.0f,
          0.5f,-0.5f,-0.5f,   0.0f,-1.0f, 0.0f,    1.0f, 1.0f,
          0.5f,-0.5f, 0.5f,   0.0f,-1.0f, 0.0f,    1.0f, 0.0f,
          0.5f,-0.5f, 0.5f,   0.0f,-1.0f, 0.0f,    1.0f, 0.0f,
         -0.5f,-0.5f, 0.5f,   0.0f,-1.0f, 0.0f,    0.0f, 0.0f,
         -0.5f,-0.5f,-0.5f,   0.0f,-1.0f, 0.0f,    0.0f, 1.0f,

         // góra
         -0.5f, 0.5f,-0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
          0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
          0.5f, 0.5f,-0.5f,   0.0f, 1.0f, 0.0f,    1.0f, 1.0f,
          0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
         -0.5f, 0.5f,-0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
         -0.5f, 0.5f, 0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 0.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // layout location 0: position (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // layout location 1: normal (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // layout location 2: texcoords (2 floats)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //odwiązanie buforu i VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

int main() {
    // Konfiguracja
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ciemny Labirynt", NULL, NULL);
    if (!window) { std::cerr << "Failed to create GLFW window\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    //włączenie testu głębokości (bliższe elementy zasłaniają dalsze)
    glEnable(GL_DEPTH_TEST);

    //załadowanie shaderów
    Shader shader("src/shaders/vertex_shader.vs", "src/shaders/fragment_shader.fs");

    // przygotowanie VAO sześcianu
    unsigned int cubeVAO = createCubeVAO();

    // generowanie labiryntu
    labyrinth.generateMaze();

    // kamera i matryca widoku
    glm::mat4 view = glm::lookAt(glm::vec3(5.5f, 8.0f, 15.0f),
        glm::vec3(5.5f, 0.0f, 5.5f),// kamera za nad i za centrum 8>0 i 15>5.5
        glm::vec3(0.0f, 1.0f, 0.0f)); // góra
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),
        800.0f / 600.0f, 0.1f, 100.0f); //obiekty poza 0.1f a 100.0f nie będą widoczne

    //aktywacja shaderów i ustawienia w OpenGl

    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // ustawienia światła (przykładowe wartości) (nowe do głownej petli)
    glm::vec3 lightPos(5.0f, 10.0f, 5.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor(0.8f, 0.3f, 0.8f); //szary (0.8f, 0.8f, 0.8f)
    glm::vec3 cameraPos(5.5f, 8.0f, 15.0f);

    shader.setVec3("lightPos", lightPos);
    shader.setVec3("lightColor", lightColor);
    shader.setVec3("objectColor", objectColor);
    shader.setVec3("viewPos", cameraPos);


    // główna pętla
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.05f, 0.05f, 0.06f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        labyrinth.drawLabyrinth(shader, cubeVAO);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
