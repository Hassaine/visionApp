
#include "visionFunction.h"

string intToString(int number)
{

    std::stringstream ss;
    ss << number;
    return ss.str();
}




void morphOps(Mat &thresh)
{

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle

    Mat erodeElement = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

    erode(thresh, thresh, erodeElement);
    erode(thresh, thresh, erodeElement);

    dilate(thresh, thresh, dilateElement);
    dilate(thresh, thresh, dilateElement);
}
void drawObject(int x, int y, Mat &frame)
{

    circle(frame, Point(x, y), 20, Scalar(0, 255, 0), 2);


}

bool trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed)
{

    Mat temp;
    threshold.copyTo(temp);
    //these two vectors needed for output of findContours
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);


    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;
    if (hierarchy.size() > 0)
    {
        //printf("found a counteur \n");
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if (numObjects < MAX_NUM_OBJECTS)
        {
            for (int index = 0; index >= 0; index = hierarchy[index][0])
            {

                Moments moment = moments((cv::Mat)contours[index]);
                double area = moment.m00;

                //if the area is less than 20 px by 20px then it is probably just noise
                //if the area is the same as the 3/2 of the image size, probably just a bad filter
                //we only want the object with the largest area so we safe a reference area each
                //iteration and compare it to the area in the next iteration.
                if (area > MIN_OBJECT_AREA && area < MAX_OBJECT_AREA && area > refArea)
                {
                    x = moment.m10 / area;
                    y = moment.m01 / area;
                    objectFound = true;
                    refArea = area;
                }
                else
                    objectFound = false;
            }
            //let user know you found an object
            if (objectFound == true)
            {
                putText(cameraFeed, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
                //draw object location on screen
                drawObject(x, y, cameraFeed);
            }
        }
        else
            putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
    }
    return objectFound;
}
