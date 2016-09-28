#include"opencv/cv.h"
#include<opencv2/highgui/highgui.hpp>
#include<bits/stdc++.h>

using namespace std;
using namespace cv;

extern int lowThreshold;
extern int highThreshold;

void extract_segments(Mat img_segments[], Mat img,int segments[], int n_segments)
{
	int i, cum_length=0;

	img_segments[0]=img(cv::Rect(0, 0, 1000, segments[0])).clone();
	imshow("segment",img_segments[0]);
	waitKey(0);
	cum_length+=segments[0];

	for(i=1;i<=n_segments-2;i++)
	{
		img_segments[i]=img(cv::Rect(0, cum_length, 1000, segments[i])).clone();
		cum_length+=segments[i];
	}

	img_segments[n_segments-1]=img(cv::Rect(0, cum_length, 1000, 1000-cum_length)).clone();
	return;
}

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