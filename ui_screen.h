#pragma once
#include "classes.h"
#include "raylib.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// --- UI Helpers ---

// Draws a hand of cards horizontally centered at (cx, y)
inline void DrawHand(const std::vector<Card>& hand, int cx, int y, Texture2D& hiddenCard, bool hideFirst = false) {
    int n = hand.size(), cardW = 120, cardH = 160, spacing = 48;
    int totalW = cardW + (n-1)*spacing, startX = cx - totalW/2;
    for (int i = 0; i < n; i++) {
        int x = startX + i*spacing;
        DrawRectangle(x, y, cardW, cardH, WHITE); // Card background
        if (hideFirst && i == 1)
            // Draw hidden card (for dealer's second card)
            DrawTexturePro(hiddenCard, {0,0,(float)hiddenCard.width,(float)hiddenCard.height},
                {(float)x, (float)y, (float)cardW, (float)cardH}, {0,0}, 0, WHITE);
        else
            // Draw actual card
            DrawTexturePro(hand[i].texture, {0,0,(float)hand[i].texture.width,(float)hand[i].texture.height},
                {(float)x, (float)y, (float)cardW, (float)cardH}, {0,0}, 0, WHITE);
    }
}

// Draws a button and returns true if clicked
inline bool Button(Rectangle rect, const char* label, Color color, int fontSize = 28) {
    DrawRectangleRounded(rect, 0.35f, 16, color);
    int tw = MeasureText(label, fontSize);
    DrawText(label, rect.x + rect.width/2 - tw/2, rect.y + rect.height/2 - fontSize/2, fontSize, WHITE);
    // Check if mouse is over and clicked
    return CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

// --- UI Screens ---

// Shows the "How To Play" popup
inline void ShowHowTo(Game& game, int cx, Color howToCol) {
    Rectangle box = {(float)(cx - 220), 180.0f, 440.0f, 300.0f};
    DrawRectangleRounded(box, 0.2f, 16, Fade(howToCol, 0.95f));
    DrawRectangleLinesEx(box, 4, howToCol);
    int titleFont = 36, font = 22, xPad = 24, y = box.y + 70;
    DrawText("HOW TO PLAY", box.x + box.width/2 - MeasureText("HOW TO PLAY", titleFont)/2, box.y + 18, titleFont, howToCol);
    // Instruction lines
    const char* lines[] = {
        "- Get as close to 21 as possible.",
        "- Dealer draws to 17.",
        "- Place your bet, then Hit or Stand.",
        "- If you bust, you lose your bet."
    };
    for (int i = 0; i < 4; ++i, y += 32) DrawText(lines[i], box.x + xPad, y, font, WHITE);
    // Close button
    Rectangle closeBtn = {box.x + box.width - 120, box.y + box.height - 54, 100, 44};
    DrawRectangleRounded(closeBtn, 0.4f, 8, RED);
    DrawRectangleLinesEx(closeBtn, 3, MAROON);
    DrawText("CLOSE", closeBtn.x + 15, closeBtn.y + 7, 28, WHITE);
    if (Button(closeBtn, "", BLANK, 0)) game.getShowHowTo() = false;
}

// Shows the title screen with Start, How To, and Quit buttons
inline void ShowTitle(Game& game) {
    int cx = 650;
    Color startCol = Color{102,255,178,255}, howToCol = Color{102,178,255,255}, quitCol = Color{255,102,102,255};
    DrawTexturePro(game.getBgTexture(), {0,0,(float)game.getBgTexture().width,(float)game.getBgTexture().height},
        {0,0,(float)SCREEN_WIDTH,(float)SCREEN_HEIGHT}, {0,0}, 0, WHITE);
    // Game title with shadow
    DrawText("BLACKJACK", cx - MeasureText("BLACKJACK", 88)/2 + 3, 90 + 3, 88, BLACK);
    DrawText("BLACKJACK", cx - MeasureText("BLACKJACK", 88)/2, 90, 88, LIME);
    // Button rectangles
    Rectangle startBtn = {cx - 120, 250, 240, 70}, howToBtn = {cx - 120, 340, 240, 70}, quitBtn  = {cx - 120, 430, 240, 70};
    // Draw buttons
    DrawRectangleRounded(startBtn, 0.5f, 16, startCol); DrawRectangleLinesEx(startBtn, 4, DARKGREEN); DrawText("START", startBtn.x + 60, startBtn.y + 18, 40, DARKGREEN);
    DrawRectangleRounded(howToBtn, 0.5f, 16, howToCol); DrawRectangleLinesEx(howToBtn, 4, DARKBLUE); DrawText("HOW TO PLAY", howToBtn.x + 18, howToBtn.y + 18, 36, DARKBLUE);
    DrawRectangleRounded(quitBtn, 0.5f, 16, quitCol); DrawRectangleLinesEx(quitBtn, 4, MAROON); DrawText("QUIT", quitBtn.x + 80, quitBtn.y + 18, 40, MAROON);
    // Button actions
    if (Button(startBtn, "", BLANK, 0)) game.setState(BETTING);
    if (Button(howToBtn, "", BLANK, 0)) game.getShowHowTo() = true;
    if (Button(quitBtn, "", BLANK, 0)) { CloseWindow(); exit(0); }
    // Show how-to popup if needed
    if (game.getShowHowTo()) ShowHowTo(game, cx, howToCol);
}

// Shows the betting screen where the player enters their bet
inline void ShowBetting(Game& game) {
    int cx = 700;
    DrawTexturePro(game.getBgTexture(), {0,0,(float)game.getBgTexture().width,(float)game.getBgTexture().height},
        {0,0,(float)SCREEN_WIDTH,(float)SCREEN_HEIGHT}, {0,0}, 0, WHITE);
    DrawText("PLACE YOUR BET", cx - MeasureText("PLACE YOUR BET", 48)/2, 180, 48, YELLOW);
    DrawText(TextFormat("Cash: $%d", game.getCash()), cx - 60, 250, 36, WHITE);
    // Input and button rectangles
    Rectangle inputBox = {cx - 60, 320, 180, 64}, allInBtn = {inputBox.x + 200, inputBox.y, 120, 64}, confirmBtn = {inputBox.x, inputBox.y + 90, 180, 64};
    // Bet input box
    DrawRectangleRounded(inputBox, 0.4f, 8, game.getBetInputActive() ? SKYBLUE : Color{220,220,220,255});
    DrawRectangleLinesEx(inputBox, 3, DARKBLUE);
    DrawText(game.getBetInput(), inputBox.x + 24, inputBox.y + 18, 36, DARKBLUE);
    DrawText("Bet:", inputBox.x - 90, inputBox.y + 22, 36, DARKBLUE);
    // All-in and confirm buttons
    DrawRectangleRounded(allInBtn, 0.4f, 8, Color{255,204,102,255}); DrawRectangleLinesEx(allInBtn, 3, ORANGE); DrawText("ALL IN", allInBtn.x + 18, allInBtn.y + 18, 32, ORANGE);
    DrawRectangleRounded(confirmBtn, 0.4f, 8, Color{102,255,178,255}); DrawRectangleLinesEx(confirmBtn, 3, DARKGREEN); DrawText("CONFIRM", confirmBtn.x + 24, confirmBtn.y + 18, 32, DARKGREEN);
    // Input handling
    if (CheckCollisionPointRec(GetMousePosition(), inputBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) game.getBetInputActive() = true;
    if (CheckCollisionPointRec(GetMousePosition(), allInBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) snprintf(game.getBetInput(), 10, "%d", game.getCash());
    if (!CheckCollisionPointRec(GetMousePosition(), inputBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        !CheckCollisionPointRec(GetMousePosition(), allInBtn) && !CheckCollisionPointRec(GetMousePosition(), confirmBtn)) game.getBetInputActive() = false;
    // Keyboard input for bet
    if (game.getBetInputActive()) {
        int key = GetCharPressed();
        while (key > 0) {
            int len = (int)strlen(game.getBetInput());
            if ((key >= '0' && key <= '9') && len < 7) { game.getBetInput()[len] = (char)key; game.getBetInput()[len+1] = '\0'; }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            int len = (int)strlen(game.getBetInput());
            if (len > 0) game.getBetInput()[len-1] = '\0';
        }
    }
    // Confirm bet logic
    static bool betError = false;
    if (CheckCollisionPointRec(GetMousePosition(), confirmBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int bet = atoi(game.getBetInput());
        if (bet >= 10 && bet <= game.getCash()) {
            PlaySound(game.getClickSound());
            game.setCash(game.getCash() - bet);
            game.setBet(bet);
            game.deal();
            game.setState(PLAYING);
            betError = false;
        } else betError = true;
    }
    // Show error if bet is invalid
    if (betError) {
        DrawText("Invalid bet!", cx - 60 + 2, 440 + 2, 32, BLACK);
        DrawText("Invalid bet!", cx - 60, 440, 32, RED);
    }
}

// Shows the main playing screen (player and dealer hands, hit/stand)
inline void ShowPlaying(Game& game) {
    int cx = 700;
    DrawTexturePro(game.getBgTexture(), {0,0,(float)game.getBgTexture().width,(float)game.getBgTexture().height},
        {0,0,(float)SCREEN_WIDTH,(float)SCREEN_HEIGHT}, {0,0}, 0, WHITE);
    // Macro for shadowed text
    #define SHADOW(x,y,s,c,f) DrawText(TextFormat s, (x)+2, (y)+2, f, BLACK); DrawText(TextFormat s, x, y, f, c)
    // Show cash, bet, dealer/player hands and scores
    SHADOW(600, 60, ("Cash: %d", game.getCash()), YELLOW, 36);
    SHADOW(600, 110, ("Bet: %d", game.getBet()), WHITE, 36);
    SHADOW(600, 170, ("DEALER"), LIGHTGRAY, 32);
    DrawHand(game.getDealer().getHand(), cx, 220, game.getHiddenCardTexture(), !game.getPlayerStands());
    if (game.getPlayerStands()) SHADOW(cx-60, 390, ("Score: %d", game.getDealer().score()), SKYBLUE, 32);
    SHADOW(600, 420, ("PLAYER"), LIGHTGRAY, 32);
    DrawHand(game.getPlayer().getHand(), cx, 470, game.getHiddenCardTexture());
    SHADOW(cx-60, 640, ("Score: %d", game.getPlayer().score()), SKYBLUE, 32);
    // Hit and Stand buttons
    Rectangle hitBtn = {cx+180, 500, 120, 50}, standBtn = {cx+180, 570, 120, 50};
    if (!game.getPlayerStands()) {
        // Hit: add card to player, check for bust
        if (Button(hitBtn, "Hit", GREEN, 32)) {
            PlaySound(game.getClickSound());
            game.getPlayer().add(game.getDeck().draw());
            if (game.getPlayer().score() > 21) { game.getPlayerStands() = true; game.resolve(); }
        }
        // Stand: end player's turn, resolve dealer
        if (Button(standBtn, "Stand", RED, 32)) { PlaySound(game.getClickSound()); game.getPlayerStands() = true; game.resolve(); }
    }
    #undef SHADOW
}

// Shows the result screen (win/lose, play again or restart)
inline void ShowResult(Game& game) {
    int boxW = 420, boxX = 600, yTitle = 70, yDealerHand = 120, yDealerScore = yDealerHand + 170, yPlayerHand = yDealerScore + 70, yPlayerScore = yPlayerHand + 170, yButton = yPlayerScore + 40;
    DrawTexturePro(game.getBgTexture(), {0,0,(float)game.getBgTexture().width,(float)game.getBgTexture().height},
        {0,0,(float)SCREEN_WIDTH,(float)SCREEN_HEIGHT}, {0,0}, 0, WHITE);
    // Show win/lose message
    const char* resultMsg = game.getPlayerWins() ? "You Win!" : "You Lose!";
    Color resultColor = game.getPlayerWins() ? GREEN : RED;
    DrawText(resultMsg, boxX + boxW/2 - MeasureText(resultMsg, 48)/2 + 2, yTitle + 2, 48, BLACK);
    DrawText(resultMsg, boxX + boxW/2 - MeasureText(resultMsg, 48)/2, yTitle, 48, resultColor);
    // Show dealer and player hands and scores
    DrawHand(game.getDealer().getHand(), boxX + boxW/2, yDealerHand, game.getHiddenCardTexture(), false);
    DrawText(TextFormat("Dealer: %d", game.getDealer().score()), boxX + boxW/2 - 100, yDealerScore, 32, SKYBLUE);
    DrawHand(game.getPlayer().getHand(), boxX + boxW/2, yPlayerHand, game.getHiddenCardTexture(), false);
    DrawText(TextFormat("You: %d", game.getPlayer().score()), boxX + boxW/2 - 60, yPlayerScore, 32, SKYBLUE);
    // Play again or restart buttons
    if (game.getCash() >= 10) {
        Rectangle playAgainBtn = {(float)boxX + boxW/2 - 100, (float)yButton, 200, 54};
        if (Button(playAgainBtn, "Play Again", BLUE, 32)) { strcpy(game.getBetInput(), ""); game.setBet(0); game.setState(BETTING); }
    } else {
        DrawText("GAME OVER", boxX + boxW/2 - MeasureText("GAME OVER", 36)/2 + 2, yButton, 36, BLACK);
        DrawText("GAME OVER", boxX + boxW/2 - MeasureText("GAME OVER", 36)/2, yButton, 36, RED);
        Rectangle restartBtn = {(float)boxX + boxW/2 - 70, (float)yButton + 50, 140, 54};
        if (Button(restartBtn, "RESTART", GREEN, 32)) game.reset();
    }
}