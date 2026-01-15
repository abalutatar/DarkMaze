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


std::vector<Item> activeItems;
//HUDState hud;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float& lightRange);
void renderMiniMap(Shader& shader, Labyrinth& labyrinth, std::vector<Item>& activeItems, Camera& camera, unsigned int cubeVAO);


const unsigned int SCR_WIDTH = 1200;  // zamiast 800
const unsigned int SCR_HEIGHT = 900; //zamiast 600
Labyrinth labyrinth;

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

unsigned int createCylinderVAO(int segments = 24) {
    std::vector<float> vertices;
    const float radius = 0.5f;
    const float halfH = 0.5f;

    // bok walca (tri strip rozbity na trójkąty)
    for (int i = 0; i < segments; ++i) {
        float a0 = (float)i / segments * 2.0f * 3.1415926f;
        float a1 = (float)(i + 1) / segments * 2.0f * 3.1415926f;
        float x0 = cos(a0), z0 = sin(a0);
        float x1 = cos(a1), z1 = sin(a1);

        // dwa trójkąty paska
        // v0
        vertices.insert(vertices.end(), { radius * x0, -halfH, radius * z0,  x0, 0.0f, z0,  (float)i / segments, 0.0f });
        // v1
        vertices.insert(vertices.end(), { radius * x0,  halfH, radius * z0,  x0, 0.0f, z0,  (float)i / segments, 1.0f });
        // v2
        vertices.insert(vertices.end(), { radius * x1,  halfH, radius * z1,  x1, 0.0f, z1,  (float)(i + 1) / segments, 1.0f });

        // v0
        vertices.insert(vertices.end(), { radius * x0, -halfH, radius * z0,  x0, 0.0f, z0,  (float)i / segments, 0.0f });
        // v2
        vertices.insert(vertices.end(), { radius * x1,  halfH, radius * z1,  x1, 0.0f, z1,  (float)(i + 1) / segments, 1.0f });
        // v3
        vertices.insert(vertices.end(), { radius * x1, -halfH, radius * z1,  x1, 0.0f, z1,  (float)(i + 1) / segments, 0.0f });
    }

    // górna pokrywa (trójkąty fan)
    for (int i = 0; i < segments; ++i) {
        float a0 = (float)i / segments * 2.0f * 3.1415926f;
        float a1 = (float)(i + 1) / segments * 2.0f * 3.1415926f;
        float x0 = cos(a0), z0 = sin(a0);
        float x1 = cos(a1), z1 = sin(a1);

        // center
        vertices.insert(vertices.end(), { 0.0f, halfH, 0.0f,  0.0f, 1.0f, 0.0f,  0.5f, 0.5f });
        // edge0
        vertices.insert(vertices.end(), { radius * x0, halfH, radius * z0,  0.0f, 1.0f, 0.0f,  0.5f + 0.5f * x0, 0.5f + 0.5f * z0 });
        // edge1
        vertices.insert(vertices.end(), { radius * x1, halfH, radius * z1,  0.0f, 1.0f, 0.0f,  0.5f + 0.5f * x1, 0.5f + 0.5f * z1 });
    }

    // dolna pokrywa (trójkąty fan)
    for (int i = 0; i < segments; ++i) {
        float a0 = (float)i / segments * 2.0f * 3.1415926f;
        float a1 = (float)(i + 1) / segments * 2.0f * 3.1415926f;
        float x0 = cos(a0), z0 = sin(a0);
        float x1 = cos(a1), z1 = sin(a1);

        // center
        vertices.insert(vertices.end(), { 0.0f, -halfH, 0.0f,  0.0f, -1.0f, 0.0f,  0.5f, 0.5f });
        // edge1 (odwrócona kolejność dla poprawnego front-face)
        vertices.insert(vertices.end(), { radius * x1, -halfH, radius * z1,  0.0f, -1.0f, 0.0f,  0.5f + 0.5f * x1, 0.5f + 0.5f * z1 });
        // edge0
        vertices.insert(vertices.end(), { radius * x0, -halfH, radius * z0,  0.0f, -1.0f, 0.0f,  0.5f + 0.5f * x0, 0.5f + 0.5f * z0 });
    }

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

//unsigned int createKeyVAO() {
    struct Mesh {
        unsigned int VAO, VBO, EBO;
        int indexCount;
    };

//}

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
        "models/",            // папка с .mtl
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


// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;

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
    // shader dla wizualnej "latarki"
    Shader lampShader("src/shaders/lamp_vertex.vs", "src/shaders/lamp_fragment.fs");
   /* Shader hudShader(
        "src/shaders/hud_vertex.vs",
        "src/shaders/hud_fragment.fs"
    );
    */
    // przygotowanie VAO sześcianu
    unsigned int cubeVAO = createCubeVAO();
    unsigned int cylinderVAO = createCylinderVAO(24);
    int cylinderVertexCount = 24 * 6   // bok: na każdy segment 2 trójkąty = 6 wierzchołków
        + 24 * 3 // pokrywa górna: 1 trójkąt na segment
        + 24 * 3; // pokrywa dolna
    Mesh keyModel = loadOBJ("src/models/key_polygons.obj");


    // generowanie labiryntu
    labyrinth.generateMaze(activeItems); // Przekazujemy wektor do uzupełnienia
    camera.maze = labyrinth.maze;
    /*
    // przykładowe obiekty w labiryncie
    activeItems.push_back(Item(glm::vec3(2.0f, 0.5f, 3.0f), ItemType::KEY));
    activeItems.push_back(Item(glm::vec3(6.0f, 0.5f, 2.0f), ItemType::KEY));
    activeItems.push_back(Item(glm::vec3(8.0f, 0.5f, 7.0f), ItemType::KEY));

    activeItems.push_back(Item(glm::vec3(3.0f, 0.5f, 5.0f), ItemType::BATTERY));
    activeItems.push_back(Item(glm::vec3(7.0f, 0.5f, 4.0f), ItemType::BATTERY));
    activeItems.push_back(Item(glm::vec3(5.0f, 0.5f, 8.0f), ItemType::BATTERY));
    activeItems.push_back(Item(glm::vec3(9.0f, 0.5f, 3.0f), ItemType::BATTERY));
    activeItems.push_back(Item(glm::vec3(10.0f, 0.5f, 6.0f), ItemType::BATTERY));
    // przykładowe pułapki
    activeItems.push_back(Item(glm::vec3(4.0f, 0.5f, 4.0f), ItemType::TRAP));
    activeItems.push_back(Item(glm::vec3(6.0f, 0.5f, 6.0f), ItemType::TRAP));

    // wyjście – początkowo zablokowane
    activeItems.push_back(Item(glm::vec3(10.0f, 0.5f, 10.0f), ItemType::EXIT));
    activeItems.back().collected = true; // zablokowane do czasu zebrania kluczy

    */


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

    float lightIntensity = 1.0f; // 0.0–1.0
    float lightRange = 1.0f;// 4.0f;     // zasięg latarki
    

    int keysCollected = 0;
    // główna pętla
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
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
            processInput(window, lightRange); // Gracz może się ruszać tylko jeśli żyje
        }
        else {
            // Logika po przegranej: możesz np. zmienić kolor światła na czerwony
            lightIntensity = glm::max(0.0f, lightIntensity - 2.0f * deltaTime);
            shader.use();
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


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Aktualizuj pozycję światła — podążaj za kamerą.
        // Mały offset do przodu, żeby nie znajdować się dokładnie wewnątrz kamery.
        glm::vec3 lightPos = camera.Position + camera.Front * 0.1f + glm::vec3(0.0f, -0.05f, 0.0f);

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
        /*
        updateHUD(hud, camera, camera.keysCollected, lightRange);

        drawHUD(
            hudShader,
            cubeVAO,
            hud,
            SCR_WIDTH,
            SCR_HEIGHT
        );
        */
        // mini-mapa w prawym dolnym rogu
        //renderMiniMap(shader, labyrinth, activeItems, camera, cubeVAO);


        for (auto& item : activeItems) {
            if (!item.collected) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, item.position);

                if (item.type == ItemType::KEY) {
                    model = glm::scale(model, glm::vec3(0.1f));
                    model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
                    shader.setVec3("objectColor", glm::vec3(0.941f, 0.925f, 0.141f)); // синий ключ
                    shader.setMat4("model", model);

                    glBindVertexArray(keyModel.VAO);
                    glDrawElements(GL_TRIANGLES, keyModel.indexCount, GL_UNSIGNED_INT, 0);
                    glBindVertexArray(0);
                }
                else if (item.type == ItemType::BATTERY) {
                    // kolor baterii (np. czerwony)
                    shader.setVec3("objectColor", glm::vec3(0.8f, 0.1f, 0.1f));
                    // walec – smuklejszy i wyższy
                    model = glm::scale(model, glm::vec3(0.2f, 0.5f, 0.2f));
                    shader.setMat4("model", model);

                    glBindVertexArray(cylinderVAO);
                    glDrawArrays(GL_TRIANGLES, 0, cylinderVertexCount);
                    glBindVertexArray(0);
                }
                // main.cpp -> fragment rysujący TRAP
                // main.cpp -> fragment rysujący TRAP
                else if (item.type == ItemType::TRAP) {
                    shader.setVec3("objectColor", glm::vec3(0.1f, 0.4f, 0.1f));

                    float time = (float)glfwGetTime();
                    float speed = 2.0f;
                    float wave = (sin(time * speed) + 1.0f) / 2.0f;

                    float openAmount = 0.0f;
                    // Synchronizacja z Camera.h (próg 0.75)
                    if (wave > 0.75f) {
                        openAmount = (wave - 0.75f) * 4.0f; // Ruch w dół
                    }

                    glm::mat4 model = glm::mat4(1.0f);
                    // Przesunięcie
                    model = glm::translate(model, glm::vec3(item.position.x, 0.0f - openAmount, item.position.z));

                    // ZMNIEJSZONO: Skala X i Z z 1.0f na 0.7f
                    model = glm::scale(model, glm::vec3(0.7f, 0.02f, 0.7f));

                    shader.setMat4("model", model);

                    glBindVertexArray(cubeVAO);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
                else if (item.type == ItemType::EXIT) {
                    shader.setVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.0f)); // zielony

                    model = glm::scale(model, glm::vec3(0.2f, 2.0f, 1.0f)); // drzwi
                    shader.setMat4("model", model);

                    glBindVertexArray(cubeVAO);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }

            }
        }

