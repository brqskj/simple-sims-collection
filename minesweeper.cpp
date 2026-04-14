/*
    TODO:
    - add game over and victory
    - add canvas size selection by input
*/

#include "raylib.h"
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;


struct Field {
    unsigned int columns = 20;
    unsigned int rows = columns * 0.5f;
    float minesPercent = 10.f;
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
        unsigned int maxMines = columns * rows * (minesPercent * 0.01f);

        for (size_t y = 0; y < rows; y++) {
            for (size_t x = 0; x < columns; x++) {
                if (maxMines > 0 && GetRandomValue(0, 100) < 15) {
                    mines[y][x] = true;
                    maxMines--;
                }
                else mines[y][x] = false;
            }
        }        


        // calculate near mines
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


    void discover(Vector2 pos) {
        int x = (int)(pos.x / cellSize);
        int y = (int)(pos.y / cellSize);

        if (y >= 0 && y < rows && x >= 0 && x < columns) {
            discovered[y][x] = true;
        }
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


void draw(Field& field) {
    BeginDrawing();
    ClearBackground(BLUE);
    
    field.draw();
    
    EndDrawing();
}

int main() {
    InitWindow(1000, 500, "minesweeper");
    SetTargetFPS(30);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Field field;

    while (!WindowShouldClose()) {
        draw(field);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) field.discover(GetMousePosition());
        if (IsWindowResized) field.updateSize();
    }

    CloseWindow();
}