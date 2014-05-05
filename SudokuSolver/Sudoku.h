#include "stdafx.h"

class Sudoku {
	int grid[9][9];
	public:
		Sudoku();
		void setSquare(int x, int y, int val);
		int valueAt(int x, int y);
		void solve();
};
	
void Sudoku::setSquare(int x, int y, int val) {
	grid[x][y] = val;
}

int Sudoku::valueAt(int x, int y) {
	return grid[x][y];
}

void Sudoku::solve() {
	// Todo: create
}