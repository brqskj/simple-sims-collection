
#include "raylib.h"
#include <cmath>


Vector2 center;


auto GetScreenCenter = []() -> Vector2 {
    return {
        (float)GetScreenWidth() * 0.5f, 
        (float)GetScreenHeight() * 0.5f
    };
};

struct Vector {
    Vector2 pos { 50, 50 };

    void pivot(float direction) {
        // sin and cos of angle
        float sin = sinf(direction * DEG2RAD);
        float cos = cosf(direction * DEG2RAD);

        // find offset
        float offsetX = pos.x - center.x;
        float offsetY = pos.y - center.y;

        // pivot
        pos.x = (offsetX * cos - offsetY * sin) + center.x;
        pos.y = (offsetX * sin + offsetY * cos) + center.y;
    }

    void module(float direction) {
        // find offset
        float offsetX = pos.x - center.x;
        float offsetY = pos.y - center.y;

        // find angle and distance
        float angle = atan2f(offsetY, offsetX);
        float distance = sqrtf(offsetX * offsetX + offsetY * offsetY) + direction;

        // module
        pos.x = center.x + cosf(angle) * distance;
        pos.y = center.y + sinf(angle) * distance;
    }

    void draw() {
        DrawLineEx(center, pos, 2.f, RED);
    }
};


void input(Vector& a, Vector& b) {
    float module = 5.f;
    float pivot = 3.f;

    if (IsKeyDown(KEY_W)) a.module(module);
    if (IsKeyDown(KEY_S)) a.module(-module);
    if (IsKeyDown(KEY_D)) a.pivot(pivot);
    if (IsKeyDown(KEY_A)) a.pivot(-pivot);

    if (IsKeyDown(KEY_UP)) b.module(module);
    if (IsKeyDown(KEY_DOWN)) b.module(-module);
    if (IsKeyDown(KEY_RIGHT)) b.pivot(pivot);
    if (IsKeyDown(KEY_LEFT)) b.pivot(-pivot);
}

void draw(Vector& a, Vector& b) {
    BeginDrawing();
    ClearBackground(BLACK);

    a.draw();
    b.draw();
    DrawCircleV(center, 15.f, BLUE);

    EndDrawing();
}

int main() {
    InitWindow(1000, 500, "sum of moving vectors");
    SetTargetFPS(30);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Vector a;
    Vector b;

    while (!WindowShouldClose()) {
        center = GetScreenCenter();
        input(a, b);
        draw(a, b);
    }

    CloseWindow();
    return 0;
}