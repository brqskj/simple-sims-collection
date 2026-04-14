
#include <iostream>
#include <ctime>
#include "raylib.h"

const int fov = 50;
const int larghezza = 30;
int attacco[fov] = {0};
int invaders[fov] = {0};


void funAttacco() {
    // controlli
    for (int i = 0; i < fov; i++) {
        
        if (attacco[i] == invaders[i]) {
            invaders[i] = 0;
            attacco[i] = 0;
        }
    }

    // movimento
    for (int i = 1; i < fov; i++) {
        attacco[i-1] = attacco[i];
    }
}

void input() {
    if (IsKeyPressed(KEY_ONE)) attacco[fov-1] = 1;
    else if (IsKeyPressed(KEY_TWO)) attacco[fov-1] = 2;
    else if (IsKeyPressed(KEY_THREE)) attacco[fov-1] = 3;
    else attacco[fov-1] = 0;

    funAttacco();
}

void muoviInvaders() {

    for (int i = fov-1; i >= 0; i--) {
        invaders[i] = invaders[i-1];
    }
}

void disegna(float unita) {

    Color colore;

    BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < fov; i++) {
            // attacchi
            switch (attacco[i]) {
                case 0: colore = BLACK; break;
                case 1: colore = RED;   break;
                case 2: colore = GREEN; break;
                case 3: colore = BLUE;  break;
            }

            DrawRectangle(larghezza * (attacco[i]-1), unita * i, larghezza, unita, colore);

            // invadaers
            if (!invaders[i]) continue;

            switch (invaders[i]) {
                case 0: colore = BLACK; break;
                case 1: colore = RED;   break;
                case 2: colore = GREEN; break;
                case 3: colore = BLUE;  break;
            }

            DrawRectangle(larghezza * (invaders[i]-1), unita * i, larghezza, unita, colore);
        }
    EndDrawing();
}

int main() {

    srand(time(nullptr));

    InitWindow(0, 0, "RGB defend");
    SetWindowState(FLAG_FULLSCREEN_MODE);
    SetTargetFPS(60);

    float unita = GetScreenHeight() / fov;
    int tick = 0;

    while (!WindowShouldClose()) {

        // funzioni / frame
        input();
        if (!(tick % 30)) funAttacco();
        if (!(tick % 20)) muoviInvaders();
        if (!(tick % 30)) invaders[0] = rand() % 4;
        (tick == 60) ? tick = 0 : tick++;   // 1 tick / frame

        // disegna
        disegna(unita);

        // game over
        if (invaders[fov-1] != 0) return 0;
    }

    CloseWindow();
}

//  versione 1 colonna
/*
const int fov = 50;
int offset = 0;
int attacco[fov] = {0};
int invaders[fov] = {0};


void funAttacco() {

    // controlli
    for (int i = 0; i < fov; i++) {
        
        if (!invaders[i]) continue;
        if (attacco[i] == invaders[i]) invaders[i] = 0;
        attacco[i] = 0;
    }

    // movimento
    for (int i = 1; i < fov; i++) {
        attacco[i-1] = attacco[i];
    }
}

void input() {

    if (IsKeyPressed(KEY_ONE)) attacco[fov-1] = 1;
    else if (IsKeyPressed(KEY_TWO)) attacco[fov-1] = 2;
    else if (IsKeyPressed(KEY_THREE)) attacco[fov-1] = 3;
    else attacco[fov-1] = 0;

    funAttacco();

    if (IsKeyDown(KEY_RIGHT)) offset = 30;
    else if (IsKeyDown(KEY_LEFT)) offset = 0;
}

void muoviInvaders() {

    for (int i = fov-1; i >= 0; i--) {
        invaders[i] = invaders[i-1];
    }
}

void disegna(float unita) {

    Color colore;

    BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < fov; i++) {        
            // attacchi
            switch (attacco[i]) {
                case 0: colore = BLACK; break;
                case 1: colore = RED;   break;
                case 2: colore = GREEN; break;
                case 3: colore = BLUE;  break;
            }

            DrawRectangle(offset, unita * i, 30, unita, colore);

            // invadaers
            if (!invaders[i]) continue;

            switch (invaders[i]) {
                case 0: colore = BLACK; break;
                case 1: colore = RED;   break;
                case 2: colore = GREEN; break;
                case 3: colore = BLUE;  break;
            }

            DrawRectangle(0, unita * i, 30, unita, colore);
        }
    EndDrawing();
}

int main() {

    srand(time(nullptr));

    InitWindow(0, 0, "RGB defend");
    SetWindowState(FLAG_FULLSCREEN_MODE);
    SetTargetFPS(60);

    float unita = GetScreenHeight() / fov;
    int tick = 0;

    while (!WindowShouldClose()) {

        // funzioni x frame
        input();
        if (!(tick % 30)) funAttacco();
        if (!(tick % 20)) muoviInvaders();
        if (!(tick % 30)) invaders[0] = rand() % 4;
        (tick == 60) ? tick = 0 : tick++;   // 1 tick / frame

        // disegna
        disegna(unita);

        // game over
        if (invaders[fov-1] != 0) return 0;
    }

    CloseWindow();
}

*/
