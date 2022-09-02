#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <cassert>
#include <algorithm>
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

    int playerX = (int) (it - map.begin()) % width;
    int playerY = (int) (it - map.begin()) / width;

    InitWindow(1280, 720, "tinyraycaster");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground((Color){11, 11, 11, 255});

        drawMap(width, height, map);

        DrawRectangle(playerX * 10 + 2, playerY * 10 + 2, 5, 5, BLUE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
