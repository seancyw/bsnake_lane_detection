#include"opencv/cv.h"
#include<opencv2/highgui/highgui.hpp>
#include<bits/stdc++.h>

using namespace std;
using namespace cv;

extern int lowThreshold;
extern int highThreshold;

Mat find_edges(Mat img)
{
	//directly in single channel
	Mat edges;
	cvtColor(img, img, CV_BGR2GRAY);
	imshow("bw", img);
	blur(img,img,Size(3,3));
	Canny( img, edges, lowThreshold, highThreshold, 3 );

	return edges;
}