/*
    DA FARE:
    - mutazioni onello spread
    - spread dei colori fatto bene
*/
/*
    https://www.youtube.com/@KatakombStudios
*/

#include "raylib.h"
#include <vector>


constexpr Vector2 NUM = {10.f, 10.f};

class Campo {
public:

    Vector2 schermo;                            // dimensioni schermo
    Vector2 unita;                              // dimensioni unita
    std::vector <int> x_colonne;                // array posizioni iniziali colonne
    std::vector <int> y_righe;                  // array posizioni righe righe
    std::vector <std::vector <int>> caselle;    // griglia di gioco
    const Color colore[8] = {                   // lookup table per disegnare la griglia
        {0, 0, 0, 255},
        {255, 0, 0, 255},       // rosso
        {255, 128, 0, 255},     // rancione
        {255, 255, 0, 255},     // giallo
        {0, 165, 0, 255},       // verde
        {0, 0, 255, 255},       // blu
        {140, 0, 255, 255},     // viola
        {255, 255, 255, 255}    // bianco
    };

    Campo() {
        // dim totale e dim unita
        this->schermo.x = GetScreenWidth();
        this->schermo.y = GetScreenHeight();
        this->unita.x = this->schermo.x / NUM.x;
        this->unita.y = this->schermo.y / NUM.y;

        // array posizioni
        this->x_colonne.resize(NUM.x);
        this->y_righe.resize(NUM.y);

        for (size_t i = 0; i < this->x_colonne.size(); i++) {
            this->x_colonne[i] = this->unita.x * i;
        }
        for (size_t i = 0; i < this->y_righe.size(); i++) {
            this->y_righe[i] = this->unita.y * i;
        }

        // caselle
        this->caselle.resize(NUM.y);

        for (size_t i = 0; i < this->caselle.size(); i++) {
            this->caselle[i].resize(NUM.x);
        }
    }

    void ammazza(Vector2 indice) {
        this->caselle[indice.y][indice.x] = 0;
    }

    void cambia(Vector2 indice) {
        this->caselle[indice.y][indice.x]++;
        caselle[indice.y][indice.x] %= sizeof(this->colore) / sizeof(this->colore[0]);
    }
};


void Input(Campo& campo, Vector2& pos, float& aspettaSec) {

    pos = {GetMouseX() / campo.unita.x, GetMouseY() / campo.unita.y};

    // tasto sinistro
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) campo.ammazza(pos);

    // tasto destro
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) campo.cambia(pos);

    // scroll
    aspettaSec += GetMouseWheelMove() * 0.5f;
    if (aspettaSec <= 0) aspettaSec = 0.5f;
    if (aspettaSec >= 10) aspettaSec = 10.f;
}

void diffusione(Campo& campo) {

    // lookup table per quelli da diffondere
    bool stato[(int)NUM.y][(int)NUM.x];

    for (int y = 0; y < campo.caselle.size(); y++) {
        for (int x = 0; x < campo.caselle[y].size(); x++) {
            stato[y][x] = (campo.caselle[y][x] > 0);
        }
    }


    // per ogni casella
    for (int y = 0; y < campo.caselle.size(); y++) {
        for (int x = 0; x < campo.caselle[y].size(); x++) {

            // guarda le vicine
            for (int h = (y-1); h <= (y+1); h++) {
                for (int k = (x-1); k <= (x+1); k++) {
                    
                    if (h < 0 || h >= campo.caselle.size() || k < 0 || k >= campo.caselle[y].size()) continue;  // limiti
                    if (!campo.caselle[h][k] && stato[y][x]) campo.caselle[h][k] = campo.caselle[y][x];
                }
            }
        }
    }
}

void disegna(Campo& campo, Vector2& pos) {

    ClearBackground(BLACK);

    for (size_t y = 0; y < campo.y_righe.size(); y++) {
        for (size_t x = 0; x < campo.x_colonne.size(); x++) {

            Rectangle rec = {
                static_cast<float>(campo.x_colonne[x]), 
                static_cast<float>(campo.y_righe[y]), 
                static_cast<float>(campo.unita.x), 
                static_cast<float>(campo.unita.y)
            };

            DrawRectangleRec(rec, campo.colore[campo.caselle[y][x]]);
            DrawRectangleLinesEx(rec, 2.0f, BLACK);

            if (pos.y == y && pos.x == x) DrawRectangleLinesEx(rec, 10.0f, (Color){200, 200, 200, 255});
        }
    }
}

int main() {

    InitWindow(0, 0, "campo fiorito");
    SetTargetFPS(30);

    Campo campo;
    Vector2 pos;
    float aspettaSec = 3.5f;
    float count = 0.f;

    while (!WindowShouldClose()) {

        BeginDrawing();
            disegna(campo, pos);
            DrawText(TextFormat("speed: %f" ,aspettaSec), 100, 100, 20, WHITE);
        EndDrawing();

        Input(campo, pos, aspettaSec);

        if (count >= GetFPS() * aspettaSec) {
            diffusione(campo);
            count = 0.f;
        }
        count += 1.f;
    }
    CloseWindow();
}
