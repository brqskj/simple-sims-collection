
#include "raylib.h"
#include <cmath>
#include <iostream>


Vector2 centro;

class Vettore {
public:
    Vector2 fine;
    float angolo = 0.f;
    float forza = 0.f;

    void cambiaAngolo(int segno) {
        this->angolo += (PI / 180.f) * 2.f * segno;
    }

    void cambiaForza(int segno) {
        this->forza += 2.f * segno;
    }

    void trovaFine() {
        this->fine.x = centro.x + cosf(this->angolo) * this->forza;
        this->fine.y = centro.y + sinf(this->angolo) * this->forza;
    }
};
class Risultante {
public:
    Vector2 fine;

    void trovaFine(Vettore& a, Vettore& b) {
        this->fine.x = a.fine.x + cosf(b.angolo) * b.forza;
        this->fine.y = a.fine.y + sinf(b.angolo) * b.forza;
    }
};

void input(Vettore& a, Vettore& b) {

    if (IsKeyDown(KEY_W)) a.cambiaForza(1);
    if (IsKeyDown(KEY_S)) a.cambiaForza(-1);
    if (IsKeyDown(KEY_A)) a.cambiaAngolo(-1);
    if (IsKeyDown(KEY_D)) a.cambiaAngolo(1);

    if (IsKeyDown(KEY_UP)) b.cambiaForza(1);
    if (IsKeyDown(KEY_DOWN)) b.cambiaForza(-1);
    if (IsKeyDown(KEY_LEFT)) b.cambiaAngolo(-1);
    if (IsKeyDown(KEY_RIGHT)) b.cambiaAngolo(1);
}

void disegna(Vettore& a, Vettore& b, Risultante& tot) {
    
    BeginDrawing();
        ClearBackground(BLACK);

        a.trovaFine();
        b.trovaFine();
        tot.trovaFine(a, b);

        DrawLineEx(centro, a.fine, 1.f, RED);
        DrawLineEx(centro, b.fine, 1.f, RED);
        DrawLineEx(centro, tot.fine, 4.f, BLUE);
        
        DrawCircleV(centro, 15.f, BLUE);
    EndDrawing();
}

int main() {

    InitWindow(0, 0, "divetimento coi vettori");
    SetTargetFPS(30);

    Vettore a;
    Vettore b;
    Risultante tot;
    centro = {GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f};

    while (!WindowShouldClose()) {
        input(a, b);
        disegna(a, b, tot);
    }

    CloseWindow();
}