#include "TicTacToe.h"

// -----------------------------------------------------------------------------
// TicTacToe.cpp
// -----------------------------------------------------------------------------
// This file is intentionally *full of comments* and gentle TODOs that guide you
// through wiring up a complete Tic‑Tac‑Toe implementation using the game engine’s
// Bit / BitHolder grid system.
//
// Rules recap:
//  - Two players place X / O on a 3x3 grid.
//  - Players take turns; you can only place into an empty square.
//  - First player to get three-in-a-row (row, column, or diagonal) wins.
//  - If all 9 squares are filled and nobody wins, it’s a draw.
//
// Notes about the provided engine types you'll use here:
//  - Bit              : a visual piece (sprite) that belongs to a Player
//  - BitHolder        : a square on the board that can hold at most one Bit
//  - Player           : the engine’s player object (you can ask who owns a Bit)
//  - Game options     : let the mouse know the grid is 3x3 (rowX, rowY)
//  - Helpers you’ll see used: setNumberOfPlayers, getPlayerAt, startGame, etc.
//
// I’ve already fully implemented PieceForPlayer() for you. Please leave that as‑is.
// The rest of the routines are written as “comment-first” TODOs for you to complete.
// -----------------------------------------------------------------------------

const int AI_PLAYER   = 1;      // index of the AI player (O)
const int HUMAN_PLAYER= 0;      // index of the human player (X)

TicTacToe::TicTacToe()
{
}

TicTacToe::~TicTacToe()
{
}

// -----------------------------------------------------------------------------
// make an X or an O
// -----------------------------------------------------------------------------
// DO NOT CHANGE: This returns a new Bit with the right texture and owner
Bit* TicTacToe::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber == 1 ? "x.png" : "o.png");
    bit->setOwner(getPlayerAt(playerNumber));
    return bit;
}

//
// setup the game board, this is called once at the start of the game
//
void TicTacToe::setUpBoard()
{
    // here we should call setNumberOfPlayers to 2 and then set up the game options so the mouse knows to draw a 3x3 grid
    // _gameOptions has a rowX and rowY property we should set to 3
    // then we need to setup our 3x3 array in _grid with the correct position of the square, and load the "square.png" sprite for each square
    // we will use the initHolder function on each square to do this
    // finally we should call startGame to get everything going

    setNumberOfPlayers(2);
    _gameOptions.rowX = 3;
    _gameOptions.rowY = 3;

    // initialize each square - 150 pixel spacing looks good
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            ImVec2 position(static_cast<float>(x) * 150.0f, static_cast<float>(y) * 150.0f);
            _grid[y][x].initHolder(position, "square.png", x, y);
        }
    }

    startGame();
}

//
// about the only thing we need to actually fill out for tic-tac-toe
//
bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    // 1) Guard clause: if holder is nullptr, fail fast.
    //    (Beginner hint: always check pointers before using them.)
    //    if (!holder) return false;
    if (!holder) return false;

    // 2) Is it actually empty?
    //    Ask the holder for its current Bit using the bit() function.
    //    If there is already a Bit in this holder, return false.
    if (holder->bit()) return false;

    // 3) Place the current player's piece on this holder:
    //    - Figure out whose turn it is (getCurrentPlayer()->playerNumber()).
    //    - Create a Bit via PieceForPlayer(currentPlayerIndex).
    //    - Position it at the holder's position (holder->getPosition()).
    //    - Assign it to the holder: holder->setBit(newBit);

    int currentPlayerIndex = getCurrentPlayer()->playerNumber();
    Bit *newBit = PieceForPlayer(currentPlayerIndex);
    newBit->setPosition(holder->getPosition());
    holder->setBit(newBit);

    // 4) Return whether we actually placed a piece. true = acted, false = ignored.
    return true;
}

bool TicTacToe::canBitMoveFrom(Bit *bit, BitHolder *src)
{
    // you can't move anything in tic tac toe
    return false;
}

bool TicTacToe::canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst)
{
    // you can't move anything in tic tac toe
    return false;
}

//
// free all the memory used by the game on the heap
//
void TicTacToe::stopGame()
{
    // clear out the board
    // loop through the 3x3 array and call destroyBit on each square

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            _grid[y][x].destroyBit();
        }
    }
}

//
// helper function for the winner check
//
Player* TicTacToe::ownerAt(int index ) const
{
    // index is 0..8, convert to x,y using:
    // y = index / 3
    // x = index % 3
    // if there is no bit at that location (in _grid) return nullptr
    // otherwise return the owner of the bit at that location using getOwner()

    int y = index / 3;
    int x = index % 3;
    Bit *bit = const_cast<Bit*>(_grid[y][x].bit()); // need const_cast since getOwner() isn't const
    if (!bit) return nullptr;
    return bit->getOwner();
}

Player* TicTacToe::checkForWinner()
{
    // check all the winning triples
    // if any of them have the same owner return that player
    // winning triples are:
    // 0,1,2
    // 3,4,5
    // 6,7,8
    // 0,3,6
    // 1,4,7
    // 2,5,8
    // 0,4,8
    // 2,4,6
    // you can use the ownerAt helper function to get the owner of a square
    // for example, ownerAt(0) returns the owner of the top-left square
    // if there is no bit in that square, it returns nullptr
    // if you find a winning triple, return the player who owns that triple
    // otherwise return nullptr

    // Hint: Consider using an array to store the winning combinations
    // to avoid repetitive code

    const int winningCombinations[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // columns
        {0, 4, 8}, {2, 4, 6}              // diagonals
    };

    for (int i = 0; i < 8; i++) {
        Player *first = ownerAt(winningCombinations[i][0]);
        Player *second = ownerAt(winningCombinations[i][1]);
        Player *third = ownerAt(winningCombinations[i][2]);

        if (first != nullptr && first == second && second == third) {
            return first;
        }
    }

    return nullptr;
}

