#include"opencv/cv.h"
#include<opencv2/highgui/highgui.hpp>
#include<bits/stdc++.h>
#include "lane_detector.h"

using namespace cv;
using namespace std;

int lowThreshold=500;
int highThreshold=150;

void extract_segments(Mat img_segments[], Mat img,int segments[], int n_segments);

Mat find_edges(Mat img);

int main()
{
	//initialize important variables here
	int n_segments=5;
	int segments[6]={0,80, 230, 285, 300,1000};
	Mat img_segments;

	cv::Mat img=cv::imread("images/l2.jpg", CV_LOAD_IMAGE_COLOR);
	cv::resize(img, img, cv::Size(1000,1000));
	imshow("lanes", img);

	int max_lowThreshold=500, max_highThreshold=500;
	namedWindow("Edge threshold", CV_WINDOW_AUTOSIZE);
	createTrackbar( "Min Threshold:", "Edge threshold", &lowThreshold, max_lowThreshold);
	createTrackbar( "Max Threshold:", "Edge threshold", &highThreshold, max_highThreshold);

	Mat edges;
	while(true)
	{
		edges=find_edges(img);
		imshow("edges", edges);

		char c=(char)waitKey(10);
		if(c=='q') 
			break;
	}

	vector<Vec4i> lines[n_segments];
	//extract_segments(img_segments, edges, segments, n_segments);
	for(int k=0;k<n_segments;k++)
		{
			img_segments=edges-edges;
			for(int i=segments[k];i<segments[k+1];i++)
				{
					for(int j=0;j<1000;j++)
					{
						img_segments.at<uchar>(i,j)=edges.at<uchar>(i,j);
						
					}
				}	

				HoughLinesP(img_segments, lines[k], 1, CV_PI/180, 100, 0, 0 );
		}

	/*vector<Vec4i> lines[n_segments];
	for(int i=0; i<n_segments ;i++)
		HoughLinesP(img_segments[i], lines[i], 1, CV_PI/180, 100, 0, 0 );
 	cout<<"reached"<<endl;*/
		
	for(int j=0;j<n_segments;j++)
	{
	 for( size_t i = 0; i < lines[j].size(); i++ )
	
  {
     float rho = lines[j][i][0], theta = lines[j][i][1];
     Point pt1, pt2;
     double a = cos(theta), b = sin(theta);
     double x0 = a*rho, y0 = b*rho;	
     pt1.x = cvRound(x0 + 1000*(-b));
     pt1.y = cvRound(y0 + 1000*(a));
     pt2.x = cvRound(x0 - 1000*(-b));
     pt2.y = cvRound(y0 - 1000*(a));
     line( img, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
  } 
  }

  	vector<Point> intersections;
  for(int k=0;k<n_segments;k++)
  {
  for(int i=0;i<lines[k].size();i++)
  	{

  		for(int j=0;j<lines[k].size();j++)
  				{
  					float rho = lines[k][i][0], theta = lines[k][i][1];
				     Point pt1, pt2;
				     double a = cos(theta), b = sin(theta);
				     double x0 = a*rho, y0 = b*rho;	
				     pt1.x = cvRound(x0 + 1000*(-b));
				     pt1.y = cvRound(y0 + 1000*(a));
				     pt2.x = cvRound(x0 - 1000*(-b));
				     pt2.y = cvRound(y0 - 1000*(a));
				     float rho1 = lines[k][j][0], theta1 = lines[k][j][1];
				     Point pt3, pt4;
				     double c = cos(theta1), d = sin(theta1);
				     double x1 = c*rho1, y1 = d*rho1;	
				     pt3.x = cvRound(x1 + 1000*(-d));
				     pt3.y = cvRound(y1 + 1000*(c));
				     pt4.x = cvRound(x1 - 1000*(-d));
				     pt4.y = cvRound(y1 - 1000*(c));
				     Point output;
				     if((pt1.x-pt2.x)*(pt3.y-pt4.y)-(pt1.y-pt2.y)*(pt3.x-pt4.x)!=0)
				     	{
				     		output.x=((pt1.x*pt2.y-pt1.y*pt2.x)*(pt3.x-pt4.x)-((pt3.x*pt4.y-pt3.y*pt4.x)*(pt1.x-pt2.x)))/((pt1.x-pt2.x)*(pt3.y-pt4.y)-(pt1.y-pt2.y)*(pt3.x-pt4.x));

				     		output.y=((pt1.x*pt2.y-pt1.y*pt2.x)*(pt3.y-pt4.y)-((pt3.x*pt4.y-pt3.y*pt4.x)*(pt1.y-pt2.y)))/((pt1.x-pt2.x)*(pt3.y-pt4.y)-(pt1.y-pt2.y)*(pt3.x-pt4.x));
				     	}	
				     	//cout<<output.x<<" "<<output.y<<endl;

				     	if(output.x<1000&&output.y<1000&&output.x>0&&output.y>0)
				     	
				     	{ circle( img, Point( output.x, output.y ), 10.0, Scalar( 0, 0, 255 ), 5, 8 );
				     		intersections.push_back(output);
				     	}

  				}
  	}
  	}

  	//find best row as horizon
  	int ctr;
  	int colmx=0;
  	int maxctr=0;

  	for(int i=26;i<970;i++)
  		{
  			ctr=0;
  				for(int j=0;j<intersections.size();j++)
  				{
  					if(intersections[j].y<i+20&&intersections[j].y>i-20)
  					{
  						if(intersections[j].y==i)
  							ctr+=1000;
  						else if(intersections[j].y<i+10&&intersections[j].y>i-10)
  							ctr+=100;
  						else
  							ctr+=40;
  						

  					}
  					

  				}
  				if(ctr>maxctr)
  						{
  							ctr=maxctr;
  							colmx=i;

  						}

  		}
  		cout<<"max:"<<colmx<<endl;
  		Point p1,p2;
  		p1.x=0;
  		p1.y=1000-colmx;
  		p2.x=999;
  		p2.y=1000-colmx;
  		 	line( img, p1, p2, Scalar(255,0,0), 3, CV_AA);

	imshow("original", img);
	waitKey(0);

	return 0;
}