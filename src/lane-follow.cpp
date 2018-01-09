/*
 * Lane Following
 * lane-follow.cpp
 * Purpose: Computes road lane markings
 *
 * @author Kenneth Jung (kennyjung91@gmail.com)
 * @version 1.0
 *
 * Notes for the Algorithm:
 * If I had more time, I would break drawRoadLine() into smaller functions to make it more readable and cleaner.
 * Normalizing the image may aide in decreasing the effects of illumination, but that is not currently
 * implemented.
 *
 */

#include <opencv2/core/utility.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;


// Algorithm parameters
double white_thres = 0.9;
int kernel_size = 3;
double canny_ratio = 3;
double low_canny = 50;
double w_perc = 0.20;
double h_perc = 0.6;
double min_line_dist = 100;
double max_line_dist = 200;

// Input an image and draws detected road lines on the image.
int drawRoadLine(Mat img_init)
{

	// Get height and width dimensions of the image.
	Mat img_bw,img_yellow, img_white;
	int h = img_init.rows;
	int w = img_init.cols;

	cvtColor(img_init, img_bw, COLOR_BGR2GRAY);

	// Create color filters for yellow and white color in the image to detect lane markings.
	inRange(img_init,Scalar(white_thres*255, white_thres*255, white_thres*255),Scalar(255, 255, 255), img_white);
	inRange(img_init,Scalar(0, 200, 200), Scalar(160, 255, 255), img_yellow);

	// Merge both white and yellow filters together, applying them to the black and white image.
	Mat lane_color_mask, canny_edge_lines;
	bitwise_or(img_white, img_yellow, lane_color_mask);
	bitwise_and(img_bw, lane_color_mask, lane_color_mask);

	// Reduce noise of the filtered image by smoothing using a Gaussian Blur.
	GaussianBlur(lane_color_mask,lane_color_mask,Size(7, 7),0,0);

	// Detect edges using a Canny detector.
	Canny(lane_color_mask, canny_edge_lines, low_canny, canny_ratio*low_canny, kernel_size);


	// Create a rectangle representing the region of interest of the lanes (crops background scenery out).
	// Apply this RoI to the Canny-ed image.
	Mat roi = Mat(h,w, CV_8U, double(0));
	Point_<int> rect[1][4];
	int npt[] = { 4 };
	rect[0][0] = Point(int(w_perc*w),int(h_perc*h));
	rect[0][1] = Point(int(w_perc*w),h);
	rect[0][2] = Point(int((1-w_perc)*w),h);
	rect[0][3] = Point(int((1-w_perc)*w),int(h_perc*h));
	const Point* ppt[1] = {rect[0]};
	fillPoly(roi,ppt,npt,1,Scalar(255));
	bitwise_and(roi, canny_edge_lines, canny_edge_lines);

	// Perform a Hough Transform to pick out lines.
	vector<Vec4i> lines;
	HoughLinesP(canny_edge_lines, lines, 2, CV_PI/180, 20, min_line_dist, max_line_dist);

	// Draw lines on screen.
	for( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		line( img_init, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,230), 3, CV_AA);
	}


	// Display screens.
	imshow("Canny", canny_edge_lines);
	imshow("Gaussian Blur", lane_color_mask);
	imshow("Line Detection", img_init);

	if(waitKey(1) == ' ')
	{
		return 1;
	}

	return 0;

}


int main (int argc, char* argv[])
{
	Mat frame;

	if (argc < 2)
	{
		cout << "Please specify path to video (i.e. ./lane-follow <path>)." << endl;
		return 0;
	}

	string filename = argv[1];
	VideoCapture cap(filename);

	while(1)
	{
		cap >> frame;
		if(frame.empty())
		{
			break;
		}

		if (drawRoadLine(frame))
		{
			break;
		}
	}

	return 0;
}
