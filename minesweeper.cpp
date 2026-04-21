
#include "raylib.h"
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;


struct Field {
    unsigned int columns = 20;
    unsigned int rows = 10;
    unsigned int minesNum = 10;
    float cellSize;

    vector <vector <bool>> mines;
    vector <vector <bool>> discovered;
    vector <vector <int>> proximity;


    Field() {
        // initialize
        mines.assign(rows, vector <bool>(columns, 0));
        discovered.assign(rows, vector <bool>(columns, 0));
        proximity.assign(rows, vector <int>(columns, 0));

        // populate mines
        unsigned int remaining = minesNum;

        for (size_t y = 0; y < rows; y++) {
            for (size_t x = 0; x < columns; x++) {
                if (remaining > 0 && GetRandomValue(0, 100) < 15) {
                    mines[y][x] = true;
                    remaining--;
                }
                else mines[y][x] = false;
            }
        }        

        // calculate proximity values
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < columns; x++) {

                if (mines[y][x]) continue;

                int count = 0;
                // count neighbours
                for (int offsetY = -1; offsetY <= 1; offsetY++) {
                    for (int offsetX = -1; offsetX <= 1; offsetX++) {
                        int checkY = y + offsetY;
                        int checkX = x + offsetX;

                        // Verifichiamo di non uscire dai bordi del vettore
                        if (checkY >= 0 && checkY < rows && checkX >= 0 && checkX < columns) {
                            if (mines[checkY][checkX]) count++;
                        }
                    }
                }

                proximity[y][x] = count;
            }
        }

        updateSize();
    }

    int discover(Vector2 pos) {
        int x = (int)(pos.x / cellSize);
        int y = (int)(pos.y / cellSize);

        if (y >= 0 && y < rows && x >= 0 && x < columns) {
            discovered[y][x] = true;
            if (mines[y][x]) return -1;     // -1 game over
            else return winCheck();         // 0 safe, 1 won
        }
    }

    bool winCheck() {
        unsigned int count = 0;

        for (size_t i = 0; i < discovered.size(); i++) {
            for (bool cell : discovered[i]) {
                count += cell;
                if (count == columns * rows - minesNum) return true;
            }
        }

        return false;
    }

    void updateSize() {
        cellSize = fmin(
            GetScreenWidth() / columns,
            GetScreenHeight() / rows
        );
    }

    void draw() {
        Color color;

        for (size_t y = 0; y < mines.size(); y++) {
            for (size_t x = 0; x < mines[y].size(); x++) {
                
                Rectangle rec = {
                    x * cellSize,
                    y * cellSize,
                    cellSize, 
                    cellSize
                };

                DrawRectangleLinesEx(rec, 2.f, DARKBLUE);
                if (!discovered[y][x]) continue;

                switch (proximity[y][x]) {
                    case 0: color = GRAY;       break;
                    case 1: color = DARKGREEN;  break;
                    case 2: color = GREEN;      break;
                    case 3: color = YELLOW;     break;
                    case 4: color = ORANGE;     break;
                    case 5: color = RED;        break;
                    case 6: color = PINK;       break;
                    case 7: color = PURPLE;     break;
                    case 8: color = DARKPURPLE; break;
                }

                DrawRectangleRec(rec, color);
                DrawRectangleLinesEx(rec, 2.f, DARKBLUE);
            }
        }
    }
};


void finalDisplay(const int result) {
    while (!GetKeyPressed()) {
        BeginDrawing();
        ClearBackground(BLACK);
        if (result) DrawText("YOU WON", 0, 0, 200.f, RED);
        else DrawText("YOU LOST", 0, 0, 200.f, RED);
        EndDrawing();
    }
}

int main() {
    InitWindow(1000, 500, "minesweeper");
    SetTargetFPS(30);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Field field;

    while (!WindowShouldClose()) {
        // input
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int result = field.discover(GetMousePosition());
            if (result != 0) {
                finalDisplay(result);
                return 0;
            }
        }
        if (IsWindowResized) field.updateSize();

        // draw
        BeginDrawing();
        ClearBackground(BLUE);
        field.draw();
        EndDrawing();
    }

    CloseWindow();
}