// File Name:     puzzlefns.cpp
// By:            Darian Benam (GitHub: https://github.com/BeardedFish/)
// Date:          Thursday, April 30, 2020

#include "../include/puzzlefns.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

void initializeGameBoard(int board[GRID_SIZE][GRID_SIZE])
{
	int gridPositionValue = 1;

	for (int row = 0; row < GRID_SIZE; row++)
	{
		for (int col = 0; col < GRID_SIZE; col++)
		{
			board[row][col] = gridPositionValue++;
		}
	}
}

board_position_t getEmptyTilePosition(const int board[GRID_SIZE][GRID_SIZE])
{
	board_position_t position { -1, -1 };

	for (int row = GRID_SIZE - 1; row >= 0; row--)
	{
		for (int col = GRID_SIZE - 1; col >= 0; col--)
		{
			if (board[row][col] == EMPTY_TILE)
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
	switch (dir)
	{
		case Direction::Left:
			return Direction::Right;
		case Direction::Right:
			return Direction::Left;
		case Direction::Up:
			return Direction::Down;
		case Direction::Down:
		default:
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

	srand(static_cast<unsigned>(time(NULL)));

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
#ifdef _WIN32
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
#endif

#ifdef __linux__
	std::cout << "\033[H\033[2J\033[3J";
#endif

	return true;
}

bool isBoardSolved(const int board[GRID_SIZE][GRID_SIZE])
{
	int gridPositionExpectedValue = 1;

	for (int row = 0; row < GRID_SIZE; row++)
	{
		for (int col = 0; col < GRID_SIZE - (row == GRID_SIZE - 1 ? 1 : 0); col++)
		{
			gridPositionExpectedValue++;

			if (board[row][col] != gridPositionExpectedValue)
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
	std::cout << '\n' << horizontalSeperator << '\n';

	for (int row = 0; row < GRID_SIZE; row++)
	{
		for (int col = 0; col < GRID_SIZE; col++)
		{
			std::cout << (col == 0 ? "| " : "") << std::left << std::setw(3);
			std::cout << ((board[row][col] == EMPTY_TILE && !drawActualValueOfBlankTile) ? "__" : std::to_string(board[row][col])) << "| ";
		}

		std::cout << '\n' << horizontalSeperator << '\n';
	}

	std::cout << '\n';
}
