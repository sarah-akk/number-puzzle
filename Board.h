#ifndef BOARD_H
#define BOARD_H

#include <iostream>

class GameBoardSegment {
public:
	unsigned int col;
	unsigned int row;
	unsigned int identity;
	int is_spacer;
	int currently_selected;
};




class GameBoard {
public:
	static const unsigned int WIDTH = 3;
	static const unsigned int HEIGHT = 3;
	static const unsigned int GRIDLENGTH = (WIDTH * HEIGHT) - 1;
	unsigned int board_size;
	unsigned int col_max;
	unsigned int row_max;

	static GameBoardSegment* board_segments;

	GameBoard();
	~GameBoard();

	static void segmentSwap(unsigned int pos1, unsigned int pos2);
	static void generateBoard(GameBoard* board);
	static void randomizeBoard(GameBoard* board);
	//static void renderBoard(GameBoard* board);
	//static void cleanupBoard(GameBoard* board);
};

#endif // !BOARD_H
