#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/features2d.hpp> // region detection
#include <time.h>
#include <opencv2/core/types_c.h> // To use CvScalar
#include <stdio.h>
#include <iostream>
#include <valarray> // for faster and simple vector math
#include <cmath> // atan calculation (seems valarray already includes it.)
#include <thread> // Multi threading
#include <unistd.h> // linux read and write
#include <fcntl.h> // linux file input, output
#include <signal.h>
#include <iomanip>

using namespace cv;
using namespace std;

void locatingTarget(Mat &input, Mat &output);

void locatingTarget(Mat &input, Mat &output){
    vector<vector<Point>> contours;
    const int minSize = 5;
    vector<RotatedRect> validPatch;
    RotatedRect target;
    Mat hsv;
    cvtColor(input, hsv, CV_BGR2HSV);
    Scalar hsv_l(100, 10, 50);
    Scalar hsv_h(140, 200, 255);
    inRange(hsv, hsv_l, hsv_h, output);
    findContours(output, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point());
    vector<RotatedRect> potentialEllipse(contours.size()); 
    for (int i = 0; i < contours.size(); i++){
        if (contours[i].size() > minSize){ 
            potentialEllipse[i] = fitEllipse(contours[i]);   
            validPatch.push_back(potentialEllipse[i]);
        }
    }
    double maxArea = 0;
    int maxInd = 0;
    double area = 0;
    bool found = false;
    for (int i = 0; i < validPatch.size(); i++){
        if (validPatch[i].size.area() > maxArea){
            maxArea = validPatch[i].size.area();
            maxInd = i;
            found = true;
        }
    }
    if (found){
        target = validPatch[maxInd];
        Scalar color = CV_RGB(255, 0, 0);
        line(input, {(int)target.center.x - 10, (int)target.center.y}, {(int)target.center.x + 10, (int)target.center.y}, color, 2);
        line(input, {(int)target.center.x, (int)target.center.y - 10}, {(int)target.center.x, (int)target.center.y + 10}, color, 2);
    }
    double disX = (target.center.x - 640) / 640;
    double disY = (target.center.y - 360) / 360;
    cout << fixed << setprecision(2);
    cout << '\r' << "disX:" << disX << " disY:" << disY; 
}
