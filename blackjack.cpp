#include "raylib.h"      // Raylib graphics library
#include <ctime>         // For random seed
#include "classes.h"     // All game logic classes (Card, Deck, Player, Game)
#include "ui_screen.h"   // All UI screen and helper functions

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

int main() {
    // Initialize window and audio
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blackjack GUI Game");
    InitAudioDevice();
    srand((unsigned)time(0)); // Seed random number generator
    SetTargetFPS(60);         // Limit FPS for smooth rendering

    Game game;                // Create game object (handles all logic and state)
    game.loadAssets();        // Load textures, sounds, and cards

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        // Switch between UI screens based on game state
        switch (game.getState()) {
            case TITLE:   ShowTitle(game);   break;   // Show title/menu screen
            case BETTING: ShowBetting(game); break;   // Show betting screen
            case PLAYING: ShowPlaying(game); break;   // Show main game screen
            case RESULT:  ShowResult(game);  break;   // Show win/lose/result screen
        }
        EndDrawing();
    }

    // Cleanup resources before exit
    game.unloadAssets();
    CloseWindow();
    return 0;
}