


#include "gameLogique.h"



cv::Point GravityCenter(int positionX,int positionY, int hight ,int width){

   int Points [4][2]={{positionX,positionY},{positionX+width,positionY},{positionX+width,positionY+hight},{positionX,positionY+hight}};
   int sumX = 0; int sumY = 0;


        for(int i=0; i<4 ;i++){
            sumX += Points[i][0];
            sumY +=Points[i][1];
        }

    cv::Point centroid = cv::Point();
       centroid.x = sumX/4;
       centroid.y = sumY/4;

    return centroid;

}

float distance2p( cv::Point point1,cv::Point point2){


        // Calculating distance
        return sqrt(pow(point1.x - point2.x, 2) +
                    pow(point1.y - point2.y, 2) * 1.0);


}
void refreshScreen(cv::Mat originGrid,cv::Mat &grid,cv::Mat mouse,cv::Mat cheese,cv::Mat cat,int catInitPosition[][2],int chesseInitPosition[][2],int mouseInitX,int mouseInitY){
    grid = originGrid.clone();
    for (int i = 0; i < 3; ++i) {
            cheese.copyTo(grid(cv::Rect(chesseInitPosition[i][0],chesseInitPosition[i][1],cheese.cols, cheese.rows)));
    }

      mouse.copyTo(grid(cv::Rect(mouseInitX,mouseInitY,mouse.cols, mouse.rows)));

     for (int i = 0; i < 2; ++i) {
              cat.copyTo(grid(cv::Rect(catInitPosition[i][0],catInitPosition[i][1],cat.cols, cat.rows)));
     }

}
