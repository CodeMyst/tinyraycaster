#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <cmath>
#include "raylib.h"

enum TileType {
    Empty,
    Wall,
    Player
};

struct Tile {
    TileType type;
    uint8_t texId = 0;
};

struct Sprite {
    float x;
    float y;
    int texId;
};

Color texIdxToCol(int idx) {
    if (idx == 1) {
        return (Color){208, 208, 208, 255};
    } else if (idx == 2) {
        return (Color){209, 27, 42, 255};
    } else if (idx == 3) {
        return (Color){171, 72, 29, 255};
    } else if (idx == 5) {
        return (Color){41, 99, 20, 255};
    }

    return (Color){0, 0, 0, 255};
}

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
            t.type = TileType::Empty;
        } else if (c == '@') {
            t.type = TileType::Player;
        } else {
            t.type = TileType::Wall;

            if (c == '#') {
                t.texId = 1;
            } else if (c == 'X') {
                t.texId = 2;
            } else if (c == 'V') {
                t.texId = 5;
            } else if (c == '|') {
                t.texId = 3;
            }
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

void drawMap(int width, int height, std::vector<Tile> map, std::vector<Sprite> sprites) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Tile tile = map.at(x + y * width);

            if (tile.type == TileType::Wall) {
                DrawRectangle(x * 10, y * 10, 10, 10, texIdxToCol(tile.texId));
            }
        }
    }

    for (Sprite s : sprites) {
        DrawRectangle(s.x * 10 - 2, s.y * 10 - 2, 5, 5, (Color){204, 20, 20, 255});
    }
}

int main() {
    auto [width, height, map] = loadMap("maps/map01.txt");

    auto it = std::find_if(map.begin(), map.end(), [](const Tile tile) {
        return tile.type == TileType::Player;
    });
    assert(it != map.end());

    float playerX = (float) ((int) (it - map.begin()) % width);
    float playerY = (float) (it - map.begin()) / float(width);

    float playerAngle = 1.5;
    float fov = M_PI / 3.0;

    std::vector<Sprite> sprites{{1.834, 8.765, 0}, {5.323, 1.365, 1}, {4.123, 10.265, 1}};

    InitWindow(1280, 720, "tinyraycaster");
    SetTargetFPS(75);
    SetTraceLogLevel(LOG_NONE);

    Texture2D tex = LoadTexture("assets/walltext.png");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground((Color){11, 11, 11, 255});

        DrawFPS(1280 - 100, 0);

        const int numRays = 320;

        // draw rays on map
        for (size_t i = 0; i < numRays; i++) {
            float angle = playerAngle - fov / 2.0 + fov * i / float(numRays);

            double c = 0;
            float cx = 0;
            float cy = 0;
            while (c < 20) {
                cx = playerX + c * std::cos(angle);
                cy = playerY + c * std::sin(angle);

                int idx = int(cx) + int(cy) * width;

                if (cx < 0 || cx > width ||
                    cy < 0 || cy > height ||
                    idx > map.size() - 1 ||
                    map.at(idx).type == TileType::Wall) {

                    float colHeight = 720.0 / (c * std::cos(angle - playerAngle));
                    float y = 720.0 / 2.0 - colHeight / 2.0;

                    Color col = (Color){0, 0, 0, 255};
                    if (idx < map.size()) {
                        int texIdx = map.at(idx).texId;
                        col = texIdxToCol(texIdx);
                    }

                    float hitX = cx - std::floor(cx + 0.5);
                    float hitY = cy - std::floor(cy + 0.5);
                    int texCoord = hitX * 64;

                    // check if vertical wall
                    if (std::abs(hitY) > std::abs(hitX)) {
                        texCoord = hitY * 64;
                    }

                    // make sure not negative
                    if (texCoord < 0) {
                        texCoord += 64;
                    }

                    assert(texCoord >= 0 && texCoord < 64);

                    DrawTexturePro(tex, (Rectangle){(float) ((64 * map.at(idx).texId) + texCoord), 0, 1, 64},
                        (Rectangle){(float) (i * 4), y, 4, colHeight}, (Vector2){0, 0}, 0, WHITE);

                    break;
                }

                DrawPixel(cx * 10, cy * 10, (Color){130, 130, 130, 255});

                c += 0.01;
            }
        }

        drawMap(width, height, map, sprites);

        // draw player on map
        DrawRectangle(playerX * 10 - 2, playerY * 10 - 2, 5, 5, BLUE);

        const float movSpeed = 0.05;
        const float lookSpeed = 1.5;

        if (IsKeyDown(KEY_A)) playerAngle -= lookSpeed * GetFrameTime();
        else if (IsKeyDown(KEY_D)) playerAngle += lookSpeed * GetFrameTime();

        if (IsKeyDown(KEY_W)) {
            playerX += movSpeed * std::cos(playerAngle);
            playerY += movSpeed * std::sin(playerAngle);
        } else if (IsKeyDown(KEY_S)) {
            playerX -= movSpeed * std::cos(playerAngle);
            playerY -= movSpeed * std::sin(playerAngle);
        }

        EndDrawing();
    }

    UnloadTexture(tex);

    CloseWindow();

    return 0;
}
