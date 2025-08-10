# Blackjack OOP (C++ with Raylib)

A simple Blackjack game implemented in C++ using Object-Oriented Programming principles and the [Raylib] graphics library. This project features a graphical user interface, card images, and sound effects for an engaging Blackjack experience.

## Features
- Classic Blackjack gameplay (player vs dealer)
- Object-oriented design (Player, Dealer, Deck, Card, etc.)
- Graphical interface using Raylib
- Card images and background graphics
- Sound effects for actions and events
- Easy to extend and modify

## Screenshots
![Poker Screen](images/PokerScreen.jpg)

## Getting Started

### Prerequisites
- C++ compiler (e.g., g++)
- [Raylib library] installed
- Windows or other OS supported by Raylib

### Build Instructions
1. Clone this repository:
   ```sh
   git clone https://github.com/yourusername/blackjack-oop.git
   cd blackjack-oop
   ```
2. Make sure Raylib is installed and available to your compiler.
3. Build the project (example for Windows):
   ```sh
   g++ blackjack.cpp -o blackjack.exe -lraylib -lopengl32 -lgdi32 -lwinmm
   ```
4. Run the game:
   ```sh
   ./blackjack.exe
   ```

## Project Structure
- `blackjack.cpp` - Main game logic and entry point
- `main.cpp` - (if present) Additional logic or alternative entry
- `classes.h` - Class definitions (Player, Dealer, Deck, Card, etc.)
- `ui_screen.h` - UI-related code
- `cards/` - Card images
- `images/` - Backgrounds and other images
- `sounds/` - Sound effects

## Controls
- Use mouse to interact with buttons (Hit, Stand, etc.)
- Follow on-screen instructions

## Credits
- [Raylib](https://www.raylib.com/) for graphics
- Card and sound assets from open sources (see respective folders)

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
