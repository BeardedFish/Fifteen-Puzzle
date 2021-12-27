// File Name:        main.cpp
// By:               Darian Benam (GitHub: https://github.com/BeardedFish/)
// Date Created:     Thursday, April 30, 2020

#include "../include/puzzlefns.hpp"
#include <iostream>
#include <string>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

int main()
{
	constexpr char PROGRAM_TITLE[] = "Fifteen Puzzle - By: Darian Benam";
	constexpr size_t SHUFFLE_SEED = 1000; // The number of iterations that the puzzle will be shuffled randomly

#ifdef _WIN32
	SetConsoleTitle(PROGRAM_TITLE);
#endif

	int puzzleBoard[GRID_SIZE][GRID_SIZE];
	initializeGameBoard(puzzleBoard);
	shufflePuzzle(puzzleBoard, SHUFFLE_SEED);

	int totalMoves = 0;
	MoveResult moveResult = MoveResult::ValidMove;
	std::string errorMessage;
	std::string command;

	do
	{
		if (!redrawConsole(PROGRAM_TITLE, puzzleBoard, totalMoves, errorMessage, false))
		{
			return EXIT_FAILURE;
		}

		std::cout << "Enter a command: ";
		std::getline(std::cin, command);

		if (command != "quit")
		{
			try
			{
				int tileToSwapWith = stoi(command);
				moveResult = doMove(puzzleBoard, tileToSwapWith);

				if (moveResult == MoveResult::ValidMove || moveResult == MoveResult::Win)
				{
					totalMoves++;

					if (moveResult == MoveResult::Win)
					{
						if (!redrawConsole(PROGRAM_TITLE, puzzleBoard, totalMoves, errorMessage, true))
						{
							return EXIT_FAILURE;
						}

						std::cout << "Congratulations, you won!" << '\n';
					}
				}
				else
				{
					errorMessage = "Invalid move! The value \"" + std::to_string(tileToSwapWith) + "\" is not near the empty tile.";
				}
			}
			catch (const std::exception&)
			{
				errorMessage = "The command " + (command.length() > 0 ? "\"" + command + "\"" : "you have entered") + " is invalid!";
			}
		}
	}
	while (command != "quit" && moveResult != MoveResult::Win);
}
