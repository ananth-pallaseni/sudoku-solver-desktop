
#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


//int main(int argc, char* argv[]) {
//	Mat img;
//	if(argc >= 2) {
//		img = imread(argv[1], 0);
//	}
//	else {
//		cout << "No arguments provided" << endl;
//	}
//	imshow("orig", img);
//	//BLUR
//	Mat imBlur = gBlur(img);
//	//imshow("blur", imBlur);
//
//	//THRESHHOLD
//	Mat t = thresh(imBlur);
//	//imshow("thresh", t);
//
//	//NOT
//	bitwise_not(t, t);
//	//imshow("not", t);
//
//	//DILATE
//	//Mat d = dil(t);
//	//imshow("dilate", d);
//
//	//DETECT BLOBS
//	detectLargestBlob(t);
//	//imshow("fill", t);
//
//	Mat newImg = Mat::zeros(img.rows, img.cols, CV_8UC3);
//	vector<Vec4i> lines = houghTransform(t, img);
//	//imshow("houghlines", img);
//
//	//CORNER FIND
//	Point points[4];
//	findCorners(img, lines, points);
//
//	for(int i = 0; i < 4; i++) {
//		circle(img, points[i], 5, Scalar(255, 0, 0), -1);
//		cout << points[i] << endl;
//	}
//	//imshow("crop corners", img);
//
//	// TRANSFORM
//	Point2f origCorners[4] = {points[2], points[3], points[0], points[1]}; // TODO: list corners in order
//	Point2f newCorners[4] = {Point(0, 0), Point(img.cols - 1, 0), Point(0, img.rows - 1), Point(img.cols - 1, img.rows - 1)};
//	Mat warpMat = getPerspectiveTransform(origCorners, newCorners);
//	warpPerspective(img, img, warpMat, img.size());
//	imshow("warp", img);
//
//
//	waitKey();
//
//	//TODO: time each stage, see where the blocks are
//}


Mat gBlur(Mat img) {
	Mat dst;
	GaussianBlur(img, dst, Size(5, 5), 0);
	return dst;
}

Mat thresh(Mat img) {
	Mat dst = Mat(img.size(), CV_8UC1);
	adaptiveThreshold(img, dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
	return dst;
}

Mat dil(Mat img) {
	Mat dst;
	Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
    dilate(img, dst, kernel);
	return dst;
}


// faster than findCorners but not as accurate
Point* fastCorners(Mat img, Point* corners) {
	int middleX = img.size().width / 2;
	int middleY = img.size().height / 2;
	int largestX = 0;
	int smallestX = img.size().width;
	int largestY = 0;
	int smallestY = img.size().height;
	uchar* mid = img.ptr(middleY);
	for(int x = 0; x < img.size().width; x++) {
		uchar pVal = mid[x];
		if(pVal == 255) {
			if(x < smallestX) {
				smallestX = x;
			}
			if(x > largestX) {
				largestX = x;
			}
		}
	}
	for(int y = 0; y < img.size().height; y++) {
		uchar pVal = img.ptr(y)[middleX];
		if(pVal == 255) {
			if(y < smallestY) {
				smallestY = y;
			}
			if(y > largestY) {
				largestY = y;
			}
		}
	}

	int minTopX = img.size().width;
	int maxTopX = 0;
	int minBotX = img.size().width;
	int maxBotX = 0;
	uchar* topRow = img.ptr(largestY);
	uchar* botRow = img.ptr(smallestY);
	for(int x = 0; x < img.size().width; x++) {
		uchar pValTop = topRow[x];
		uchar pValBot = botRow[x];
		if(pValTop == 255) {
			if(x < minTopX) {
				minTopX = x;
			}
			if(x > maxTopX) {
				maxTopX = x;
			}
		}
		if(pValBot == 255) {
			if(x < minBotX) {
				minBotX = x;
			}
			if(x > maxBotX) {
				maxBotX = x;
			}
		}
	}
	/*int minLeftY = img.size().height;
	int maxLeftY = 0;
	int minRightY = img.size().height;
	int maxRightY = 0;
	for(int y = 0; y < img.size().height; y++) {
		uchar pValLeft = img.ptr(y)[smallestX];
		uchar pValRight = img.ptr(y)[largestX];
		if(pValLeft == 255) {
			if(y < minLeftY) {
				minLeftY = y;
			}
			if(y > maxLeftY) {
				maxLeftY = y;
			}
		}
		if(pValRight == 255) {
			if(y < minRightY) {
				minRightY = y;
			}
			if(y > maxRightY) {
				maxRightY = y;
			}
		}
	}*/

	//Crop corners
	Point cTopLeft = Point(smallestX, largestY);
	Point cTopRight = Point(largestX, largestY);
	Point cBotRight = Point(largestX, smallestY);
	Point cBotLeft = Point(smallestX, smallestY);
	corners[0] = cTopLeft;
	corners[1] = cTopRight;
	corners[2] = cBotRight;
	corners[3] = cBotLeft;

	// Actual corners of puzzle
	Point topLeft = Point(minTopX, largestY);
	Point topRight = Point(maxTopX, largestY);
	Point botRight = Point(maxBotX, smallestY);
	Point botLeft = Point(minBotX, smallestY);
	corners[4] = topLeft;
	corners[5] = topRight;
	corners[6] = botRight;
	corners[7] = botLeft;
	
	return corners;
}


void q(Mat img) {
	for(int row = 0; row < img.rows / 10; row++) {
		uchar* p = img.ptr(row);
		for(int col = 0; col < img.cols / 10; col++) {
			uchar x = p[col];
			cout << (int)x << endl;
		}
	}
}