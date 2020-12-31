/*
\file:     main.cpp
\author:   Darian Benam
\date:     Thursday, April 30, 2020
\version:  1.0
\brief:    The main C++ file that contains the entry point of the Fifteen Puzzle game.
*/

#include "../inc/puzzlefns.hpp"
#include <iostream>
#include <string>
#include <windows.h>

constexpr char PROGRAM_TITLE[] = "Fifteen Puzzle"; // The title that will appear on the window/first line of the console
constexpr size_t SHUFFLE_SEED = 1000; // The number of iterations that the puzzle will be shuffled randomly

/*
\fn:        main()
\brief:     The main entry point of the program.
\param:     argc - Argument count, the amount of arguments available that were passed into the program on execution.
\           argv - Argument vector, a pointer to a char array that contains the actual arguments passed into the program.
\return:    An int representing the exit code of the program.
*/
int main(int argc, char* argv[])
{
	SetConsoleTitle(PROGRAM_TITLE);

	// Create and shuffle the game board
	int puzzleBoard[GRID_SIZE][GRID_SIZE];
	initalizeGameBoard(puzzleBoard);
	shufflePuzzle(puzzleBoard, SHUFFLE_SEED);

	// Variables that will be modified in the main game loop
	int totalMoves = 0;
	MoveResult moveResult = ValidMove;
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

		printBoard(puzzleBoard, moveResult == Win);

		if (moveResult == Win)
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

				if (moveResult == ValidMove || moveResult == Win)
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

	std::cout << "Congratulations, you won!" << std::endl;
}