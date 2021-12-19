// File Name:     main.cpp
// By:            Darian Benam (GitHub: https://github.com/BeardedFish/)
// Date:          Thursday, April 30, 2020
// Brief:         The main C++ file that contains the entry point of the Fifteen Puzzle game.

#include "../include/puzzlefns.hpp"
#include <iostream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

constexpr char PROGRAM_TITLE[] = "Fifteen Puzzle - By: Darian Benam"; // The title that will appear on the window/first line of the console
constexpr size_t SHUFFLE_SEED = 1000; // The number of iterations that the puzzle will be shuffled randomly

/**
 * @brief     The main entry point of the program.
 * @param     argc Argument count, the amount of arguments available that were passed into the program on execution.
 * @param     argv Argument vector, a pointer to a char array that contains the actual arguments passed into the program.
 * @return    An int representing the exit code of the program.
 */
int main(int argc, char* argv[])
{
#ifdef _WIN32
	SetConsoleTitle(PROGRAM_TITLE);
#endif

	// Create and shuffle the game board
	int puzzleBoard[GRID_SIZE][GRID_SIZE];
	initializeGameBoard(puzzleBoard);
	shufflePuzzle(puzzleBoard, SHUFFLE_SEED);

	// Variables that will be modified in the main game loop
	int totalMoves = 0;
	MoveResult moveResult = MoveResult::ValidMove;
	std::string errorMsg;
	std::string userInput;

	while (true) // Main game loop
	{
		if (!clearConsole())
		{
			std::cerr << "FATAL ERROR: Unable to clear the console. Terminating program...";

			return EXIT_FAILURE;
		}
	
		std::cout << PROGRAM_TITLE << " | Total Moves: " << totalMoves << std::endl;

		printBoard(puzzleBoard, moveResult == MoveResult::Win);

		if (moveResult == MoveResult::Win)
		{
			break;
		}

		if (!errorMsg.empty())
		{
			std::cout << "ERROR: " << errorMsg << std::endl << std::endl;

			errorMsg.clear();
		}

		// Get the input from the user and store it in a string
		std::cout << "Enter a Command: ";
		std::cin >> userInput;
		
		if (userInput == "quit") // User wants to quit the program
		{
			break;
		}
		else // User wants to move a tile on the game board
		{
			try
			{
				// Extract the actual integer value from the input string and try and do a move
				int tileValue = stoi(userInput);
				moveResult = doMove(puzzleBoard, tileValue);

				if (moveResult == MoveResult::ValidMove || moveResult == MoveResult::Win)
				{
					totalMoves++;
				}
				else // The enum is InvalidMove
				{
					errorMsg = "Invalid move! The value '" + std::to_string(tileValue) + "' is not near the empty tile.";
				}
			}
			catch (const std::exception&) // User did not enter a valid number
			{
				errorMsg = "Invalid command!";
			}
		}
	} // End while

	if (moveResult == MoveResult::Win)
	{
		std::cout << "Congratulations, you won!" << std::endl;
	}
}