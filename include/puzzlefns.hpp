// File Name:        puzzlefns.hpp
// By:               Darian Benam (GitHub: https://github.com/BeardedFish/)
// Date Created:     Thursday, April 30, 2020

#pragma once

#include <string>
#include <vector>

class PuzzleGame // suggest if better name
{
public:
	PuzzleGame(const std::string& title); // contructor
	~PuzzleGame() = default; // destructor

	PuzzleGame(PuzzleGame&) = default; // copy constructor
	PuzzleGame& operator=(PuzzleGame& const) = default; // copy assignment

	PuzzleGame(PuzzleGame&&) noexcept = default; // move constructor
	PuzzleGame& operator=(PuzzleGame&&) noexcept = default; // move destructor

private:
	static const int GRID_SIZE = 4; // The number of columns and rows that the grid will have
	static const int EMPTY_TILE = GRID_SIZE * GRID_SIZE;

	enum class Direction
	{
		Up,
		Down,
		Left,
		Right
	};

	struct board_position_t
	{
		int column, row;
	};

public:

	enum class MoveResult
	{
		InvalidMove,
		ValidMove,
		Win
	};

	/** @brief      Clears the console and prints the program title, the total number of moves the player has performed, and the game board. If an non-empty error message is provided, it is
*               also printed. After the error message is printed, the reference to the string is then assigned an empty string value.
 *  @return     True if the console was cleared, if not, false. */
	bool redrawConsole(std::string& errorMessage, const bool& drawActualValueOfBlankTile);

	/** @brief     Initializes a game board of GRID_SIZE * GRID_SIZE with values from 1 to GRID_SIZE - 1, in numeric order.*/
	void initializeGameBoard();

	/** @brief     Searches the board for the EMPTY_TILE.
	 *  @return    A struct of 'board_position_t' that holds the column and row where the empty tile is. */
	board_position_t getEmptyTilePosition();

	/** @brief     Moves the empty tile on the game board to a specified direction. Whatever tile is in that direction that is touching the empty tile is
	 *             swapped with the empty tile. This function DOES NOT check for bounds so undefined behaviour may occur if you go out of bounds of the game
	 *             board.
	 *  @param     dir The direction that the empty tile should move to. */
	void doMove(const Direction dir);

	/** @brief     Searches for a specified tile value around the empty tile value (up, down, left, or right). If the tile is found, then
	 *             a swap occurs between those two values.
	 *  @param     tileValue The tile value to be swapped with the empty tile.
	 *  @return    If a swap occurred, then MoveResult::ValidMove is returned. If a swap did not occur due to the tile not being near the empty tile, then MoveResult::InvalidMove is returned. If a swap did occur and it resulted in the board being solved, then MoveResult::Win is returned. */
	MoveResult doMove(const int tileValue);

	/** @brief     Gets the opposite direction of the one specified in the parameter of the function.
	 *  @param     dir The direction to get the opposite direction from.
	 *  @return    The direction that is the opposite of the one passed into the parameters. For example, passing in Direction::Left would return Direction::Right. */
	Direction getOppositeMove(const Direction dir);

	/** @brief     Finds all the valid directions that the empty tile can move on the game board.
	 *  @return    A vector of type Direction that contains all the valid moves that can be performed. */
	std::vector<Direction> getValidMoves();

	/** @brief     Shuffles the game board puzzle randomly a specified number of times.
	 *  @param     n The number of times the board should be shuffled randomly. */
	void shufflePuzzle(const unsigned n);

	/** @brief     Clears the console output stream. This function works on either Windows or Linux. The code for clearing the console in Windows was borrowed
	 *             from: https://stackoverflow.com/a/34843392/
	 *  @return    True if the console was cleared. False if it was not due to an error (only applies if the _WIN32 macro is defined). */
	bool clearConsole();

	/** @brief     Determines if the board is solved or not. A solved board is a board that is in numeric order from the top
	 *             left all the way to the bottom right.
	 *  @return    True if the board is solved, if not, false. */
	bool isBoardSolved();

	/** @brief     Prints the game board to the standard output stream.
	 *  @param     drawActualValueOfBlankTile States whether the empty tile integer value should actually be shown or not. If this is false, then the character '_' is printed
	 *                                        for the empty tile. */
	void printBoard(const bool drawActualValueOfBlankTile);

	/** @brief     Prints the horizontal and vertical separtors
	 *  @return    string of separator */
	std::string getHorizontalSeperator();

private:
	std::string programTitle;
	size_t totalMoves;
	std::vector<std::vector<int> > puzzleBoard;
};