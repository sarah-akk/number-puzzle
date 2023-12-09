#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <iostream>
using namespace std;


class GameBoard {
public:
	static const unsigned int WIDTH = 3;
	static const unsigned int HEIGHT = 3;
	static const unsigned int GRIDLENGTH = (WIDTH * HEIGHT) ;
	unsigned int board_size;
	unsigned int col_max;
	unsigned int row_max;

	unsigned int  board_segments[WIDTH][HEIGHT];

	GameBoard();
	~GameBoard();

	static void generateBoard( vector<int>& initialPositions, GameBoard* board);
	static void randomizeBoard(GameBoard* board);
	static void segment_swap(GameBoard* board, int pos1, int pos2);
	//static void renderBoard(GameBoard* board);
	//static void cleanupBoard(GameBoard* board);
};

#endif // !BOARD_H
