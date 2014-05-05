#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//TODO: comment
void detectLargestBlob(Mat img) {
	Mat outImg = img.clone();
	int largestArea = 0;
	Point blobStart; // TODO: Error catching
	Scalar fillColour = CV_RGB(0, 0, 1); //fill in with black
	for(int y = 0; y < img.rows; y++) {
		uchar* row = img.ptr(y);
		for(int x = 0; x < img.cols; x++) {
			uchar pixel = row[x];
			if(pixel == 255) { // only fill white pixels
				int blobArea = floodFill(img, Point(x, y), fillColour);
				if(blobArea > largestArea) {
					largestArea = blobArea;
					blobStart = Point(x, y);
				}
			}
		}
	}
	floodFill(img, blobStart, CV_RGB(0, 0, 255));
	//Refill black?
	for(int y = 0; y < img.rows; y++) {
		uchar* row = img.ptr(y);
		for(int x = 0; x < img.cols; x++) {
			uchar pixel = row[x];
			if(pixel != 255) { // only fill white pixels
				int mm = floodFill(img, Point(x, y), CV_RGB(0, 0, 0));
			}
		}
	}
}

Point* findCorners(Mat img, vector<Vec4i> lines, Point* corners) {
	int maxDist = (img.size().width + img.size().height) * (img.size().width + img.size().height); // TODO: consider this value accidentally hitting int.max ?
	Point imgCorners[4] = {Point(0, img.size().height), Point(img.size().width, img.size().height), Point(0, 0), Point(img.size().width, 0)}; // TopLeft, TopRight, BottomLeft, BottomRight
	int dists[4] = {maxDist, maxDist, maxDist, maxDist};
	for(int i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];
		for(int j = 0; j < 4; j++) {
			int dist1 = sqrt(pow(abs(imgCorners[j].x - l[0]) , 2) + pow(abs(imgCorners[j].y - l[1]), 2)); // TODO: do we NEED to calc both distances?
			int dist2 = sqrt(pow(abs(imgCorners[j].x - l[2]) , 2) + pow(abs(imgCorners[j].y - l[3]), 2)); // TODO: do this without the sqrts?
			if(dist1 < dist2 && dist1 < dists[j]){
				corners[j] = Point(l[0], l[1]);
				dists[j] = dist1;
			}
			else if(dist2 < dist1 && dist2 < dists[j]) {
				corners[j] = Point(l[2], l[3]);
				dists[j] = dist2;
			}
		}
	}
	return corners;
}



//Takes a single channel black and white image, finds the sudoku in it and then crops the image around that sudoku. *WARNING* - changes the input image. 
void isolateSudoku(Mat img) {
	Mat tempImg = img.clone();
	// Blur the image to reduce noise
	GaussianBlur(tempImg, tempImg, Size(5, 5), 0);

	// Threshold the image
	adaptiveThreshold(tempImg, tempImg, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);

	// Invert the image so that black pixels are white and vice versa
	// TODO: is this necessary?
	bitwise_not(tempImg, tempImg);

	// Detect the largest square in the image - this is probably the sudoku grid 
	detectLargestBlob(tempImg);

	// Perform a Hough Transform to get the constituent lines of the square
	vector<Vec4i> lines;
	HoughLinesP(tempImg, lines,1, CV_PI/180, 50);

	// Find the corners of this square
	Point points[4];
	findCorners(tempImg, lines, points);

	// Use a perspective transform to simultaneously crop and straighten image
	Point2f origCorners[4] = {points[2], points[3], points[0], points[1]}; // TODO: list corners in order
	Point2f newCorners[4] = {Point(0, 0), Point(tempImg.cols - 1, 0), Point(0, tempImg.rows - 1), Point(tempImg.cols - 1, tempImg.rows - 1)};
	Mat warpMat = getPerspectiveTransform(origCorners, newCorners);
	warpPerspective(img, img, warpMat, img.size());
}

void squares(Mat img, Mat sq[9][9]) {
	int sideLength = img.rows / 9;
	int i = 0;
	int j = 0;
	for(int x = 0; x < img.cols - img.cols / 9; x += img.cols / 9) {
		for(int y = 0; y < img.rows - img.rows / 9; y += img.rows / 9) {
			Rect roi = Rect(x, y, img.cols / 9, img.rows / 9);
			sq[i][j] = img(roi);
			j++;
		}
		j = 0;
		i++;
	}
}

Rect contourTest(Mat img, Mat drawing) {
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat cannyOut;
	Canny(img, cannyOut, 100, 200, 3, false);
	findContours(cannyOut, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	int max = 0;
	Rect largestBlob;
	for(int i = 0; i < contours.size(); i++) {
		Rect blob = boundingRect(contours[i]);
		int size = blob.area() ;
		if(size > max) {
			max = size;
			largestBlob = blob;
		}
	}

	return largestBlob;
}

Rect getDigitBoundingBox(Mat img) {
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat cannyOut;
	Canny(img, cannyOut, 100, 200, 3, false);
	findContours(cannyOut, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	int minArea = img.size().area() / 2;
	for(int i = 0; i < contours.size(); i++) {
		Rect blob = boundingRect(contours[i]);
		int area = blob.area() ;
		if(area < minArea) {
			return blob;
		}
	}
}
