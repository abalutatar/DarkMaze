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
void Labyrinth::generateMaze() {
    srand((unsigned)time(0));



    // ROWS i COLS muszą być nieparzyste.

    // losowanie indeksu komórki należącej do siatki "komórek"
    // czyli wartości 1,3,5,...,ROWS-2 (gdy ROWS jest nieparzyste).
    auto randCellIndex = [&](int maxSize) -> int {
        int count = (maxSize - 1) / 2; // np. dla 11 -> 5 (daje indeksy 1,3,5,7,9)
        int r = rand() % count;        // 0..count-1 (0, 1, 2, 3,4)
        return 1 + 2 * r;              // 1,3,5,...,9
        };

    // Wybieramy wewnętrzne komórki startowe (dla generowania) i docelowe
    int startR = randCellIndex(ROWS);
    int startC = randCellIndex(COLS);
    int endR;
    int endC;

    // Wybierz inne losowe miejsce docelowe (może być inny wiersz/kolumna)
    do {
        endR = randCellIndex(ROWS);
        endC = randCellIndex(COLS);
    } while (endR == startR && endC == startC);

    std::cout << startR << "   " << startC << std::endl;

    // Zostawiamy jedynie komórki o indeksach 1,3,5,... jako możliwe do odwiedzenia,
    // między nimi będziemy "wybijać" ściany (dr/2, dc/2).
    std::stack<std::pair<int, int>> s;
    maze[startR][startC] = 0; // oznaczamy komórkę startową jako korytarz
    s.push({ startR, startC });

    //idziemy albo kolumnowo albo wierszowo
    int dr[4] = { -2, 2, 0, 0 };
    int dc[4] = { 0, 0, -2, 2 };

    while (!s.empty()) {
        auto current = s.top();
        s.pop();

        std::vector<int> dirs = { 0,1,2,3 };
        // przetasowanie
        for (int i = 0; i < 4; ++i) {
            int r = rand() % 4;
            std::swap(dirs[i], dirs[r]);
        }

        for (int k = 0; k < 4; ++k) {
            int d = dirs[k];
            int nr = current.first + dr[d];
            int nc = current.second + dc[d];

            // sprawdzamy czy nowa komórka jest wewnątrz planszy i nie jest odwiedzona
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

    //Póki co wybiera na sztywno wejscie w pierwszej kolumnie i wyjście w ostatniej (można dodać losowość)


    // Wybieramy wiersze wejścia/wyjścia odpowiadające wierszom startowym/końcowym
    int entranceRow = startR;
    int exitRow = endR;

    //Wybijamy w pierszej i drugiej kolumnie
    maze[entranceRow][0] = 0;
    maze[entranceRow][1] = 0; // łączymy z wnętrzem

    // //Wybijamy w ostatniej i przedostatniej kolumnie
    maze[exitRow][COLS - 1] = 0;
    maze[exitRow][COLS - 2] = 0;

}

//Rysowanie labiryntu
void Labyrinth::drawLabyrinth(Shader& shader, unsigned int cubeVAO) {
    shader.use();

    float cellSize = 1.0f;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (maze[i][j] == 1) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(j * cellSize, 0.5f, i * cellSize)); //przesunięcie na poz. j,0.5, i
                //model = glm::scale(model, glm::vec3(cellSize, 1.0f, cellSize)); //póki co skalowanie nie jest potrzebne
                shader.setMat4("model", model);

                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
            }
        }
    }
}
