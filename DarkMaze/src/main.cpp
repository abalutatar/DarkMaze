#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./lighting/Shader.h"
#include "./labyrinth/Labyrinth.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "../Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


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

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    shader.use();

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
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        labyrinth.drawLabyrinth(shader, cubeVAO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// callback zmiany rozmiaru okna
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
