
#include <iostream>
#include "raylib.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#define mina 99

const int righe = 12, colonne = 15;
const int num_mine = 10;
int mouse[2];


class Campo {
public:
    int prossimita[righe][colonne];
    int scoperta[righe][colonne];

    Campo() {

        // azzera
        for (int y = 0; y < righe; y++) {
            for (int x = 0; x < colonne; x++) {
                this->scoperta[y][x] = 0;
                this->prossimita[y][x] = 0;
            }
        }

        // genera mine random
        std::srand(std::time(nullptr));

        for (int i = 0; i < num_mine; i++) {
            int x = std::rand() % colonne;
            int y = std::rand() % righe;

            if (this->prossimita[y][x] != mina) {
                this->prossimita[y][x] = mina;
            }
        }

        // per ogni casella
        for (int y = 0; y < righe; y++) {
            for (int x = 0; x < colonne; x++) {

                if (this->prossimita[y][x] == mina) continue;     // salta mine
                int conta = 0;

                // guarda i vicini
                for (int y2 = y-1; y2 <= y+1; y2++) {
                    if (y2 < 0 || y2 == righe) continue;    // check limiti

                    for (int x2 = x-1; x2 <= x+1; x2++) {
                        if (x2 < 0 || x2 == colonne) continue;          // check limiti
                        if (this->prossimita[y2][x2] == mina) conta++;      // conta mine
                    }
                }

                this->prossimita[y][x] = conta;
            }
        }
    }
};

class Schermo {
public:
    float colonna[colonne];
    float riga[righe];
    float cell_height;
    float cell_width;

    Schermo() {
        float screen_height = GetScreenHeight();
        float screen_width = GetScreenWidth();
        cell_height = screen_height / righe;
        cell_width = screen_width / colonne;

        for (int i = 0; i < colonne; i++) {
            this->colonna[i] = cell_width * i;
        }
        for (int i = 0; i < righe; i++) {
            this->riga[i] = cell_height * i;
        }
    }
};

void disegnaCelle(Schermo& schermo, Campo& campo) {

    for (int y = 0; y < righe; y++) {
        for (int x = 0; x < colonne; x++) {
            
            if (campo.scoperta[y][x]) {
                Color colore;
                switch (campo.prossimita[y][x]) {
                    case 0: colore = WHITE;      break;
                    case 1: colore = LIGHTGRAY;  break;
                    case 2: colore = YELLOW;     break;
                    case 3: colore = ORANGE;     break;
                    case 4: colore = PINK;       break;
                    case 5: colore = RED;        break;
                    case 6: colore = MAROON;     break;
                    case 7: colore = PURPLE;     break;
                    case 8: colore = BLACK;      break;
                    case 99: colore = BLACK;     break;  // mina
                }
                
                DrawRectangle(schermo.colonna[x], schermo.riga[y], schermo.cell_width, schermo.cell_height, colore);
            }

            /*if (y == mouse[0] && x == mouse[1]) {     // evidenzia
                DrawRectangle(schermo.colonna[x], schermo.riga[y], schermo.cell_width, schermo.cell_height, BLUE);
            }*/

            DrawRectangleLines(schermo.colonna[x], schermo.riga[y], schermo.cell_width, schermo.cell_height, BLACK);
        }
    }
}

void funzioniMouse(Schermo& schermo, Campo& campo) {

    mouse[0] = GetMouseY() / schermo.cell_height;
    mouse[1] = GetMouseX() / schermo.cell_width;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (!campo.scoperta[mouse[0]][mouse[1]]) {
            campo.scoperta[mouse[0]][mouse[1]] = 1;
        }

        if (campo.prossimita[mouse[0]][mouse[1]] == mina) {
            //ClearBackground(BLACK);
            //DrawText("GAME OVER", GetScreenWidth()/2, GetScreenHeight()/2, 40, RED);
            //Sleep(3000);
            exit(0);
        }
    }
}

int main() {

    InitWindow(0, 0, "Minesweeper by reggere");
    SetWindowState(FLAG_FULLSCREEN_MODE);
    SetTargetFPS(30);

    Schermo schermo;
    Campo campo;

    while (!WindowShouldClose()) {

        BeginDrawing();
            ClearBackground(GREEN);
            disegnaCelle(schermo, campo);
        EndDrawing();

        funzioniMouse(schermo, campo);
    }

    CloseWindow();
}
