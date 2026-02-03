# CMPM 123 - Assignment 2: Tic-Tac-Toe

**Author:** Miga Damdinbazar
**Platform:** macOS (Darwin 23.3.0)
**Date:** January 29, 2026

## What I Did

Implemented a fully functional two-player Tic-Tac-Toe game using the provided game framework skeleton code. The game supports turn-based gameplay, win detection, draw detection, and proper memory management.

## Features

- Two-player turn-based gameplay
- 3x3 grid with visual squares
- Win detection for all 8 winning combinations (3 rows, 3 columns, 2 diagonals)
- Draw detection when board is full
- Reset button to start a new game
- State serialization for save/load functionality
- Proper memory cleanup on game end

## How It Works

The game uses an object-oriented framework with inheritance. TicTacToe inherits from the base Game class and implements all the required abstract methods.

The skeleton code in TicTacToe.cpp had detailed comments explaining what each method should do, so I followed those instructions to implement the game logic.

### Game Flow

Player 0 goes first and places O pieces. Player 1 goes second and places X pieces. You click an empty square to place your piece, and the game automatically switches to the next player. After each move, the game checks if someone won or if it's a draw.

When someone wins, it shows "winner: 0" or "winner: 1". When it's a draw, it shows "winner: -1".

### Main Implementation

The core of the game is in `setUpBoard()` which creates the grid:

```cpp
void TicTacToe::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 3;
    _gameOptions.rowY = 3;

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            ImVec2 position(static_cast<float>(x) * 150.0f, static_cast<float>(y) * 150.0f);
            _grid[y][x].initHolder(position, "square.png", x, y);
        }
    }

    startGame();
}
```

And `actionForEmptyHolder()` which handles placing pieces:

```cpp
bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    if (!holder) return false;
    if (holder->bit()) return false;

    int currentPlayerIndex = getCurrentPlayer()->playerNumber();
    Bit *newBit = PieceForPlayer(currentPlayerIndex);
    newBit->setPosition(holder->getPosition());
    holder->setBit(newBit);

    return true;
}
```

### Win Detection

I used an array to store all the winning combinations instead of writing 8 separate if statements:

```cpp
const int winningCombinations[8][3] = {
    {0, 1, 2},  // Top row
    {3, 4, 5},  // Middle row
    {6, 7, 8},  // Bottom row
    {0, 3, 6},  // Left column
    {1, 4, 7},  // Middle column
    {2, 5, 8},  // Right column
    {0, 4, 8},  // Diagonal top-left to bottom-right
    {2, 4, 6}   // Diagonal top-right to bottom-left
};
```

Then I loop through and check if all three positions in any combination have the same owner.

## Files I Added/Changed

**Modified:**
- `classes/TicTacToe.cpp` - Implemented all the game logic methods (setUpBoard, actionForEmptyHolder, checkForWinner, checkForDraw, stateString, setStateString, stopGame, and the ownerAt helper)
- `Application.cpp` - Fixed the winner detection logic in the EndOfTurn() method (line 69)

**No changes to:**
- TicTacToe.h
- Any of the game framework classes
- Any other existing files

## Design Choices

**Grid Positioning:** I used 150 pixels between squares. ImGui works with pixel coordinates, so this gives good visual separation.

**Helper Function:** I made an `ownerAt(index)` helper that converts an index from 0-8 into x,y coordinates and returns who owns that square. This made the win checking code much cleaner.

**State String Format:** Used a simple 9-character string where '0' means empty, '1' means Player 0's piece, and '2' means Player 1's piece. The order is left-to-right, top-to-bottom.

**Memory Management:** Used the framework's `destroyBit()` method instead of manually deleting. The framework has a reference counting system built in.

**const_cast:** Had to use `const_cast` in a couple places because the framework's `getOwner()` method isn't marked const but I was calling it from const methods. Not ideal but necessary given the framework's API.

## Build Instructions (macOS)

You need GLFW and CMake:
```bash
brew install glfw cmake
```

Then build:
```bash
mkdir build
cd build
cmake ..
make
./demo
```

The game window opens automatically.

## Problems I Ran Into

**Problem 1: Grid squares all stacked on top of each other**

When I first ran the game, I only saw one blue square. When I clicked it, both X and O appeared on top of each other. The player stayed on Player 0 and never switched.

I figured out the issue was that all 9 squares were at the same position because my spacing was way too small. I initially used:

```cpp
ImVec2 position(static_cast<float>(x) * 1.1f, static_cast<float>(y) * 1.1f);
```

But 1.1 pixels is basically 0, so everything was at position (0,0). Changed it to 150 pixels and the grid appeared properly:

```cpp
ImVec2 position(static_cast<float>(x) * 150.0f, static_cast<float>(y) * 150.0f);
```

**Problem 2: Wrong winner when winning on the last move**

When I filled the last square and won at the same time, the game showed "winner: -1" instead of showing me as the winner. Any other win worked fine, just not when it happened on move 9.

I traced through the code and found the bug in `Application.cpp`. The `EndOfTurn()` function was checking for a draw even after it already found a winner:

```cpp
if (winner) {
    gameOver = true;
    gameWinner = winner->playerNumber();
}
if (game->checkForDraw()) {
    gameOver = true;
    gameWinner = -1;  // This overwrites the winner!
}
```

