// File Name:     puzzlefns.cpp
// By:            Darian Benam (GitHub: https://github.com/BeardedFish/)
// Date:          Thursday, April 30, 2020
// Brief:         Equivelent C++ file for the 'puzzlefns.hpp' file. Contains the actual function code for the function headers in 'puzzlefns.hpp'. For function header
//                documentation, refer to 'puzzlefns.hpp'.

#include "../include/puzzlefns.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <windows.h>

void initializeGameBoard(int board[GRID_SIZE][GRID_SIZE])
{
	int gridPositionValue = 1;

	// Initialize all grid squares, excluding the bottom right one, with their actual value
	for (int row = 0; row < GRID_SIZE; row++)
	{
		for (int col = 0; col < GRID_SIZE - (row == GRID_SIZE - 1 ? 1 : 0); col++)
		{
			board[row][col] = gridPositionValue++;
		}
	}

	// Set the last grid square in the board as the empty tile
	board[GRID_SIZE - 1][GRID_SIZE - 1] = EMPTY_TILE;
}

board_position_t getEmptyTilePosition(const int board[GRID_SIZE][GRID_SIZE])
{
	board_position_t position { -1, -1 };

	// Start at [GRID_SIZE - 1, GRID_SIZE - 1] since the empty tile will initially start at that position
	for (int row = GRID_SIZE - 1; row >= 0; row--)
	{
		for (int col = GRID_SIZE - 1; col >= 0; col--)
		{
			if (board[row][col] == EMPTY_TILE) // Found it
			{
				position.column = col;
				position.row = row;

				col = row = -1; // Break out of nested for loop
			}
		}
	}

	return position;
}

void doMove(int board[GRID_SIZE][GRID_SIZE], const Direction dir)
{
	board_position_t blankPosition = getEmptyTilePosition(board);

	int swapCol = blankPosition.column;
	int swapRow = blankPosition.row;

	switch (dir)
	{
		case Direction::Left:
			swapCol--;
			break;
		case Direction::Right:
			swapCol++;
			break;
		case Direction::Up:
			swapRow--;
			break;
		case Direction::Down:
		default:
			swapRow++;
	}

	std::swap(board[blankPosition.row][blankPosition.column], board[swapRow][swapCol]);
}

MoveResult doMove(int board[GRID_SIZE][GRID_SIZE], const int tileValue)
{
	board_position_t blankPosition = getEmptyTilePosition(board);

	if (blankPosition.column > 0 && board[blankPosition.row][blankPosition.column - 1] == tileValue)
	{
		doMove(board, Direction::Left);
	}
	else if (blankPosition.column < GRID_SIZE - 1 && board[blankPosition.row][blankPosition.column + 1] == tileValue)
	{
		doMove(board, Direction::Right);
	}
	else if (blankPosition.row > 0 && board[blankPosition.row - 1][blankPosition.column] == tileValue)
	{
		doMove(board, Direction::Up);
	}
	else if (blankPosition.row < GRID_SIZE - 1 && board[blankPosition.row + 1][blankPosition.column] == tileValue)
	{
		doMove(board, Direction::Down);
	}
	else
	{
		return MoveResult::InvalidMove;
	}

	if (isBoardSolved(board))
	{
		return MoveResult::Win;
	}

	return MoveResult::ValidMove;
}

Direction getOppositeMove(const Direction dir)
{
	if (dir == Direction::Left)
	{
		return Direction::Right;
	}
	else if (dir == Direction::Right)
	{
		return Direction::Left;
	}
	else if (dir == Direction::Up)
	{
		return Direction::Down;
	}
	else // dir equals Down
	{
		return Direction::Up;
	}
}

std::vector<Direction> getValidMoves(const int board[GRID_SIZE][GRID_SIZE])
{
	std::vector<Direction> result;
	board_position_t blankPosition = getEmptyTilePosition(board);

	if (blankPosition.column > 0 && blankPosition.column <= GRID_SIZE - 1)
	{
		result.push_back(Direction::Left);
	}

	if (blankPosition.column < GRID_SIZE - 1 && blankPosition.column >= 0)
	{
		result.push_back(Direction::Right);
	}

	if (blankPosition.row >= 0 && blankPosition.row < GRID_SIZE - 1)
	{
		result.push_back(Direction::Down);
	}

	if (blankPosition.row <= GRID_SIZE - 1 && blankPosition.row > 0)
	{
		result.push_back(Direction::Up);
	}

	return result;
}

void shufflePuzzle(int board[GRID_SIZE][GRID_SIZE], const unsigned n)
{
	Direction previousMove;
	Direction currentMove;

	// Seed the random
	srand((unsigned)time(NULL));

	for (unsigned i = 0; i < n; i++)
	{
		// Get the moves that are legal and remove the opposite direction from the valid moves to prevent the shuffle from going back
		std::vector<Direction> validMoves = getValidMoves(board);
		validMoves.erase(std::remove(validMoves.begin(), validMoves.end(), previousMove), validMoves.end());

		// Randomly shuffle all the valid moves and get the first element as the next move
		std::random_shuffle(validMoves.begin(), validMoves.end());
		currentMove = validMoves[0];

		// Do the move in that direction
		doMove(board, currentMove);
		previousMove = getOppositeMove(currentMove);
	}
}

std::string getHorizontalSeperator()
{
	std::string result = "|";

	for (int i = 0; i < GRID_SIZE; i++)
	{
		result += "====|";
	}

	return result;
}

bool clearConsole()
{
	// Get the Win32 handle representing standard output. This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console. We need to flush that to the console because we're
	// circumventing std::cout entirely; after we clear the console, we don't want stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi))
	{
		return false;
	}

	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD written;

	// Fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default. This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);

	return true;
}

bool isBoardSolved(const int board[GRID_SIZE][GRID_SIZE])
{
	int gridPositionExpectedValue = 1;

	for (int row = 0; row < GRID_SIZE; row++)
	{
		for (int col = 0; col < GRID_SIZE - (row == GRID_SIZE - 1 ? 1 : 0); col++)
		{
			if (board[row][col] != gridPositionExpectedValue++)
			{
				return false;
			}
		}
	}

	return true;
}

void printBoard(const int board[GRID_SIZE][GRID_SIZE], const bool drawActualValueOfBlankTile)
{
	std::string horizontalSeperator = getHorizontalSeperator();
	std::cout << std::endl << horizontalSeperator << std::endl;

	for (int row = 0; row < GRID_SIZE; row++)
	{
		for (int col = 0; col < GRID_SIZE; col++)
		{
			std::cout << (col == 0 ? "| " : "") << std::left << std::setw(3);

			std::string outputValue = (board[row][col] == EMPTY_TILE && !drawActualValueOfBlankTile) ? "__" : std::to_string(board[row][col]);

			std::cout << outputValue << "| ";
		}

		std::cout << std::endl << horizontalSeperator << std::endl;
	}

	std::cout << std::endl;
}