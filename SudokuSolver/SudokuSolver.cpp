// SudokuSolver.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Sudoku.h"
#include "PreProcess.h"

using namespace cv;
using namespace std;

//int main(int argc, char* argv[]) {
//	Mat img;
//	cout << "1" << endl;
//	if(argc >= 2) {
//		img = imread(argv[1], 0);
//	}
//	else {
//		cout << "No arguments provided" << endl;
//	}
//	
//	isolateSudoku(img);
//	imshow("1", img);
//
//	Mat sq[9][9];
//	squares(img, sq);
//
//	Mat q = sq[0][1];
//	imshow("q", q);
//	Rect a = getDigitBoundingBox(q);
//	Mat s = q(a);
//	imshow("qq", s);
//
//	
//
//	waitKey();
//}
//
