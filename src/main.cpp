#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <cassert>
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

int main() {
    auto [width, height, map] = loadMap("maps/map01.txt");

    std::cout << width << ", " << height << std::endl;
    std::cout << map.size() << std::endl;

    InitWindow(1280, 720, "tinyraycaster");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground((Color){11, 11, 11, 255});

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
