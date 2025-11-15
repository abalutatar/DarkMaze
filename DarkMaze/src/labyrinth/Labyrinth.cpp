#include "Labyrinth.h"
#include <stack>
#include <cstdlib>
#include <ctime>

Labyrinth::Labyrinth() {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            maze[i][j] = 1;
}

bool Labyrinth::isValid(int r, int c) {
    return r >= 0 && r < ROWS && c >= 0 && c < COLS;
}

void Labyrinth::generateMaze() {
    srand(time(0));
    std::stack<std::pair<int, int>> s;
    int startRow = 0, startCol = 0;
    maze[startRow][startCol] = 0;
    s.push({ startRow, startCol });

    int dr[4] = { -2, 2, 0, 0 };
    int dc[4] = { 0, 0, -2, 2 };

    while (!s.empty()) {
        auto current = s.top();
        s.pop();

        std::vector<int> directions = { 0,1,2,3 };
        for (int i = 0; i < 4; i++) {
            int r = rand() % 4;
            std::swap(directions[i], directions[r]);
        }

        for (int i = 0; i < 4; i++) {
            int nr = current.first + dr[directions[i]];
            int nc = current.second + dc[directions[i]];

            if (isValid(nr, nc) && maze[nr][nc] == 1) {
                int wallRow = current.first + dr[directions[i]] / 2;
                int wallCol = current.second + dc[directions[i]] / 2;
                maze[wallRow][wallCol] = 0;
                maze[nr][nc] = 0;
                s.push({ nr, nc });
            }
        }
    }
}

// cubeVAO: VAO sześcianu zdefiniowany w main.cpp
void Labyrinth::drawLabyrinth(Shader& shader, unsigned int cubeVAO) {
    shader.use();

    float cellSize = 1.0f;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == 1) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(j * cellSize, 0.5f, i * cellSize));
                model = glm::scale(model, glm::vec3(cellSize, 1.0f, cellSize));
                shader.setMat4("model", model);

                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
            }
        }
    }
}
