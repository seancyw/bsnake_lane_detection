

#include  "../include/lane.hpp"


using namespace cv;
using namespace std;

int lowThreshold=70;
int highThreshold=150;

int main()
{
	//initialize important variables here
	int n_segments=5;
	int segments[5]={75, 140, 215, 250, 320};
	Mat img_segments[5];
	std::stringstream window_name;

	cv::Mat img=cv::imread("images/l7.jpg", CV_LOAD_IMAGE_COLOR);
	cv::resize(img, img, cv::Size(1000,1000));
	imshow("lanes", img);

	img=removeShadow(img);

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

	extract_segments(img_segments, edges, segments, n_segments);

	int i, j, k;

	/*display edge segments
	for(i=0;i<n_segments;i++)
		cout<<img_segments[i].rows<<" "<<img_segments[i].cols<<endl;
	
	for(i=0;i<n_segments;i++)
  	{
  		window_name<<"edges"<<i<<"";
  		imshow(window_name.str(), img_segments[i]);
  		window_name.str("");
  	}*/

	vector<Vec4i> lines[n_segments];
	int hough_threshold[5]={30, 30, 40, 50, 50};
	int hough_minLineLength[5]={20, 25, 25, 30, 50};
	for(i=0; i<n_segments ;i++)
		HoughLinesP(img_segments[i], lines[i], 1, CV_PI/180, hough_threshold[i], hough_minLineLength[i], 50 );

  	Mat line_segments[n_segments];
  	Mat empty=img-img;
  	extract_segments(line_segments, empty, segments, n_segments);


  	for(i=0;i<n_segments;i++)
  		for(j=0;j<lines[i].size();j++)
  		{
  			Vec4i l = lines[i][j];
  			line( line_segments[i], Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
  		}


  	/* display line segments
  	for(i=0;i<n_segments;i++)
  	{
  		window_name<<"window"<<i<<"";
  		imshow(window_name.str(), line_segments[i]);
  		window_name.str("");
  	}*/


  	Mat line=img-img;
  	merge_segments(line_segments, line, segments, n_segments);
  	imshow("detected lines",line);

  	for(i=0;i<n_segments;i++)
  		cout<<"#lines for "<<i<<": "<<lines[i].size()<<endl;

  	int vanish_row_vote[2000]={0};

  	int cum_sum=1000;
  	for(i=4;i>=3;i--)
  	{
  		cum_sum-=segments[i];
  		for(j=0;j<lines[i].size();j++)
  			for(k=0;k<lines[i].size();k++)
  			{
  				if(j==k)
  					continue;

  				int vanish_row=find_intersection(lines[i][j], lines[i][k])+cum_sum;

  				//for checking intersection function
  				/*Mat ci(1000, 1000, CV_8UC3, Scalar(0));
  				Mat cs[5];
  				extract_segments(cs, ci, segments, n_segments);
  				cv::line( cs[i], Point(lines[i][j][0], lines[i][j][1]), Point(lines[i][j][2], lines[i][j][3]), Scalar(255,0,0), 3, CV_AA, 0);
  				cv::line( cs[i], Point(lines[i][k][0], lines[i][k][1]), Point(lines[i][k][2], lines[i][k][3]), Scalar(255,0,0), 3, CV_AA, 0);
  				merge_segments(cs, ci, segments, n_segments);
  				imshow("check", ci);

  				cout<<1000-vanish_row<<endl;
  				waitKey(2000);*/

  				if(vanish_row>-1000 && vanish_row<1000)
  					vanish_row_vote[1000-vanish_row]++;
  			}
  	}

  	int current_votes=0;
  	int max_votes=-1, max_i=-1;
  	for(i=0;i<50;i++)
  		current_votes+=vanish_row_vote[i];

  	for(i=50;i<2000;i++)
  	{
  		current_votes+=vanish_row_vote[i];
  		current_votes-=vanish_row_vote[i-50];

  		if(current_votes>=max_votes)
  		{
  			max_votes=current_votes;
  			max_i=i;
  		}

  		//cout<<i<<" "<<current_votes<<endl;
  	}

  	int vanish_row=max_i-25;

  	cout<<"Vanishing row: "<<vanish_row<<" with votes: "<<max_votes<<endl;

  	Mat output(1200, 1000, CV_8UC3, Scalar(0));
  	line.copyTo(output(cv::Rect(0, 200, 1000, 1000)));
  	cv::line( output, Point(0, 1200-vanish_row), Point(1000, 1200-vanish_row), Scalar(255,0,0), 10, CV_AA, 0);
  	//line( output, Point(0, 1000-vanish_row), Point(1000, 1000-vanish_row), Scalar(255,0,0), 10, CV_AA);


  	imshow("output", output);

  	Mat lanes(1000, 1000, CV_8UC3, Scalar(0));
  	Mat lanes_segments[n_segments];
  	extract_segments(lanes_segments, lanes, segments, n_segments);

  	cum_sum=1000;
  	for(i=4;i>=2;i--)
  	{
  		cum_sum-=segments[i];
  		for(j=0;j<lines[i].size();j++)
  			for(k=0;k<lines[i].size();k++)
  			{
  				if(j==k)
  					continue;

  				int vanishRow=find_intersection(lines[i][j], lines[i][k])+cum_sum;

  				if(1000-vanishRow>= vanish_row-20 && 1000-vanishRow<= vanish_row+20)
  				{
  					if(i==4)
  						cout<<"yay"<<j<<endl;


  					cv::line( lanes_segments[i], Point(lines[i][j][0], lines[i][j][1]), Point(lines[i][j][2], lines[i][j][3]), Scalar(255,0,0), 3, CV_AA, 0);
  					cv::line( lanes_segments[i], Point(lines[i][k][0], lines[i][k][1]), Point(lines[i][k][2], lines[i][k][3]), Scalar(255,0,0), 3, CV_AA, 0);
  					/*imshow("please", lanes_segments[i]);

  					Mat temp(segments[i], 1000, CV_8UC3, Scalar(0));
  					cv::line( temp, Point(lines[i][j][0], lines[i][j][1]), Point(lines[i][j][2], lines[i][j][3]), Scalar(255,0,0), 3, CV_AA, 0);
  					cv::line( temp, Point(lines[i][k][0], lines[i][k][1]), Point(lines[i][k][2], lines[i][k][3]), Scalar(255,0,0), 3, CV_AA, 0);

  					imshow("verify", temp);

  					waitKey(2000);*/

  				}	
  			}
  	}
  	merge_segments(lanes_segments, lanes, segments, n_segments);

  	for(i=1000-vanish_row;i>=0;i--)
  		for(j=0;j<img.cols;j++)
  			lanes.at<Vec3b>(i, j)={0, 0, 0};

  	for(i=0;i<img.rows;i++)
  		for(j=0;j<img.cols;j++)
  			if(lanes.at<Vec3b>(i, j)[0]==255)
  				img.at<Vec3b>(i, j)={255, 0, 0};


  	imshow("yay", img);
  	imshow("wohoo!", lanes);

	imshow("edges", edges);
	waitKey(0);

	return 0;
}