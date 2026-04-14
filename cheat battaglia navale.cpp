
#include "raylib.h"
#include <vector>
#include <algorithm>


// ------ DEFINITIONS ------ //
auto anyMouse = []{ 
    return IsMouseButtonDown(MOUSE_BUTTON_LEFT) || 
           IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || 
           IsMouseButtonDown(MOUSE_BUTTON_MIDDLE); 
};

enum cell {
    UNKNOWN,
    MISS,
    HIT
};

struct Ship {

    unsigned short int length;
    bool sunk = false;

    Ship(const int input) { length = input; }
};

const unsigned short int map_side = 10;
const unsigned short int ship_number = 10;


// ------ GLOBALS ------ //
std::vector <std::vector <cell>> map(map_side, std::vector<cell>(map_side, cell::UNKNOWN));
std::vector <std::vector <int>> heatMap(map_side, std::vector<int>(map_side, 0));
unsigned short int maxValue = 1;


// ------ FUNCTIONS ------ //
void algorithm(std::vector <Ship>& ships) {
    
    // reset heatmap
    for (auto& row : heatMap) {
        for (auto& cell : row) {
            cell = 0;
    } }
    maxValue = 0;


    // for every ship
    for (auto& ship : ships) {
        if (ship.sunk) continue;

        // for every cell
        for (int y = 0; y < map_side; y++) {
            for (int x = 0; x < map_side; x++) {

                // horizontal orientation
                if (x + ship.length <= map_side) {

                    bool valid = true;

                    for (int k = 0; k < ship.length; k++) {
                        if (map[y][x + k] == MISS) {
                            valid = false;
                            break;
                        }
                    }

                    if (valid) {
                        for (int k = 0; k < ship.length; k++) {
                            heatMap[y][x + k]++;
                        }
                    }
                }

                // vertical orientation
                if (y + ship.length <= map_side) {

                    bool valid = true;

                    for (int k = 0; k < ship.length; k++) {
                        if (map[y + k][x] == MISS) {
                            valid = false;
                            break;
                        }
                    }

                    if (valid) {
                        for (int k = 0; k < ship.length; k++) {
                            heatMap[y + k][x]++;
                        }
                    }
                }
            }
        }
    }


    // find biggest number
    for (auto& row : heatMap) {
        for (int x : row) {
            if (x > maxValue) maxValue = x;
    } }
}

void draw(const Vector2 cellLenght) {
    BeginDrawing();
    ClearBackground(BLACK);


    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {

            // calculations
            Rectangle rec = {
                cellLenght.x * x, 
                cellLenght.y * y,
                cellLenght.x,
                cellLenght.y
            };
            Vector2 center = {
                rec.x + (rec.width * 0.5f), 
                rec.y + (rec.height * 0.5f)
            };
            
            // color
            Color color = (map[y][x] == HIT) ? BLUE : BLACK;
            if (map[y][x] == UNKNOWN) {
                unsigned char r = (maxValue > 0) ? (heatMap[y][x] * 255) / maxValue : 0;
                color = {r, 0, 0, 255};
            }

            // draw
            DrawRectangleRec(rec, color);
            if (map[y][x] == MISS) DrawCircleV(center, rec.height * 0.1f, WHITE);
            DrawRectangleLinesEx(rec, 2, WHITE);
        }
    }

    
    EndDrawing(); 
}

void input(const Vector2 cellLenght) {

    // find cell
    Vector2 mouse = GetMousePosition();
    int x = (int)(mouse.x / cellLenght.x);
    int y = (int)(mouse.y / cellLenght.y);

    // bounds check
    if (x < 0 || x >= map_side || y < 0 || y >= map_side) return;

    // state change
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        map[y][x] = HIT;

        // miss on corners
        if (y - 1 >= 0) {
            if (x - 1 >= 0) map[y-1][x-1] = MISS;
            if (x + 1 < map_side) map[y-1][x+1] = MISS;
        }
        if (y + 1 < map_side) {
            if (x - 1 >= 0) map[y+1][x-1] = MISS;
            if (x + 1 < map_side) map[y+1][x+1] = MISS;
        }
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) map[y][x] = MISS;
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) map[y][x] = UNKNOWN;
}

int main() {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(750, 750, "battleship heatmap");
    SetTargetFPS(30);


    std::vector <Ship> ships = {
        Ship(4),

        Ship(3),
        Ship(3),

        Ship(2),
        Ship(2),
        Ship(2),

        Ship(1),
        Ship(1),
        Ship(1),
        Ship(1)
    };
    Vector2 cellLenght = {
        (float)GetScreenWidth() / map_side, 
        (float)GetScreenHeight() / map_side
    };


    while (!WindowShouldClose()) {

        if (IsWindowResized()) cellLenght = {
            (float)GetScreenWidth() / map_side, 
            (float)GetScreenHeight() / map_side
        }; // float cellLenght = std::min(GetScreenWidth(), GetScreenHeight()) / map_side;
        
        draw(cellLenght);

        if (anyMouse()) {
            input(cellLenght);
            algorithm(ships);
        }
    }


    CloseWindow();
    return 0;
}
