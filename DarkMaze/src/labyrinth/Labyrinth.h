#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>
#include "../lighting/Shader.h"

class Labyrinth {
public:
    static const int ROWS = 11;
    static const int COLS = 11;

    int maze[ROWS][COLS];

    Labyrinth();

    void generateMaze();
    void drawLabyrinth(Shader& shader, unsigned int cubeVAO);


private:
    bool isValid(int r, int c);
};
