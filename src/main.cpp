#include "raylib.h"

int main() {
    InitWindow(1280, 720, "tinyraycaster");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground((Color){11, 11, 11, 255});

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
