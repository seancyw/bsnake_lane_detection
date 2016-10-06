

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include<bits/stdc++.h>



extern int lowThreshold;
extern int highThreshold;

int find_intersection(Vec4i l1, Vec4i l2);
void extract_segments(Mat img_segments[], Mat img,int segments[], int n_segments);
void merge_segments(Mat img_segments[],Mat img, int segments[], int n_segments);
Mat find_edges(Mat img);


