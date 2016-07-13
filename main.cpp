// #include <opencv2/core/core.hpp>
// #include <opencv2/imgproc.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>
// #include <iostream>
// // #include <opencv2/imgcodecs.hpp>
// #include <opencv2/video.hpp>
// #include <opencv2/imgproc.hpp>
// #include <climits>
// #include <cfloat>
// #include <vector>
// #include <limits>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/imgproc.hpp>
// #include <opencv2/video/tracking.hpp>
#include <iostream>

#pragma comment(lib, "opencv_core310.lib")
#pragma comment(lib, "opencv_imgcodecs310.lib")
#pragma comment(lib, "opencv_videoio310.lib")
#pragma comment(lib, "opencv_highgui310.lib")
#pragma comment(lib, "opencv_tracking310.lib")
#pragma comment(lib, "opencv_imgproc310.lib")

using namespace cv;
using namespace std;

// Rect2d selectROI(const String &video_name, const Mat &frame);

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
   Rect2d roi = selectROI("MyVideo", frame); 
   Mat target(frame, roi);
   imwrite("target.jpeg", target);
   std::cout << "(x, y, width, height) = (" << roi.x << "," << roi.y << "," << roi.width << "," << roi.height << ")" << std::endl;
   if (roi.width == 0 || roi.height == 0){
     return -1;
   }

   Scalar colorkcf = Scalar(0, 255, 0);
   Ptr<cv::Tracker> tracker = Tracker::create("KCF");
   tracker->init(frame, roi);


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

    flip(frame, frame, 1);
    //Here we go
    tracker->update(frame, roi);
    rectangle(frame, roi, colorkcf, 1, 1);
    putText(frame, "- KCF", Point(5, 20), FONT_HERSHEY_SIMPLEX, .5, colorkcf, 1, CV_AA);

    imshow("MyVideo", frame); //show the frame in "MyVideo" window

    if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
    {
      cout << "esc key is pressed by user" << endl;
      break;
    }
  }
  return 0;
}


// Rect2d selectROI(const String &video_name, const Mat &frame)
// {
//     struct Data
//     {
//         Point center;
//         Rect2d box;
//
//         static void mouseHandler(int event, int x, int y, int flags, void *param)
//         {
//             Data *data = (Data*)param;
//             switch( event )
//             {
//             // start to select the bounding box
//             case EVENT_LBUTTONDOWN:
//                 data->box = cvRect( x, y, 0, 0 );
//                 data->center = Point2f((float)x,(float)y);
//                 break;
//             // update the selected bounding box
//             case EVENT_MOUSEMOVE:
//                 if(flags == 1)
//                 {
//                     data->box.width  = 2 * (x - data->center.x);
//                     data->box.height = 2 * (y - data->center.y);
//                     data->box.x = data->center.x - data->box.width / 2.0;
//                     data->box.y = data->center.y - data->box.height / 2.0;
//                 }
//                 break;
//             // cleaning up the selected bounding box
//             case EVENT_LBUTTONUP:
//                 if( data->box.width < 0 )
//                 {
//                     data->box.x += data->box.width;
//                     data->box.width *= -1;
//                 }
//                 if( data->box.height < 0 )
//                 {
//                     data->box.y += data->box.height;
//                     data->box.height *= -1;
//                 }
//                 break;
//             }
//         }
//     } data;
//
//     setMouseCallback(video_name, Data::mouseHandler, &data);
//     while(waitKey(1) < 0)
//     {
//         Mat draw = frame.clone();
//         rectangle(draw, data.box, Scalar(255,0,0), 2, 1);
//         imshow(video_name, draw);
//     }
//     return data.box;
// }
