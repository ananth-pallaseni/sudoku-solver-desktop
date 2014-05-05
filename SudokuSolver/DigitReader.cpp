#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

vector<vector<Point>> contours[16];
vector<vector<vector<Point>>> digits[10];

int compareVectors(vector<vector<Point>> vec1, vector<vector<Point>> vec2) {
	int q = 0;
	if(vec1.size() < vec2.size()) {
		q = vec1.size();
	}
	else {
		q = vec2.size();
	}
	for(int i = 0; i < q; i++) {
		if( vec1[i] != vec2[i] ) {

		}
	}
}

int getMostLikelyMatch(vector<vector<Point>> vec1, vector<vector<Point>> vec2) {

}

int ocr(Mat img) {
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat cannyOut;
	Canny(img, cannyOut, 100, 200, 3, false);
	findContours(cannyOut, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	for(int i = 0; i < 10; i++) {
		if(compareVectors(contours, digits[i]) == 1) {
			return i;
		}
	}
}


int main(int argc, char* argv[]) {
	Mat img;
	if(argc >= 2) {
		img = imread(argv[1], 0);
	}
	else {
		cout << "No arguments provided" << endl;
	}

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat cannyOut;
	Canny(img, cannyOut, 100, 200, 3, false);
	findContours(cannyOut, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	int max = 0;
	for(int i = 0; i < contours.size(); i++) {
		Rect box = boundingRect(contours[i]);
		Mat q1 = img(box);
		vector<vector<Point> > f1;
		Mat c1;
		vector<Vec4i> h1;
		Canny(q1, c1, 100, 200, 3, false);
		findContours(c1, f1, h1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		if(f1.size() > max) {
			max = f1.size();
		}
	}
	cout << max << endl;
}

	

