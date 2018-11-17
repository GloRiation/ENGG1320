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
#include "imageProcessing.h"

using namespace cv;
using namespace std;

const int VideoWidth = 1280;
const int VideoHeight = 720;
const bool UsingCam = true;

bool IsBlue;
static VideoCapture MyVideo;

int main(){
    Mat InputImg, OutputImg;
    cout << "ENGG1320 ----- Fan camera locating program\n";
    if (UsingCam){
        MyVideo.open(-1);
        if (!MyVideo.isOpened()){
			cout << "Could not open the camera. " << endl;
			return -1;
		}
		else
			cout << "Camera successfully opened. " << endl;
	}
    else{
		MyVideo.open("LabRecordB.avi"); 
		if (!MyVideo.isOpened()){
			cout << "Could not open the video. " << endl;
			return 1;
		}
		else
			cout << "Video successfully opened. " << endl;
	}
    if (UsingCam){
		cout << "Setting up the camera" << endl;
		// Setup Camera 
		MyVideo.set(CV_CAP_PROP_FRAME_WIDTH, VideoWidth);
		MyVideo.set(CV_CAP_PROP_FRAME_HEIGHT, VideoHeight);
		MyVideo.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
		//MyVideo.set(CV_CAP_PROP_EXPOSURE, 0.99);
		
        //cout << "FPS:" << MyVideo.get(CV_CAP_PROP_FPS) << endl;
        //cout << "Exposure" << MyVideo.get(CV_CAP_PROP_EXPOSURE) << endl;
        
		// Seems ubuntu cannot get correct data from MyVideo.get() command.
	}
	double FramteRate = MyVideo.get(CAP_PROP_FPS);

	cout << "Processing starts!" << endl;
 
    while(true){
        MyVideo >> InputImg;
        if (InputImg.empty()){
		    cout << "Video has been read out. " << endl;
		    return -2;
        }
        locatingTarget(InputImg, OutputImg);
        imshow("processed", OutputImg);
        imshow("contour", InputImg);
        if (waitKey(1) != -1){
		    cout << "Paused by user. Wanna continue? Yes 1 or No 0: ";
		    cin >> IsBlue; // Make use of this variable.
	    	if (IsBlue){
		    	cout << "Blue 1 or red 0, make your choice: ";
			    cin >> IsBlue;
		    }
		    else
			    return -3;
	    }

    }    
    return 0;
}
