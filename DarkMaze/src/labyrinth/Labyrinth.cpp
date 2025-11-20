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
    srand((unsigned)time(0));



    // ROWS i COLS muszą być nieparzyste.

    // Pomocnik: losuje indeks komórki należącej do siatki "komórek"
    // czyli wartości 1,3,5,...,ROWS-2 (gdy ROWS jest nieparzyste).
    auto randCellIndex = [&](int maxSize) -> int {
        int count = (maxSize - 1) / 2; // np. dla 11 -> 5 (daje indeksy 1,3,5,7,9)
        int r = rand() % count;        // 0..count-1
        return 1 + 2 * r;              // 1,3,5,...
        };

    // 2) Wybieramy wewnętrzne komórki startowe (dla generowania) i docelowe
    int startR = randCellIndex(ROWS);
    int startC = randCellIndex(COLS);
    int endR;
    int endC;

    // Wybierz inne losowe miejsce docelowe (może być inny wiersz/kolumna)
    do {
        endR = randCellIndex(ROWS);
        endC = randCellIndex(COLS);
    } while (endR == startR && endC == startC);

    // 3) Standardowy DFS (randomized) na siatce "komórkowej"
    // Zostawiamy jedynie komórki o indeksach 1,3,5,... jako możliwe do odwiedzenia,
    // między nimi będziemy "wybijać" ściany (dr/2, dc/2).
    std::stack<std::pair<int, int>> s;
    maze[startR][startC] = 0; // oznaczamy komórkę jako korytarz
    s.push({ startR, startC });

    int dr[4] = { -2, 2, 0, 0 };
    int dc[4] = { 0, 0, -2, 2 };

    while (!s.empty()) {
        auto current = s.top();
        s.pop();

        std::vector<int> dirs = { 0,1,2,3 };
        // proste losowe przetasowanie
        for (int i = 0; i < 4; ++i) {
            int r = rand() % 4;
            std::swap(dirs[i], dirs[r]);
        }

        for (int k = 0; k < 4; ++k) {
            int d = dirs[k];
            int nr = current.first + dr[d];
            int nc = current.second + dc[d];

            // sprawdź, czy nowa komórka jest wewnątrz planszy i nie odwiedzona
            if (nr > 0 && nr < ROWS && nc > 0 && nc < COLS && maze[nr][nc] == 1) {
                // "wybijamy" ścianę pomiędzy current a (nr,nc)
                int wallR = current.first + dr[d] / 2;
                int wallC = current.second + dc[d] / 2;
                maze[wallR][wallC] = 0;
                maze[nr][nc] = 0;
                s.push({ nr, nc });
            }
        }
    }

    // 4) Teraz wybieramy pozycje wejścia i wyjścia na obrzeżu
    // Zrobimy wejście po lewej krawędzi powiązane z wybraną komórką
    // oraz wyjście po prawej krawędzi powiązane z wybraną komórką.

    // Wybieramy wiersze wejścia/wyjścia odpowiadające komórkom (tych, co wyżej)
    int entranceRow = startR;
    int exitRow = endR;

    // Otwórz połączenie do krawędzi (po lewej i prawej)
    // Po lewej: (entranceRow, 0) oraz (entranceRow, 1) -> 1 = komórka przy krawędzi
    maze[entranceRow][0] = 0;
    maze[entranceRow][1] = 0; // łączymy z wnętrzem

    // Po prawej: (exitRow, COLS-1) oraz (exitRow, COLS-2)
    maze[exitRow][COLS - 1] = 0;
    maze[exitRow][COLS - 2] = 0;

    // 5) Zamknij wszystkie pozostałe krawędzie (upewnij się, że reszta obwodu to ściany)
    for (int r = 0; r < ROWS; ++r) {
        if (r != entranceRow) maze[r][0] = 1;
        if (r != exitRow)     maze[r][COLS - 1] = 1;
    }
    for (int c = 0; c < COLS; ++c) {
        maze[0][c] = 1;
        maze[ROWS - 1][c] = 1;
    }

    // 6) Upewnienie się: jeśli z jakiegoś powodu start/end nie połączyły się z wnętrzem,
    // (to się raczej nie zdarzy bo ich sąsiad (1 lub COLS-2) zostały wcześniej "wyrąbane"),
    // możemy wymusić połączenie (bezpiecznik):
    maze[entranceRow][1] = 0;
    maze[exitRow][COLS - 2] = 0;
}

/*
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
*/
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
