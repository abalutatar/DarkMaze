#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./lighting/Shader.h"
#include "./labyrinth/Labyrinth.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "../Camera.h"
#include "../Item.h"
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include "../tiny_obj_loader.h"

// functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void handleItems(float currentFrame);
// constants
const unsigned int SCR_WIDTH = 1200;  // zamiast 800
const unsigned int SCR_HEIGHT = 900; //zamiast 600

// funkcja przygotowująca VAO/VBO dla sześcianu 
unsigned int createCubeVAO() {
    float vertices[] = {
        // pos               // normal        // texcoords
        // tył
        -0.5f,-0.5f,-0.5f,   0.0f,0.0f,-1.0f, 0.0f,0.0f,
         0.5f,-0.5f,-0.5f,   0.0f,0.0f,-1.0f, 1.0f,0.0f,
         0.5f, 0.5f,-0.5f,   0.0f,0.0f,-1.0f, 1.0f,1.0f,
         0.5f, 0.5f,-0.5f,   0.0f,0.0f,-1.0f, 1.0f,1.0f,
        -0.5f, 0.5f,-0.5f,   0.0f,0.0f,-1.0f, 0.0f,1.0f,
        -0.5f,-0.5f,-0.5f,   0.0f,0.0f,-1.0f, 0.0f,0.0f,

        // przód
        -0.5f,-0.5f, 0.5f,   0.0f,0.0f,1.0f, 0.0f,0.0f,
         0.5f,-0.5f, 0.5f,   0.0f,0.0f,1.0f, 1.0f,0.0f,
         0.5f, 0.5f, 0.5f,   0.0f,0.0f,1.0f, 1.0f,1.0f,
         0.5f, 0.5f, 0.5f,   0.0f,0.0f,1.0f, 1.0f,1.0f,
        -0.5f, 0.5f, 0.5f,   0.0f,0.0f,1.0f, 0.0f,1.0f,
        -0.5f,-0.5f, 0.5f,   0.0f,0.0f,1.0f, 0.0f,0.0f,

        // lewa
        -0.5f,-0.5f,-0.5f,  -1.0f,0.0f,0.0f, 0.0f,0.0f,
        -0.5f,-0.5f, 0.5f,  -1.0f,0.0f,0.0f, 1.0f,0.0f,
        -0.5f, 0.5f, 0.5f,  -1.0f,0.0f,0.0f, 1.0f,1.0f,
        -0.5f, 0.5f, 0.5f,  -1.0f,0.0f,0.0f, 1.0f,1.0f,
        -0.5f, 0.5f,-0.5f,  -1.0f,0.0f,0.0f, 0.0f,1.0f,
        -0.5f,-0.5f,-0.5f,  -1.0f,0.0f,0.0f, 0.0f,0.0f,

        // prawa
         0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f, 0.0f,0.0f,
         0.5f,-0.5f, 0.5f,  1.0f,0.0f,0.0f, 1.0f,0.0f,
         0.5f, 0.5f, 0.5f,  1.0f,0.0f,0.0f, 1.0f,1.0f,
         0.5f, 0.5f, 0.5f,  1.0f,0.0f,0.0f, 1.0f,1.0f,
         0.5f, 0.5f,-0.5f,  1.0f,0.0f,0.0f, 0.0f,1.0f,
         0.5f,-0.5f,-0.5f,  1.0f,0.0f,0.0f, 0.0f,0.0f,

         // dół
         -0.5f,-0.5f,-0.5f, 0.0f,-1.0f,0.0f, 0.0f,0.0f,
          0.5f,-0.5f,-0.5f, 0.0f,-1.0f,0.0f, 1.0f,0.0f,
          0.5f,-0.5f, 0.5f, 0.0f,-1.0f,0.0f, 1.0f,1.0f,
          0.5f,-0.5f, 0.5f, 0.0f,-1.0f,0.0f, 1.0f,1.0f,
         -0.5f,-0.5f, 0.5f, 0.0f,-1.0f,0.0f, 0.0f,1.0f,
         -0.5f,-0.5f,-0.5f, 0.0f,-1.0f,0.0f, 0.0f,0.0f,

         // góra
         -0.5f,0.5f,-0.5f,  0.0f,1.0f,0.0f, 0.0f,0.0f,
          0.5f,0.5f,-0.5f,  0.0f,1.0f,0.0f, 1.0f,0.0f,
          0.5f,0.5f, 0.5f,  0.0f,1.0f,0.0f, 1.0f,1.0f,
          0.5f,0.5f, 0.5f,  0.0f,1.0f,0.0f, 1.0f,1.0f,
         -0.5f,0.5f, 0.5f,  0.0f,1.0f,0.0f, 0.0f,1.0f,
         -0.5f,0.5f,-0.5f,  0.0f,1.0f,0.0f, 0.0f,0.0f
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

unsigned int createQuadVAO() {
    float quadVertices[] = {
        // pos      // tex
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,

        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // pozycja → location 0 (2D)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texcoord → location 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    return VAO;
}

struct Mesh {
    unsigned int VAO, VBO, EBO;
    int indexCount;
};

Mesh loadOBJ(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ok = tinyobj::LoadObj(
        &attrib,
        &shapes,
        &materials,
        &err,
        path.c_str(),     // путь к OBJ
        "src/models/",            // папка с .mtl
        true                  // triangulate
    );

    if (!warn.empty()) std::cout << "WARN: " << warn << std::endl;
    if (!err.empty())  std::cerr << "ERR: " << err << std::endl;
    if (!ok) throw std::runtime_error("Failed to load OBJ");

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (const auto& shape : shapes) {
        for (const auto& idx : shape.mesh.indices) {
            // position
            vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

            // normal (если есть)
            if (idx.normal_index >= 0) {
                vertices.push_back(attrib.normals[3 * idx.normal_index + 0]);
                vertices.push_back(attrib.normals[3 * idx.normal_index + 1]);
                vertices.push_back(attrib.normals[3 * idx.normal_index + 2]);
            }
            else {
                vertices.insert(vertices.end(), { 0.0f, 1.0f, 0.0f });
            }

            // texcoord (если есть)
            if (idx.texcoord_index >= 0) {
                vertices.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                vertices.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
            }
            else {
                vertices.insert(vertices.end(), { 0.0f, 0.0f });
            }

            indices.push_back(indices.size());
        }
    }

    Mesh mesh;
    mesh.indexCount = indices.size();

    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    int stride = 8 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    return mesh;
}


enum class GameState {
    PLAYING,
    LEVEL_COMPLETE,
    GAME_OVER_LOSE,
    GAME_OVER_WIN
};

// settings
// Game state
GameState gameState = GameState::PLAYING;
int currentLevel = 1;
float levelDurations[3] = { 180.0f, 120.0f, 60.0f }; // 3 min, 2 min, 1 min
float timeRemaining = levelDurations[0];

// Labirinth
Labyrinth labyrinth;
std::vector<Item> activeItems;
int keysCollected = 0;

// camera
Camera camera(glm::vec3(1.0f, 0.7f, 1.0f));
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ciemny Labirynt", nullptr, nullptr);
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
    Shader uiShader("src/shaders/ui_vertex.vs", "src/shaders/ui_fragment.fs");

    // przygotowanie materialów
    unsigned int cubeVAO = createCubeVAO();
    unsigned int quadVAO = createQuadVAO();
    Mesh keyModel = loadOBJ("src/models/key_2.obj");
    unsigned int keyTex = shader.loadTexture("src/textures/key_2.png");
    Mesh batteryModel = loadOBJ("src/models/9V_Battery_Varta.obj");
    unsigned int batteryTex = shader.loadTexture("src/textures/9V_Battery_Varta_diffuse.png");
    unsigned int doorTex = shader.loadTexture("src/textures/door.png");
    unsigned int lavaTex = shader.loadTexture("src/textures/lava.png");
    unsigned int gameOverTex = shader.loadTexture("src/textures/game_over.png");
    unsigned int levelCompleteTex = shader.loadTexture("src/textures/level_complete.png");
    unsigned int youWonTex = shader.loadTexture("src/textures/you win.png");

    // generowanie labiryntu
    labyrinth.generateMaze(activeItems); // Przekazujemy wektor do uzupełnienia
    camera.maze = labyrinth.maze;


    shader.use();

    // początkowe wartości (będą aktualizowane co klatkę)
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor(0.8f, 0.3f, 0.8f);

    // kolor i zakres mgły
    //glm::vec3 fogColor(0.15f, 0.15f, 0.18f); // lekko niebieskawo-szary, pasuje do „ciemnego labiryntu”
    glm::vec3 fogColor(0.02f, 0.02f, 0.04f);
    float fogNear = 0.5f;//1.0f;   // zaczyna się 2 jednostki od kamery
    float fogFar = 15.0f;  // pełna mgła przy 12 jednostkach


    // ustawienia tłumienia (typowe wartości)
    float att_constant = 1.0f;
    float att_linear = 0.7f;
    float att_quadratic = 0.017f;

    float lightIntensity = 0.7f; // 0.0–1.0
    float lightRange = 1.0f;// 4.0f;     // zasięg latarki
    

    // główna pętla
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (gameState == GameState::PLAYING) {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        timeRemaining -= deltaTime;
        if (timeRemaining <= 0.0f) {
            timeRemaining = 0.0f;
            gameState = GameState::GAME_OVER_LOSE;
            //camera.isGameOver = true;
            // UKOŃCZENIE POZIOMU
            //if (!camera.isGameOver && keysCollected >= 3) {
                //camera.levelCompleted = true;
            //}
            std::cout << "Czas sie skonczyl! Przegrana.\n";
        }
        if (currentFrame < camera.lightEffectEndTime) {
            // Bonus trwa
            lightRange = camera.boostedLightRange;
        }
        else {
            // Bonus wygasł (lub wcale go nie było)
            lightRange = camera.baseLightRange;
        }
        shader.setFloat("cutoff", lightRange);
        shader.setFloat("fogFar", lightRange + 1.0f);
        // input
        // -----
        //processInput(window,lightRange);
        if (!camera.isGameOver) {
            processInput(window); // Gracz może się ruszać tylko jeśli żyje
            handleItems(currentFrame);
        }
        else {
            gameState = GameState::LEVEL_COMPLETE;
            // Logika po przegranej: możesz np. zmienić kolor światła na czerwony
            lightIntensity = glm::max(0.0f, lightIntensity - 2.0f * deltaTime);
            if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
                labyrinth.clearMaze();
                labyrinth.generateMaze(activeItems);
                camera = Camera(glm::vec3(1.0f, 0.7f, 1.0f));
                camera.maze = labyrinth.maze;
                lightIntensity = 0.7f;
            }
            shader.setFloat("lightIntensity", lightIntensity);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            lightIntensity = glm::min(1.0f, lightIntensity + 0.5f * deltaTime);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            lightIntensity = glm::max(0.0f, lightIntensity - 0.5f * deltaTime);

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            lightRange = glm::min(10.0f, lightRange + 1.5f * deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            lightRange = glm::max(1.0f, lightRange - 1.5f * deltaTime);

        shader.use();
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        // ustawienia światła w shaderze (co klatkę)
        //shader.setVec3("lightPos", lightPos);
        shader.setVec3("lightPos", camera.Position);
        shader.setVec3("lightColor", lightColor);
        shader.setVec3("objectColor", objectColor);
        shader.setVec3("viewPos", camera.Position);
        shader.setFloat("constant", att_constant);
        shader.setFloat("linear", att_linear);
        shader.setFloat("quadratic", att_quadratic);
        //shader.setFloat("cutoff", lightRange);
        // koniecznie ustaw softCutoff (0 = twardy)
        //shader.setFloat("softCutoff", 0.0f);
        shader.setFloat("lightIntensity", lightIntensity);


        // mgła
        shader.setVec3("fogColor", fogColor);
        shader.setFloat("fogNear", fogNear);
        //shader.setFloat("fogFar", fogFar);
        shader.setFloat("cutoff", lightRange);
        shader.setFloat("softCutoff", 1.5f); // Dodaj miękką krawędź (bardzo ważne dla smoothstep!)
        shader.setFloat("fogFar", lightRange + 1.0f);
        labyrinth.drawLabyrinth(shader, cubeVAO);

        for (auto& item : activeItems) {
            if (!item.collected) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, item.position);

                if (item.type == ItemType::KEY) {
                    //model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f));
                    model = glm::scale(model, glm::vec3(0.2f));
                    model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0, 0, 1));
                    shader.setVec3("objectColor", glm::vec3(0.941f, 0.925f, 0.141f));
                    shader.setMat4("model", model);

                    shader.setBool("useTexture", true);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, keyTex);
                    glBindVertexArray(keyModel.VAO);
                    glDrawElements(GL_TRIANGLES, keyModel.indexCount, GL_UNSIGNED_INT, 0);
                    glBindVertexArray(0);
                    shader.setBool("useTexture", false);
                }
                else if (item.type == ItemType::BATTERY) {
                    model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f));
                    model = glm::scale(model, glm::vec3(3.0f));
                    //model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
                    shader.setVec3("objectColor", glm::vec3(0.941f, 0.925f, 0.141f));
                    shader.setMat4("model", model);

                    shader.setBool("useTexture", true);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, batteryTex);
                    glBindVertexArray(batteryModel.VAO);
                    glDrawElements(GL_TRIANGLES, batteryModel.indexCount, GL_UNSIGNED_INT, 0);
                    glBindVertexArray(0);
                    shader.setBool("useTexture", false);
                }
                // main.cpp -> fragment rysujący TRAP
                else if (item.type == ItemType::TRAP) {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(item.position.x, 0.0f, item.position.z)); // на полу
                    model = glm::scale(model, glm::vec3(0.7f, 0.02f, 0.7f)); // тонкая плита

                    shader.setMat4("model", model);

                    shader.setBool("useTexture", true);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, lavaTex);
                    glBindVertexArray(cubeVAO);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    glBindVertexArray(0);
                    shader.setBool("useTexture", false);
                }
                else if (item.type == ItemType::EXIT) {
                    shader.setVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.0f)); // zielony

                    model = glm::scale(model, glm::vec3(0.2f, 1.0f, 1.0f)); // drzwi
                    shader.setMat4("model", model);

                    shader.setBool("useTexture", true);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, doorTex);
                    glBindVertexArray(cubeVAO);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    shader.setBool("useTexture", false);
                }

            }
        }

        // HUD – pasek czasu (uiShader)
        {
            glDisable(GL_DEPTH_TEST);

            uiShader.use();
            uiShader.setBool("useTexture", false);

            // --- Орто-проекция ---
            glm::mat4 projection = glm::ortho(
                0.0f, (float)SCR_WIDTH,
                0.0f, (float)SCR_HEIGHT
            );

            uiShader.setMat4("projection", projection);
            uiShader.setMat4("view", glm::mat4(1.0f)); // ВАЖНО: identity для UI

            // --- Пропорция времени ---
            float ratio = timeRemaining / levelDurations[currentLevel - 1];
            ratio = glm::clamp(ratio, 0.0f, 1.0f);

            // --- Размеры полосы ---
            float maxBarWidth = SCR_WIDTH - 40.0f;
            float barWidth = maxBarWidth * ratio;
            float barHeight = 20.0f;

            float barX = 20.0f + barWidth * 0.5f;
            float barY = SCR_HEIGHT - 30.0f;

            // --- Цвет (красный → зелёный) ---
            glm::vec3 barColor = glm::mix(
                glm::vec3(1.0f, 0.2f, 0.2f),
                glm::vec3(0.2f, 1.0f, 0.2f),
                ratio
            );

            uiShader.setVec3("color", barColor);

            // --- Model matrix ---
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(barX, barY, 0.0f));
            model = glm::scale(model, glm::vec3(barWidth, barHeight, 1.0f));
            uiShader.setMat4("model", model);

            // --- Рендер ---
            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

            glEnable(GL_DEPTH_TEST);
        }


    }
    else //if (gameState == GameState::LEVEL_COMPLETE)
    {
        glDisable(GL_DEPTH_TEST);

        uiShader.use();

        glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT);
        uiShader.setMat4("projection", projection);
        uiShader.setMat4("view", glm::mat4(1.0f));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(SCR_WIDTH * 0.5f, SCR_HEIGHT * 0.5f, 0.0f));
        model = glm::scale(model, glm::vec3(SCR_WIDTH, SCR_HEIGHT, 1.0f));

        uiShader.setMat4("model", model);
        uiShader.setBool("useTexture", true);
        glActiveTexture(GL_TEXTURE0);
        if (gameState == GameState::GAME_OVER_LOSE) {
            glBindTexture(GL_TEXTURE_2D, gameOverTex);
        }
        else if (gameState == GameState::GAME_OVER_WIN) {
            glBindTexture(GL_TEXTURE_2D, youWonTex);
        }
        else if (gameState == GameState::LEVEL_COMPLETE) {
            glBindTexture(GL_TEXTURE_2D, levelCompleteTex);
        }

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        uiShader.setBool("useTexture", false);

        glEnable(GL_DEPTH_TEST);

        // Sterowanie przejściem:
        if (gameState == GameState::LEVEL_COMPLETE) {
            if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                currentLevel++;
                if (currentLevel <= 3) {
                    timeRemaining = levelDurations[currentLevel - 1];
                    keysCollected = 0;
                    activeItems.clear();
                    labyrinth.clearMaze();
                    labyrinth.generateMaze(activeItems);
                    camera = Camera(glm::vec3(1.0f, 0.7f, 1.0f));
                    camera.maze = labyrinth.maze;
                    lightIntensity = 0.7f;

                    gameState = GameState::PLAYING;
                }
                else {
                    gameState = GameState::GAME_OVER_WIN;
                }
            }
        }
        else {
            if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
                currentLevel = 1;
                timeRemaining = levelDurations[0];
                keysCollected = 0;
                activeItems.clear();
                labyrinth.clearMaze();
                labyrinth.generateMaze(activeItems);
                camera = Camera(glm::vec3(1.0f, 0.7f, 1.0f));
                camera.maze = labyrinth.maze;
                lightIntensity = 0.7f;

                gameState = GameState::PLAYING;
            }
        }

    }

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
    float currentFrame = (float)glfwGetTime();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime, activeItems);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime, activeItems);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime, activeItems);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime, activeItems);
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