bool TicTacToe::checkForDraw()
{
    // is the board full with no winner?
    // if any square is empty, return false
    // otherwise return true

    for (int i = 0; i < 9; i++) {
        if (ownerAt(i) == nullptr) {
            return false;
        }
    }
    return true;
}

//
// state strings
//
std::string TicTacToe::initialStateString()
{
    return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string TicTacToe::stateString() const
{
    // return a string representing the current state of the board
    // the string should be 9 characters long, one for each square
    // each character should be '0' for empty, '1' for player 1 (X), and '2' for player 2 (O)
    // the order should be left-to-right, top-to-bottom
    // for example, the starting state is "000000000"
    // if player 1 has placed an X in the top-left and player 2 an O in the center, the state would be "100020000"
    // you can build the string using a loop and the to_string function
    // for example, to convert an integer to a string, you can use std::to_string(1) which returns "1"
    // you can get the bit at each square using _grid[y][x].bit()
    // if the bit is not null, you can get its owner using bit->getOwner()->playerNumber()
    // remember that player numbers are zero-based, so add 1 to get '1' or '2'
    // if the bit is null, add '0' to the string
    // finally, return the constructed string

    std::string state = "";

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            Bit *bit = const_cast<Bit*>(_grid[y][x].bit());
            if (bit == nullptr) {
                state += '0';
            } else {
                // player numbers are 0,1 but state string uses 1,2
                int playerNumber = bit->getOwner()->playerNumber() + 1;
                state += std::to_string(playerNumber);
            }
        }
    }

    return state;
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void TicTacToe::setStateString(const std::string &s)
{
    // set the state of the board from the given string
    // the string will be 9 characters long, one for each square
    // each character will be '0' for empty, '1' for player 1 (X), and '2' for player 2 (O)
    // the order will be left-to-right, top-to-bottom
    // for example, the starting state is "000000000"
    // if player 1 has placed an X in the top-left and player 2 an O in the center, the state would be "100020000"
    // you can loop through the string and set each square in _grid accordingly
    // for example, if s[0] is '1', you would set _grid[0][0] to have player 1's piece
    // if s[4] is '2', you would set _grid[1][1] to have player 2's piece
    // if s[8] is '0', you would set _grid[2][2] to be empty
    // you can use the PieceForPlayer function to create a new piece for a player
    // remember to convert the character to an integer by subtracting '0'
    // for example, int playerNumber = s[index] - '0';
    // if playerNumber is 0, set the square to empty (nullptr)
    // if playerNumber is 1 or 2, create a piece for that player and set it in the square
    // finally, make sure to position the piece at the holder's position
    // you can get the position of a holder using holder->getPosition()
    // loop through the 3x3 array and set each square accordingly
    // the string should always be valid, so you don't need to check its length or contents
    // but you can assume it will always be 9 characters long and only contain '0', '1', or '2'

    int index = 0;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            int playerNumber = s[index] - '0';
            _grid[y][x].destroyBit();

            if (playerNumber == 1 || playerNumber == 2) {
                // state uses 1,2 but PieceForPlayer expects 0,1
                Bit *bit = PieceForPlayer(playerNumber - 1);
                bit->setPosition(_grid[y][x].getPosition());
                _grid[y][x].setBit(bit);
            }

            index++;
        }
    }
}


//
// negamax - works on a plain int array, not the real board
// currentPlayer is 1 or 2 (matching the state string encoding)
// returns a score from the perspective of currentPlayer
//
int TicTacToe::negamax(int board[9], int currentPlayer)
{
    // check terminal states
    // winning triples
    const int wins[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };

    int opponent = (currentPlayer == 1) ? 2 : 1;

    for (int i = 0; i < 8; i++) {
        int a = board[wins[i][0]];
        int b = board[wins[i][1]];
        int c = board[wins[i][2]];
        if (a != 0 && a == b && b == c) {
            // someone won - if it's the current player return +10, else -10
            return (a == currentPlayer) ? 10 : -10;
        }
    }

    // check for draw - no empty squares left
    bool hasEmpty = false;
    for (int i = 0; i < 9; i++) {
        if (board[i] == 0) { hasEmpty = true; break; }
    }
    if (!hasEmpty) return 0;

    // recurse - try each empty square
    int bestScore = -100;
    for (int i = 0; i < 9; i++) {
        if (board[i] == 0) {
            board[i] = currentPlayer;
            // negamax: negate the score from the opponent's perspective
            int score = -negamax(board, opponent);
            board[i] = 0; // undo
            if (score > bestScore) bestScore = score;
        }
    }

    return bestScore;
}

//
// runs negamax for each empty square and picks the one with the best score
//
int TicTacToe::getBestMove()
{
    // copy the real board into a plain array
    int board[9];
    for (int i = 0; i < 9; i++) {
        Player *owner = ownerAt(i);
        board[i] = owner ? (owner->playerNumber() + 1) : 0;
    }

    // AI is player 1 (state string value = 2)
    int aiValue = AI_PLAYER + 1;

    int bestScore = -100;
    int bestMove = -1;
    for (int i = 0; i < 9; i++) {
        if (board[i] == 0) {
            board[i] = aiValue;
            int score = -negamax(board, (aiValue == 1) ? 2 : 1);
            board[i] = 0;
            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }

    return bestMove;
}

//
// called each frame when it's the AI's turn
//
void TicTacToe::updateAI()
{
    int move = getBestMove();
    if (move == -1) return; // no valid move (shouldn't happen)

    int x = move % 3;
    int y = move / 3;

    BitHolder &holder = _grid[y][x];
    if (actionForEmptyHolder(&holder)) {
        endTurn();
    }
}