/*
        for (auto& item : activeItems) {
            if (!item.collected) {
                float dist = glm::distance(camera.Position, item.position);
                if (dist < 0.5f) { // próg kolizji
                    item.collected = true;
                    if (item.type == ItemType::KEY) {
                        std::cout << "Zebrano klucz!\n";
                    }
                    else if (item.type == ItemType::BATTERY) {
                        std::cout << "Zebrano baterię!\n";
                        lightRange += 2.0f; // np. zwiększ zasięg latarki
                    }
                }
            }
        }*/
        /*     

        for (auto& item : activeItems) {
            if (!item.collected) {
                float dist = glm::distance(camera.Position, item.position);
                if (dist < 0.5f) {
                    if (item.type == ItemType::KEY) {
                        item.collected = true;
                        keysCollected++;
                        std::cout << "Zebrano klucz! (" << keysCollected << "/3)\n";
                    }
                    else if (item.type == ItemType::BATTERY) {
                        item.collected = true;
                        // Logika kumulacji czasu (2 minuty = 120s)
                        if (lightEffectEndTime > currentFrame) {
                            lightEffectEndTime += 45.0f;
                        }
                        else {
                            lightEffectEndTime = currentFrame + 45.0f;
                        }
                    }
                    else if (item.type == ItemType::TRAP && !item.collected) {
                        std::cout << "Pułapka! Straciłeś życie lub zatrzymano ruch.\n";
                        // np. cofnięcie gracza
                        camera.Position -= camera.Front * 0.5f;
                        item.collected = true;
                    }
                    else if (item.type == ItemType::EXIT && !item.collected) {
                        std::cout << "Gratulacje! Ukończyłeś poziom!\n";
                        glfwSetWindowShouldClose(window, true);
                    }
                }
            }
        }
         */
        // Odblokowanie wyjścia po zebraniu 3 kluczy
        if (camera.keysCollected >= 3) {
            for (auto& item : activeItems) {
                if (item.type == ItemType::EXIT && item.collected == true) {
                    item.collected = false; // teraz dostępne
                    std::cout << "Wyjście odblokowane!\n";
                }
            }
        }


        // --- usuń lub zakomentuj ten blok jeśli nie chcesz widocznej kostki ---
/*
        // narysuj małą kostkę reprezentującą latarkę
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f)); // mała
        lampShader.setMat4("model", model);
        lampShader.setVec3("lightColor", lightColor);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        */

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
void processInput(GLFWwindow* window, float& lightRange)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float currentFrame = (float)glfwGetTime();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime, activeItems, lightRange, currentFrame);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime, activeItems, lightRange, currentFrame);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime, activeItems, lightRange, currentFrame);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime, activeItems, lightRange, currentFrame);
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



