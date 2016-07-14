#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

// #pragma comment(lib, "opencv_core310.lib")
// #pragma comment(lib, "opencv_imgcodecs310.lib")
// #pragma comment(lib, "opencv_videoio310.lib")
// #pragma comment(lib, "opencv_highgui310.lib")
// #pragma comment(lib, "opencv_tracking310.lib")
// #pragma comment(lib, "opencv_imgproc310.lib")

using namespace cv;
using namespace std;


int main(int argc, char* argv[])
{
  VideoCapture cap; //

  cap.open("http://192.168.1.130:8080/?action=stream&ignored.mjpg");
  // cap.open(0);
  if (!cap.isOpened())  // if not success, exit program
  {
    cout << "Cannot open the video cam" << endl;
    return -1;
  }

  double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
  double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

  cout << "Frame size : " << dWidth << " x " << dHeight << endl;

  namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

  //prepare
  Mat frame;
  cap.read(frame);
  bool bSuccess = cap.read(frame); // read a new frame from video
  if (!bSuccess) //if not success, break loop
  {
    cout << "Cannot read a frame from video stream" << endl;
    return -1;
  }
   // flip(frame, frame, 1);
   Scalar colorkcf = Scalar(0, 255, 0);
   Scalar colorRed = Scalar(0, 0, 255);
   Scalar colorBlue = Scalar(255, 0, 0);
   putText(frame, "Select ROI", Point(5, 20), FONT_HERSHEY_SIMPLEX, .5, colorRed, 1, CV_AA);
   Rect2d roi = selectROI("MyVideo", frame); 
   Mat target(frame, roi);
   imwrite("target.jpeg", target);
   std::cout << "(x, y, width, height) = (" << roi.x << "," << roi.y << "," << roi.width << "," << roi.height << ")" << std::endl;
   if (roi.width == 0 || roi.height == 0){
     return -1;
   }

   Ptr<cv::Tracker> tracker = Tracker::create("KCF");
   tracker->init(frame, roi);

   // cv::VideoWriter writer( "output.avi", cv::VideoWriter::fourcc('M','J','P','G'), cap.get(cv::CAP_PROP_FPS), cv::Size((int)cap.get(cv::CAP_PROP_FRAME_WIDTH), (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT)));
   cv::VideoWriter writer( "output.mov", cv::VideoWriter::fourcc('M','P','4','V'), cap.get(cv::CAP_PROP_FPS), cv::Size((int)cap.get(cv::CAP_PROP_FRAME_WIDTH), (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

  while (1)
  {
    Mat frame;
    Mat contours;

    bool bSuccess = cap.read(frame); // read a new frame from video

    if (!bSuccess) //if not success, break loop
    {
      cout << "Cannot read a frame from video stream" << endl;
      break;
    }

    // flip(frame, frame, 1);
    //Here we go
    tracker->update(frame, roi);
    rectangle(frame, roi, colorkcf, 1, 1);
    putText(frame, "<<Tracking>>", Point(5, 20), FONT_HERSHEY_SIMPLEX, .5, colorRed, 1, CV_AA);
    putText(frame, "- KCF", Point(5, 40), FONT_HERSHEY_SIMPLEX, .5, colorkcf, 1, CV_AA);

    imshow("MyVideo", frame); //show the frame in "MyVideo" window

    writer << frame;

    if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
    {
      cout << "esc key is pressed by user" << endl;
      break;
    }
  }
  return 0;
}

