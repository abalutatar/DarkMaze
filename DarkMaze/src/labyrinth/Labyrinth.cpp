#include "Labyrinth.h"
#include <stack>
#include <cstdlib>
#include <ctime>
#include <iostream>

Labyrinth::Labyrinth() {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            maze[i][j] = 1;
}

bool Labyrinth::isValid(int r, int c) {
    return r >= 0 && r < ROWS && c >= 0 && c < COLS;
}

void Labyrinth::generateMaze(std::vector<Item>& activeItems) {
    srand((unsigned)time(0));

    auto randCellIndex = [&](int maxSize) -> int {
        int count = (maxSize - 1) / 2;
        int r = rand() % count;
        return 1 + 2 * r;
    };

    int startR = randCellIndex(ROWS);
    int startC = randCellIndex(COLS);
    int endR, endC;
    do {
        endR = randCellIndex(ROWS);
        endC = randCellIndex(COLS);
    } while (endR == startR && endC == startC);

    std::stack<std::pair<int, int>> s;
    maze[startR][startC] = 0;
    s.push({ startR, startC });

    int dr[4] = { -2, 2, 0, 0 };
    int dc[4] = { 0, 0, -2, 2 };

    while (!s.empty()) {
        auto current = s.top(); s.pop();
        std::vector<int> dirs = { 0,1,2,3 };
        for (int i = 0; i < 4; ++i) {
            int r = rand() % 4;
            std::swap(dirs[i], dirs[r]);
        }

        for (int k = 0; k < 4; ++k) {
            int d = dirs[k];
            int nr = current.first + dr[d];
            int nc = current.second + dc[d];
            if (nr > 0 && nr < ROWS && nc > 0 && nc < COLS && maze[nr][nc] == 1) {
                int wallR = current.first + dr[d] / 2;
                int wallC = current.second + dc[d] / 2;
                maze[wallR][wallC] = 0;
                maze[nr][nc] = 0;
                s.push({ nr, nc });
            }
        }
    }

    int entranceRow = startR;
    int exitRow = endR;
    //maze[entranceRow][0] = 0;
    //maze[entranceRow][1] = 0;
    maze[exitRow][COLS - 1] = 0;
    maze[exitRow][COLS - 2] = 0;

    // 1. Znajdź wszystkie wolne komórki (korytarze)
    std::vector<std::pair<int, int>> freeCells;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == 0) {
                // Nie stawiaj przedmiotów dokładnie na wejściu (0, entranceRow)
                if (j > 1 && j < COLS - 2) {
                    freeCells.push_back({ i, j });
                }
            }
        }
    }

    // 2. Potasuj wolne komórki, żeby przedmioty były w losowych miejscach
    for (int i = 0; i < freeCells.size(); ++i) {
        int r = rand() % freeCells.size();
        std::swap(freeCells[i], freeCells[r]);
    }

    // 3. Wyczyść stare przedmioty (opcjonalnie) i dodaj nowe
    activeItems.clear();
    int cellIdx = 0;

    // Dodaj 3 KLUCZE
    for (int i = 0; i < 3 && cellIdx < freeCells.size(); ++i) {
        activeItems.push_back(Item(glm::vec3(freeCells[cellIdx].second, 0.5f, freeCells[cellIdx].first), ItemType::KEY));
        cellIdx++;
    }

    // Dodaj 5 BATERII
    for (int i = 0; i < 5 && cellIdx < freeCells.size(); ++i) {
        activeItems.push_back(Item(glm::vec3(freeCells[cellIdx].second, 0.5f, freeCells[cellIdx].first), ItemType::BATTERY));
        cellIdx++;
    }

    // Dodaj 2 PUŁAPKI
    for (int i = 0; i < 2 && cellIdx < freeCells.size(); ++i) {
        activeItems.push_back(Item(glm::vec3(freeCells[cellIdx].second, 0.5f, freeCells[cellIdx].first), ItemType::TRAP));
        cellIdx++;
    }

    // 4. WYJŚCIE (zgodnie z logiką labiryntu na samym końcu po prawej)
    // Wyjście jest tam, gdzie wstawiłeś maze[exitRow][COLS-1] = 0
    activeItems.push_back(Item(glm::vec3(COLS - 1 +0.5f, 0.5f, exitRow),ItemType::EXIT));

    //activeItems.back().collected = true; // Zablokowane na start
}


//Rysowanie labiryntu + podłogi + sufitu
void Labyrinth::drawLabyrinth(Shader& shader, unsigned int cubeVAO) {
    shader.use();

    float cellSize = 1.0f;

    // 1) Rysuj jedną dużą, cienką płytę jako podłogę
    {
        float width = COLS * cellSize;
        float depth = ROWS * cellSize;
        glm::mat4 model = glm::mat4(1.0f);
        // ustaw środek podłogi w środku siatki i nieco poniżej y=0 aby góra kostki leżała na y=0
        model = glm::translate(model, glm::vec3((COLS - 1) * cellSize * 0.5f, -0.01f, (ROWS - 1) * cellSize * 0.5f));
        model = glm::scale(model, glm::vec3(width, 0.02f, depth)); // bardzo cienka w osi Y
        shader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    // 2) Rysuj sufit jako cienką płytę nad ścianami
    {
        float width = COLS * cellSize;
        float depth = ROWS * cellSize;
        // Ściany mają środek y = 0.5 i wysokość 1.0 => ich góra jest na y = 1.0.
        // Umieść środek sufitu nieco powyżej y=1.0, żeby uniknąć z-fightingu.
        float ceilingCenterY = 1.01f;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3((COLS - 1) * cellSize * 0.5f, ceilingCenterY, (ROWS - 1) * cellSize * 0.5f));
        model = glm::scale(model, glm::vec3(width, 0.02f, depth)); // bardzo cienka
        shader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    // 3) Rysuj ściany (tak jak wcześniej)
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == 1) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(j * cellSize, 0.5f, i * cellSize)); // ściana na y=0.5 (wys. 1)
                shader.setMat4("model", model);

                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
            }
        }
    }
}
