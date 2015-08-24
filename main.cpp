

#include <iostream>
#include "Corners.hpp"
#include "sstream"
#include "opencv2/features2d.hpp"

using namespace filter;
using namespace Utils;


int main (int argc, char** argv) 
{
	cv::VideoCapture cap(0);
	if(!cap.isOpened())  // check if succeeded
		return -1;
	
	for (;;) 
	{
		cv::Mat in, flip, gray, edge;
	
		cap >> in;
		if (in.data == NULL)
			continue;
		
		cv::flip(in, flip, 1);
		
		cv::imshow("RGB Stream", flip);
		char c = cv::waitKey(30);
		
		if (c == 'q')
			break;
		
		if (c == 'd') 
		{
	
// 			in = cv::imread("stahl.jpg", CV_LOAD_IMAGE_COLOR);
	
			cv::cvtColor(flip, gray, CV_BGR2GRAY);
			
			canny::getCannyEdge(gray, edge, 1, 4, 5, Utils::diffQ);
			
			HoughLines newL(edge, 200); // 120
			HoughL lines = newL.HoughTransform();
			
			int acc_size[2];
			newL.getAccumulatorSize(acc_size);
			cv::Mat accu = Helper::Array2D2Mat(newL.getAccumulator(), acc_size[0], acc_size[1]);
			
			HoughL::iterator it = lines.begin();
			HoughL::iterator end = lines.end();
			
			cv::Mat circ = flip.clone();
			
			for (; it != end; ++it) 
			{
				cv::line(flip, cv::Point(it->first.first, it->first.second), cv::Point(it->second.first, it->second.second), cv::Scalar(0, 0, 255), 1, 8);
// 				std::cout << "Line deg: " << atan2((it->second.second - it->first.second), (it->second.first - it->first.first)) * Helper::rad2deg << std::endl;
			}	
			
			Corners inter;
			vector<cv::Point> p = inter.getLineCrossings(lines);
			vector<cv::Point>::iterator it1 = p.begin(), end1 = p.end();
			
			for (; it1 != end1; ++it1) 
			{
				cv::circle( flip, *it1, 3, cv::Scalar( 255, 0, 0 ), -1, 8 );
			}	
			
			cv::imshow("Greyscale image", gray);
			c = cv::waitKey(0);
			cv::imshow("Canny edges", edge);
			c = cv::waitKey(0);
			cv::imshow("Accumulator", accu);
			c = cv::waitKey(0);
			cv::imshow("Lines found", flip);
			c = cv::waitKey(0);
			cv::imshow("Corners", circ);
			c = cv::waitKey(0);
			
			cv::destroyAllWindows();
			
			if (c == 'q')
				break;
		}
	}
	
	return 0;
	
}


