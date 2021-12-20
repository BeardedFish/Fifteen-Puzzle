// File Name:     main.cpp
// By:            Darian Benam (GitHub: https://github.com/BeardedFish/)
// Date:          Thursday, April 30, 2020

#include "../include/puzzlefns.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char* argv[])
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
	std::string errorMsg;
	std::string command;

	do
	{
		if (!clearConsole())
		{
			std::cout << "FATAL ERROR: Unable to clear the console. Terminating program...";

			return EXIT_FAILURE;
		}
	
		std::cout << PROGRAM_TITLE << " | Total Moves: " << totalMoves << '\n';

		printBoard(puzzleBoard, moveResult == MoveResult::Win);

		if (!errorMsg.empty())
		{
			std::cout << "ERROR: " << errorMsg << '\n' << '\n';
			errorMsg.clear();
		}

		std::cout << "Enter a command: ";
		std::getline(std::cin, command);

		if (command != "quit")
		{
			if (command.length() > 0 && std::all_of(command.begin(), command.end(), std::isdigit))
			{
				int tileToSwapWith = stoi(command);
				moveResult = doMove(puzzleBoard, tileToSwapWith);

				if (moveResult == MoveResult::ValidMove || moveResult == MoveResult::Win)
				{
					totalMoves++;

					if (moveResult == MoveResult::Win)
					{
						std::cout << "Congratulations, you won!" << '\n';
					}
				}
				else
				{
					errorMsg = "Invalid move! The value \"" + std::to_string(tileToSwapWith) + "\" is not near the empty tile.";
				}
			}
			else
			{
				errorMsg = "The command " + (command.length() > 0 ? "\"" + command + "\"" : "you have entered") + " is invalid!";
			}
		}
	}
	while (command != "quit" && moveResult != MoveResult::Win);
}