void handleItems(float currentFrame)
{
    for (auto& item : activeItems) {
        if (!item.collected || item.type == ItemType::EXIT) {
            float dist = glm::distance(camera.Position, item.position);
            if (dist < 0.5f) {
                if (item.type == ItemType::BATTERY && !item.collected) {
                    item.collected = true;
                    float currentTime = (float)glfwGetTime();

                    // Jeśli bonus już trwa, dodajemy czas. Jeśli nie, liczymy od teraz.
                    if (camera.lightEffectEndTime > currentTime)
                        camera.lightEffectEndTime += 10.0f;
                    else
                        camera.lightEffectEndTime = currentTime + 10.0f;
                }
                else if (item.type == ItemType::KEY && !item.collected) {
                    item.collected = true;
                    keysCollected++;
                    std::cout << "Klucz zebrany! (" << keysCollected << "/3)\n";
                }
                else if (item.type == ItemType::TRAP) {

                    // sprawdzamy tylko XZ
                    float distXZ = glm::distance(
                        glm::vec2(camera.Position.x, camera.Position.z),
                        glm::vec2(item.position.x, item.position.z)
                    );

                    // promień OTWORU (mniejszy niż kostka!)
                    if (distXZ < 0.25f) {
                        gameState = GameState::GAME_OVER_LOSE;
                        std::cout << "PRZEGRANA! Wpadles w otwarta zapadnie.\n";
                    }
                }
                else if (item.type == ItemType::EXIT) {

                    float distXZ = glm::distance(
                        glm::vec2(camera.Position.x, camera.Position.z),
                        glm::vec2(item.position.x, item.position.z)
                    );

                    if (keysCollected >= 3) {
                        std::cout << "Drzwi sie otwieraja...\n";
                        std::cout << "Zwyciestwo!\n";
                        item.collected = true;
                        gameState = GameState::LEVEL_COMPLETE;
                        camera.lightEffectEndTime = currentFrame + 9999.0f;
                    }
                    else {
                        std::cout << "Drzwi sa zamkniete. Potrzebujesz 3 kluczy.\n";
                    }
                }

            }
        }
    }
}


