#pragma once                  // Prevents multiple inclusion of this header file
#include "raylib.h"           // Raylib graphics library (for Texture2D, Sound, etc.)
#include <vector>             // STL vector for dynamic arrays (used for hands)
#include <string>             // STL string for card names
#include <map>                // STL map for storing cards in the deck
#include <cstring>            // For C string functions (like strcpy)

using namespace std;

// Enum for the different game states/screens
enum GameState { TITLE, BETTING, PLAYING, RESULT };

// Represents a single playing card
class Card {
public:
    string name;         // Card name (e.g. "A_of_spades")
    Texture2D texture;   // Card image
    int value;           // Blackjack value (2-11)
    Card() : name(""), value(0) {}
    Card(const string& n, const Texture2D& t, int v) : name(n), texture(t), value(v) {}
};

// Represents a deck of cards
class Deck {
    map<string, Card> cards; // All cards in the deck
public:
    // Loads all card textures and values into the deck
    void load() {
        const char *suits[] = {"hearts", "diamonds", "clubs", "spades"};
        const char *ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
        const char *fnames[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "jack", "queen", "king", "ace"};
        int values[] = {2,3,4,5,6,7,8,9,10,10,10,10,11};
        for (int s = 0; s < 4; s++)
            for (int r = 0; r < 13; r++) {
                string name = string(ranks[r]) + "_of_" + suits[s];
                string file = "cards/" + string(fnames[r]) + "_of_" + suits[s] + ".png";
                cards[name] = Card(name, LoadTexture(file.c_str()), values[r]);
            }
    }
    // Draws a random card from the deck
    Card draw() { auto it = cards.begin(); advance(it, rand() % cards.size()); return it->second; }
    // Unloads all card textures from memory
    void unload() { for (auto& e : cards) UnloadTexture(e.second.texture); }
};

// Represents a player or dealer hand
class Player {
    vector<Card> hand; // The cards in the player's hand
public:
    // Clears the hand
    void clear() { hand.clear(); }
    // Adds a card to the hand
    void add(const Card& c) { hand.push_back(c); }
    // Calculates the hand's blackjack score (handles aces)
    int score() const {
        int score = 0, aces = 0;
        for (auto& c : hand) { score += c.value; if (c.value == 11) aces++; }
        while (score > 21 && aces--) score -= 10;
        return score;
    }
    // Returns the hand (for drawing)
    const vector<Card>& getHand() const { return hand; }
};

// Main game logic and state
class Game {
    Deck deck;                  // The deck of cards
    Player player, dealer;      // The player and dealer hands
    int cash = 1000, bet = 0;   // Player's cash and current bet
    char betInput[10] = "";     // Bet input buffer
    bool betInputActive = false, playerStands = false, playerWins = false, showHowTo = false;
    GameState state = TITLE;    // Current game state
    Sound clickSound;           // Button click sound
    Texture2D bgTexture, hiddenCardTexture; // Background and hidden card images
public:
    // Loads all textures and sounds
    void loadAssets() {
        deck.load();
        clickSound = LoadSound("sounds/click_001.ogg");
        bgTexture = LoadTexture("images/PokerScreen.jpg");
        hiddenCardTexture = LoadTexture("images/cardimage.png");
    }
    // Unloads all textures and sounds
    void unloadAssets() {
        deck.unload();
        UnloadSound(clickSound);
        UnloadTexture(bgTexture);
        UnloadTexture(hiddenCardTexture);
        CloseAudioDevice();
    }
    // Resets the game to initial state
    void reset() {
        cash = 1000; bet = 0; strcpy(betInput, "");
        player.clear(); dealer.clear();
        playerStands = playerWins = false; state = BETTING;
    }
    // Deals two cards to player and dealer
    void deal() {
        player.clear(); dealer.clear();
        player.add(deck.draw()); player.add(deck.draw());
        dealer.add(deck.draw()); dealer.add(deck.draw());
        playerStands = false;
    }
    // Resolves the round (dealer draws, checks win/loss)
    void resolve() {
        int ps = player.score(), ds = dealer.score();
        while (ds < 17) { dealer.add(deck.draw()); ds = dealer.score(); }
        playerWins = (ps <= 21 && (ds > 21 || ps > ds));
        if (playerWins) cash += bet * 2;
        state = RESULT;
    }
    // --- Getters and setters for UI ---
    Player& getPlayer() { return player; }
    Player& getDealer() { return dealer; }
    int getCash() const { return cash; }
    int getBet() const { return bet; }
    char* getBetInput() { return betInput; }
    bool& getBetInputActive() { return betInputActive; }
    bool& getPlayerStands() { return playerStands; }
    bool& getPlayerWins() { return playerWins; }
    bool& getShowHowTo() { return showHowTo; }
    GameState& getState() { return state; }
    Sound& getClickSound() { return clickSound; }
    Texture2D& getBgTexture() { return bgTexture; }
    Texture2D& getHiddenCardTexture() { return hiddenCardTexture; }
    void setBet(int b) { bet = b; }
    void setCash(int c) { cash = c; }
    void setState(GameState s) { state = s; }
    Deck& getDeck() { return deck; }
};