#ifndef VISIONFUNCTION_H
#define VISIONFUNCTION_H

#include <sstream>
#include <string>
#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui_c.h>


using namespace cv;
using namespace std;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
const int MAX_NUM_OBJECTS = 50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20 * 20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT * FRAME_WIDTH / 1.5;

string intToString(int number);
void drawObject(int x, int y, Mat &frame);
void morphOps(Mat &thresh);
bool trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed);





#endif // VISIONFUNCTION_H
