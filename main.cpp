#include "raylib.h"

int main() {
    InitWindow(800, 600, "Raylib Window");
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello from Raylib!", 200, 200, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
