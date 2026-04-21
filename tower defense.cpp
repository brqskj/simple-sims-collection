/*
    TODO:
    attacks
    - spawn with input
    - move them

    enemies
    - make a class
    - draw them
    - spawn them
    - move them

    - collisions
*/

#include "raylib.h"
#include <vector>
using namespace std;


struct Attack {
    Vector2 position;
    Vector2 direction;
    float radius;       // hitbox
};

struct Tower {
    Vector2 center;
    const float radius_percentage = 0.1f;
    float radius;
    vector <Attack> attacks;

    void findCenter() {
        center = {
            (float) GetScreenWidth(),
            (float) GetScreenHeight()
        };

        radius = center.x * radius_percentage;
    }

    void draw() { 
        // tower
        DrawCircleLinesV(center, radius, BLUE);

        // attacks
        for (Attack current : attacks) {
            DrawCircleV(current.position, current.radius, RED);
        }
    }
};


void drawScene(Tower& tower) {
    BeginDrawing();
    ClearBackground(BLACK);

    tower.draw();

    EndDrawing();
}

int main() {
    InitWindow(600, 600, "tower defense");
    SetTargetFPS(30);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Tower tower;

    while (!WindowShouldClose()) {
        tower.findCenter();
        drawScene(tower);
    }
    
    CloseWindow();
    return 0;
}