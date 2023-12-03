#include <iostream>
#include <ctime>
#include <Windows.h>
#include "Board.h"


GameBoard::GameBoard() {
}

GameBoard::~GameBoard() {
	delete[] board_segments;
}

GameBoardSegment* GameBoard::board_segments = nullptr;

void GameBoard::generateBoard(GameBoard* board) {
	srand(static_cast<unsigned int>(time(0)));

	 board_segments = new GameBoardSegment[GameBoard::GRIDLENGTH];

	for (size_t i = 0; i < WIDTH; i++) {
		for (size_t j = 0; j < HEIGHT; j++) {
			if (i == WIDTH - 1 && j == HEIGHT - 1) {
				GameBoardSegment& current_segment = board_segments[(i * WIDTH) + j];
				current_segment.col = i;
				current_segment.row = j;
				current_segment.identity = 0;
				current_segment.is_spacer = 1;
				current_segment.currently_selected = 0;
			}
			else {
				GameBoardSegment& current_segment = board_segments[(i * WIDTH) + j];
				current_segment.col = i;
				current_segment.row = j;
				current_segment.identity = ((i * WIDTH) + j) + 1;
				current_segment.is_spacer = 0;
				current_segment.currently_selected = 0;

			}
		}
	}

	board->board_size = GRIDLENGTH + 1;
	board->col_max = WIDTH;
	board->row_max = HEIGHT;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameBoard::randomizeBoard(GameBoard * board) {
	for (unsigned int i = 0; i < GRIDLENGTH; i++) {
		int value = rand() % 15;
		segmentSwap(i, value);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void GameBoard::segmentSwap(unsigned int pos1, unsigned int pos2) {

	GameBoardSegment seg_cpy = board_segments[pos1];
	seg_cpy.col = board_segments[pos2].col;
	seg_cpy.row = board_segments[pos2].row;

	board_segments[pos2].col = board_segments[pos1].col;
	board_segments[pos2].row = board_segments[pos1].row;

	board_segments[pos1] = board_segments[pos2];
	board_segments[pos2] = seg_cpy;
}

