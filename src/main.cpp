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
	const std::string PROGRAM_TITLE{ "Fifteen Puzzle - By: Darian Benam" };
	constexpr size_t SHUFFLE_SEED = 1000; // The number of iterations that the puzzle will be shuffled randomly

	PuzzleGame puzzleGame(PROGRAM_TITLE);

#ifdef _WIN32
	SetConsoleTitle(PROGRAM_TITLE.c_str());
#endif

	puzzleGame.initializeGameBoard();
	puzzleGame.shufflePuzzle(SHUFFLE_SEED);

	int totalMoves = 0;
	enum PuzzleGame::MoveResult moveResult = PuzzleGame::MoveResult::ValidMove;
	std::string errorMessage;
	std::string command;

	do
	{
		if (!puzzleGame.redrawConsole(errorMessage, false))
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
				moveResult = puzzleGame.doMove(tileToSwapWith);

				if (moveResult == PuzzleGame::MoveResult::ValidMove || moveResult == PuzzleGame::MoveResult::Win)
				{
					totalMoves++;

					if (moveResult == PuzzleGame::MoveResult::Win)
					{
						if (!puzzleGame.redrawConsole(errorMessage, true))
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
	while (command != "quit" && moveResult != PuzzleGame::MoveResult::Win);
}
