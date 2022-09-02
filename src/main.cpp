#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <cmath>
#include "raylib.h"

enum Tile {
    Empty,
    Wall,
    Player
};

std::tuple<int, int, std::vector<Tile>> loadMap(const std::string& path) {
    int width = 0;
    int height = 1;

    std::ifstream file(path);

    assert(file.is_open());

    std::vector<Tile> map;

    char c;
    while(file.get(c)) {
        Tile t;

        if (c == '-') {
            t = Tile::Empty;
        } else if (c == '#') {
            t = Tile::Wall;
        } else if (c == '@') {
            t = Tile::Player;
        }

        if (c == '\n') {
            height++;
        } else {
            map.push_back(t);
        }

        if (height == 1) width++;
    }

    return {width, height, map};
}

void drawMap(int width, int height, std::vector<Tile> map) {
    DrawRectangle(0, 0, (width + 1) * 10, (height + 1) * 10, (Color){28, 28, 28, 255});

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Tile tile = map.at(x + y * width);

            if (tile == Tile::Wall) {
                DrawRectangle(x * 10, y * 10, 10, 10, (Color){204, 204, 204, 255});
            }
        }
    }
}

int main() {
    auto [width, height, map] = loadMap("maps/map01.txt");

    auto it = std::find(map.begin(), map.end(), Tile::Player);
    assert(it != map.end());

    float playerX = (float) ((int) (it - map.begin()) % width);
    float playerY = (float) (it - map.begin()) / float(width);

    float playerAngle = 1.5;
    float fov = 1;

    InitWindow(1280, 720, "tinyraycaster");
    SetTraceLogLevel(LOG_NONE);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground((Color){11, 11, 11, 255});

        SetTargetFPS(75);
        DrawFPS(1280 - 100, 0);

        drawMap(width, height, map);

        // draw player on map
        DrawRectangle(playerX * 10 + 2, playerY * 10 + 2, 5, 5, BLUE);

        // draw rays on map
        for (size_t i = 0; i < 512; i++) {
            float angle = playerAngle - fov / 2 + fov * i / float(512);

            float c = 0;
            float cx = 0;
            float cy = 0;
            while (c < 20) {
                cx = playerX + c * std::cos(angle);
                cy = playerY + c * std::sin(angle);

                if (cx < 0 || cx > width ||
                    cy < 0 || cy > height ||
                    map.at(int(cx) + int(cy) * width) == Tile::Wall) break;

                c += 0.05;
            }

            DrawLine(playerX * 10 + 5, playerY * 10 + 5,
                    cx * 10, cy * 10, RED);
        }

        if (IsKeyDown(KEY_A)) playerAngle -= 0.5f * GetFrameTime();
        else if (IsKeyDown(KEY_D)) playerAngle += 0.5f * GetFrameTime();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
