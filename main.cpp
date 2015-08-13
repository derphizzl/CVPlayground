#include <iostream>
#include "CannyEdge.hpp"
#include "sstream"
// #include "opencv2/opencv.hpp"
// #include "opencv2/highgui.hpp"

using namespace filter;
using namespace Utils;


int main (int argc, char** argv) 
{
// 	std::string file =  argv[1];
// 	std::istringstream iss(argv[2]);
// 	int filtersize;
// 	iss >> filtersize;
	
	
	int filtersize = 3;
	std::string file = "babe.jpg";
	
	//////////////////////////////////////////////////////SINGLE IMAGE INPUT/////////////////////////////////////////////
	
// 	cv::Mat input = cv::imread(file, CV_LOAD_IMAGE_COLOR);
// 	
// 	if (input.data == NULL) 
// 	{
// 		std::cout << "image value --> NULL" << std::endl;
// 		return 0;
// 	}
// 	
// 	cv::Mat img_grey = Helper::color2grey(input);
// 	
// 	std::cout << "Input rows: " << img_grey.rows << std::endl;
// 	std::cout << "Input cols: " << img_grey.cols << std::endl;
	
	//////////////////////////////////////////////////////IMAGE STREAM PROCESSING/////////////////////////////////////////////
	
	cv::VideoCapture cap(0); // open the default camera
	if(!cap.isOpened())  // check if we succeeded
		return -1;

	cv::Mat grey;
	int low, high;
	low = 2;
	high = 12;
	
	Algorithm alg = diffQ;
	
	cv::namedWindow("edges",1);
	for(;;)
	{
		if (high > 255)
			high = 255;
		
		if (low < 0 && high < 0) 
		{
			low = 1;
			high = 2;
		}
		
		if (low >= high)
			low = high -1;
		
		
		
		cv::Mat frame;
		cv::Mat out;
		
		cap >> frame; // get a new frame from camera
		
		cv::cvtColor(frame, grey, CV_BGR2GRAY);
		
		canny::getCannyEdge(grey, out, low, high, 3, alg);
		
		cv::imshow("bla", /*filter_calc*/out);
// 		cv::imshow("Blo", frame);
		char inp = cv::waitKey(30); 
		
		if (inp == 'q')
			break;
		
		if (inp == 'p')
			high++;
		
		if (inp == 'l')
			high--;
		
		if (inp == 'o')
			low++;
		
		if (inp == 'k')
			low--;
		
		if (inp == 's')
			alg = sobel;
		
		if (inp == 'd')
			alg = diffQ;
				
		if (inp == 'f')
			alg = diffQN;
			
	}
	
	//////////////////////////////////////////////////////CANNY/////////////////////////////////////////////
	

// 	cv::Mat out;
// 	canny::getCannyEdge(img_grey, out, 5, 12, 5); //normal diff
// // 	canny::getCannyEdge(img_grey, out, 18, 65, 5);
// 	cv::Mat canny;
// 	cv::Canny(img_grey, canny, 100, 200, filtersize);
// 	Helper::printImage(img_grey, "GREY", 1);	
// 	Helper::printImage(out, "CANNY PHT", 1);
// 	Helper::printImage(canny, "CANNY OPENCV", 1);
	

	
	return 0;
}


