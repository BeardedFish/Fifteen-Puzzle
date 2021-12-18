// File Name:     puzzlefns.hpp
// By:            Darian Benam (GitHub: https://github.com/BeardedFish/)
// Date:          Thursday, April 30, 2020
// Brief:         Header file for function declarations, enum declarations, and constants in order to make the Fifteen Puzzle game work.

#pragma once

#include <vector>

constexpr int GRID_SIZE = 4; // States the number of columns and rows that the grid will have
constexpr int EMPTY_TILE = GRID_SIZE * GRID_SIZE; // States the value of an empty tile on the game board

// An enum that holds all the possible directions the player can move on the game board
enum class Direction
{
	Up,
	Down,
	Left,
	Right
};

// An enum that holds all the possible states that can occur after a move is done in the doMove() function
enum class MoveResult
{
	InvalidMove,
	ValidMove,
	Win
};

// A struct that holds the column and row on the game board
struct board_position_t
{
	int column, row;
};

/* --- BEGIN FUNCTION DECLARATIONS --- */

/**
 * @brief     Initializes a game board of GRID_SIZE * GRID_SIZE with values from 1 to GRID_SIZE - 1, in numeric order.
 * @param     board The game board to be initalized with values.
 */
void initializeGameBoard(int board[GRID_SIZE][GRID_SIZE]);

/**
 * @brief     Searches the board for the EMPTY_TILE.
 * @param     board The game board that will be searched.
 * @return    A struct of 'board_position_t' that holds the column and row where the empty tile is.
 */
board_position_t getEmptyTilePosition(const int board[GRID_SIZE][GRID_SIZE]);

/**
 * @brief     Moves the empty tile on the game board to a specified direction. Whatever tile is in that direction that is touching the empty tile is
 *            swapped with the empty tile. This function DOES NOT check for bounds so undefined behaviour may occur if you go out of bounds of the game
 *            board.
 * @param     board The game board that the move will be performed on.
 * @param     dir The direction that the empty tile should move to.
 */
void doMove(int board[GRID_SIZE][GRID_SIZE], const Direction dir);

/**
 * @brief     Searches for a specified tile value around the empty tile value (up, down, left, or right). If the tile is found, then
 *            a swap occurs between those two values.
 * @param     board The game board that the move will be performed on.
 * @param     tileValue The tile value to be swapped with the empty tile.
 * @return    If a swap occured, then MoveResult::ValidMove is returned. If a swap did not occur due to the tile not being near the empty tile, then MoveResult::InvalidMove
 *            is returned. If a swap did occur and it resulted in the board being solved, then MoveResult::Win is returned.
 */
MoveResult doMove(int board[GRID_SIZE][GRID_SIZE], const int tileValue);

/**
 * @brief     Gets the opposite direction of the one specified in the parameter of the function.
 * @param     dir The direction to get the opposite direction from.
 * @return    The direction that is the opposite of the one passed into the parameters. For example, passing in Direction::Left would return Direction::Right.
 */
Direction getOppositeMove(const Direction dir);

/**
 * @brief     Finds all the valid directions that the empty tile can move on the game board.
 * @param     board The board to be looked at for valid moves.
 * @return    A vector of type Direction that contains all the valid moves that can be performed.
 */
std::vector<Direction> getValidMoves(const int board[GRID_SIZE][GRID_SIZE]);

/**
 * @brief     Shuffles the game board puzzle randomly a specified number of times.
 * @param     board The game board to be shuffled.
 * @param     n The number of times the board should be shuffled randomly.
 */
void shufflePuzzle(int board[GRID_SIZE][GRID_SIZE], const unsigned n);

/**
 * @brief     Clears the console output stream. This function only works on the Windows operating systems since it
 *            uses the windows.h library. The code for this function was borrowed and modified from https://stackoverflow.com/a/34843392/.
 * @return    A boolean of value true if the console was cleared or false if it was not (due to an error).
 */
bool clearConsole();

/**
 * @brief     Determines if the board is solved or not. A solved board is a board that is in numeric order from the top
 *            left all the way to the bottom right.
 * @param     board The game board to be checked on whether it is solved or not.
 * @return    True if the board is solved, if not, false.
 */
bool isBoardSolved(const int board[GRID_SIZE][GRID_SIZE]);

/**
 * @brief     Prints the game board to the standard output stream.
 * @param     board The game board to be printed.
 * @param     drawActualValueOfBlankTile States whether the empty tile integer value should actually be shown or not. If this is false, then the character '_' is printed
 *                                       for the empty tile.
 */
void printBoard(const int board[GRID_SIZE][GRID_SIZE], const bool drawActualValueOfBlankTile);

/* ---  END FUNCTION DECLARATIONS  --- */