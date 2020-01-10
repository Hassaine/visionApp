#ifndef GAMELOGIQUE_H
#define GAMELOGIQUE_H
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui_c.h>


cv::Point GravityCenter(int positionX,int positionY, int hight ,int weight);
float distance2p( cv::Point point1,cv::Point point2);
void refreshScreen(cv::Mat originGrid,cv::Mat &grid,cv::Mat mouse,cv::Mat cheese,cv::Mat cat,int catInitPosition[][2],int chesseInitPosition[][2],int mouseInitX,int mouseInitY);


#endif // GAMELOGIQUE_H
