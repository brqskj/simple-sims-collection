
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <cmath>


struct Map {
    std::vector <Vector2> points;
    float hitbox = 15.f;

    int hover(Vector2 position) {  

        for (int i = 0; i < points.size(); i++) {
            if (CheckCollisionPointCircle(position, points[i], hitbox)) {
                return i;
            }
        }

        return -1;
    }

    void generate(Vector2 position) {
        points.push_back(position);
    }

    void remove(int& index) {
        points.erase(points.begin() + index);
    }
    
    void move(int& index, Vector2 position) {
        points[index] = position;
    }

    void draw(Vector2 mousePos) {
        if (points.empty()) return;
        Vector2 sum = { 0, 0 };

        for (int i = 0; i < points.size(); i++) {
            for (int j = i + 1; j < points.size(); j++) {
                DrawLineEx(points[i], points[j], 3.f, BLUE);  // connectind lines
                DrawCircleV(Vector2Lerp(points[i], points[j], 0.5f), 5.f, PURPLE);  // middles
            }

            DrawCircleV(points[i], 12.5f, BLUE); // points
            sum += points[i];   // sum
        }

        // hovered point
        int hovered = hover(mousePos);
        if (hovered != -1) DrawCircleV(points[hovered], 20.f, BLUE);

        // median spot
        sum.x /= points.size(), sum.y /= points.size();
        DrawCircleV(sum, 3.f, RED);
        DrawCircleLinesV(sum, 25.f, RED);
    }
};


void draw(Map& map, Texture2D& background) {
    BeginDrawing();
    ClearBackground(BLACK);

    
    // find scale factor
    float scale = fminf(
        (float)GetScreenWidth()/background.width, 
        (float)GetScreenHeight()/background.height
    );

    // calculate position
    Vector2 position = {
        (GetScreenWidth() - (background.width * scale)) / 2,
        (GetScreenHeight() - (background.height * scale)) / 2
    };

    // 3. Disegna
    DrawTextureEx(background, position, 0, scale, GRAY);


    map.draw(GetMousePosition());

    EndDrawing();
}

void input(Map& map, Texture2D& background) {
    
    int index = map.hover(GetMousePosition());

    if (index != -1) {
        while (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            map.move(index, GetMousePosition());
            draw(map, background);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            map.remove(index);
        }
    }

    else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        map.generate(GetMousePosition());
    }
}

int main() {
    InitWindow(800, 600, "simulation");
    SetTargetFPS(30);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Map map;
    Texture2D background = LoadTexture("map.png");  // optinal background

    while (!WindowShouldClose()) {
        input(map, background);
        draw(map, background);
    }

    UnloadTexture(background);
    CloseWindow();
    return 0;
}