The draw check was running every time and overwriting the winner variable. Fixed it by adding `else`:

```cpp
else if (game->checkForDraw()) {
    gameOver = true;
    gameWinner = -1;
}
```

Now it only checks for a draw if there's no winner.

**Problem 3: Compiler errors with const methods**

Got compiler errors when trying to call `getOwner()` from my const methods:

```
error: 'this' argument to member function 'getOwner' has type 'const Bit',
but function is not marked const
```

The framework's `Bit::getOwner()` method isn't const-qualified, but I was calling it from `ownerAt()` and `stateString()` which are const methods. I couldn't modify the framework classes, so I had to use `const_cast`:

```cpp
Bit *bit = const_cast<Bit*>(_grid[y][x].bit());
```

Not the cleanest solution but it works and doesn't actually modify anything.

## Implementation Details

**setUpBoard()** - Sets up 2 players, configures the grid to 3x3, loops through and initializes each square with its position and sprite, then calls startGame().

**actionForEmptyHolder()** - Checks if the holder exists and is empty, gets the current player number, creates a piece for that player, positions it, and places it in the holder.

**ownerAt()** - Helper function that takes an index (0-8) and converts it to x,y coordinates, then returns who owns that square (or nullptr if empty).

**checkForWinner()** - Defines all 8 winning combinations in an array, loops through each one checking if all three squares have the same owner, returns the winner or nullptr.

**checkForDraw()** - Loops through all 9 squares and returns true if they're all occupied (meaning it's a draw).

**stateString()** - Builds a 9-character string representing the board state. '0' for empty, '1' for Player 0, '2' for Player 1.

**setStateString()** - Takes a 9-character string and rebuilds the board from it. Destroys any existing pieces first, then creates new ones based on the string.

**stopGame()** - Loops through all squares and calls destroyBit() to clean up memory.

## Testing

I tested all the different ways to win:
- All 3 horizontal rows
- All 3 vertical columns
- Both diagonals
- Draw when board fills up with no winner
- Winning on the last move (this was the bug that showed winner: -1)
- Reset button to start a new game
- Making sure players alternate correctly
- Trying to click on occupied squares (should do nothing)

Everything works correctly now.

## AI - Negamax

I added an AI opponent using the negamax algorithm. You can switch between 2 player mode and vs AI mode using the button in the Settings panel. The AI always plays as Player 1 (X), so you play as Player 0 (O) and go first.

### How Negamax Works

Negamax is a version of minimax that's a bit simpler to write. The idea is that in a two player zero-sum game, one player's gain is the other player's loss. So instead of having separate max and min functions, you just have one function that always maximizes from the current player's perspective, and negate the score when you recurse into the opponent's turn.

The function tries every possible move, recurses to see what happens, then undoes the move and tries the next one. At the bottom it scores the board: +10 if the current player wins, -10 if they lose, 0 for a draw.

```cpp
int score = -negamax(board, opponent);
```

The negation flips the score so that the opponent's win (which would be +10 from their perspective) becomes -10 from ours.

### Why It Works for Tic-Tac-Toe

Tic-tac-toe has a small game tree - at most 9! = 362880 possible games, and way fewer in practice because games end early when someone wins. So negamax can search all the way to the end without needing alpha-beta pruning. It just brute forces every possible game and picks the best move.

### Implementation Details

The negamax search doesn't touch the real game board at all. It works on a plain `int board[9]` array where 0 = empty, 1 = Player 0, 2 = Player 1. This keeps it fast and avoids any issues with creating/destroying Bit sprites during the search.

`getBestMove()` copies the real board into that array, runs negamax for each empty square, and returns the index of the best move. Then `updateAI()` takes that index, converts it to x,y coordinates, and places the piece on the real grid.

The AI hook was already partially set up in the framework - there was commented out code in `Game.cpp::scanForMouse()` that checks if it's the AI's turn and calls `updateAI()`. I just uncommented that and implemented the rest.

### Bug I Ran Into

The `setAIPlayer()` call has to come *after* `setUpBoard()`. The reason is that `setUpBoard()` calls `setNumberOfPlayers()` which recreates all the Player objects from scratch, wiping out the AI flag. I figured this out when the AI just wasn't responding at all after switching modes.

## Files I Added/Changed (Assignment 2)

**Modified:**
- `classes/TicTacToe.h` - Added negamax(), getBestMove() declarations, _aiEnabled flag, setAIEnabled/isAIEnabled methods. Changed gameHasAI() to return _aiEnabled instead of always true.
- `classes/TicTacToe.cpp` - Implemented negamax(), getBestMove(), and updateAI()
- `classes/Game.cpp` - Uncommented the AI check in scanForMouse()
- `Application.cpp` - Added aiMode toggle button, wired up setAIEnabled and setAIPlayer on mode switch and reset

## Time Spent

About 5 hours for the core game logic, plus another 2-3 hours for the AI. Most of the AI time was figuring out the setAIPlayer ordering bug.

## References

- The skeleton code comments in TicTacToe.cpp were very detailed and helpful
- Looked at the Game.h, BitHolder.h, and Square.h headers to understand the framework's API
- Wikipedia negamax article for the algorithm: https://en.wikipedia.org/wiki/Negamax

## GitHub

https://github.com/mdamdinb/cmpm123_assignment2
