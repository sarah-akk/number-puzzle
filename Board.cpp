#include <iostream>
#include <ctime>
#include <Windows.h>
#include "Board.h"
#include <vector>
using namespace std;


GameBoard::GameBoard() {
}

GameBoard::~GameBoard() {
}

void GameBoard::generateBoard(vector<int>& initialPositions, GameBoard* board) {

	int index = 0;
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {

			board->board_segments[i][j] = initialPositions[index] ;
			index+=1;
		
		}
	}
			board->board_size = GRIDLENGTH + 1;
			board->col_max = WIDTH;
			board->row_max = HEIGHT;
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GameBoard::randomizeBoard(GameBoard * board) {
		for (unsigned int i = 0; i < GRIDLENGTH; i++) {
        // Generate a random index in the range [0, 7]
        int value = rand() % 8;
		segment_swap(board,i, value);
    }
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GameBoard::segment_swap(GameBoard* board,int i, int j) {
		// Assuming board_segments is a 3x3 array
		if (i >= 0 && i < WIDTH && j >= 0 && j < HEIGHT) {
			// Swap elements (i, j) and (i, i)
			int temp = board->board_segments[i][i];
			board->board_segments[i][i] = board->board_segments[i][j];
			board->board_segments[i][j] = temp;
		}
		else {
			// Handle an out-of-bounds index error if needed
			std::cerr << "Error: Attempted to swap out-of-bounds indices.\n";
		}
	}

