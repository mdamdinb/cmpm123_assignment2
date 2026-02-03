#pragma once
#include "Game.h"
#include "Square.h"

//
// the classic game of tic tac toe
//

//
// the main game class
//
class TicTacToe : public Game
{
public:
    TicTacToe();
    ~TicTacToe();

    // set up the board
    void        setUpBoard() override;

    Player*     checkForWinner() override;
    bool        checkForDraw() override;
    std::string initialStateString() override;
    std::string stateString() const override;
    void        setStateString(const std::string &s) override;
    bool        actionForEmptyHolder(BitHolder *holder) override;
    bool        canBitMoveFrom(Bit*bit, BitHolder *src) override;
    bool        canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst) override;
    void        stopGame() override;

	void        updateAI() override;
    bool        gameHasAI() override { return _aiEnabled; }
    BitHolder &getHolderAt(const int x, const int y) override { return _grid[y][x]; }

    void        setAIEnabled(bool enabled) { _aiEnabled = enabled; }
    bool        isAIEnabled() const { return _aiEnabled; }

private:
    Bit *       PieceForPlayer(const int playerNumber);
    Player*     ownerAt(int index ) const;

    // negamax search on a plain board array (0=empty, 1=player0, 2=player1)
    int         negamax(int board[9], int currentPlayer);
    // returns the best move index (0-8) for the AI
    int         getBestMove();

    Square      _grid[3][3];
    bool        _aiEnabled = false;
};

