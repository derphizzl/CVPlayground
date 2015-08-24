

#include <iostream>
#include "Corners.hpp"
#include "sstream"
#include "opencv2/features2d.hpp"

using namespace filter;
using namespace Utils;


int main (int argc, char** argv) 
{
	cv::VideoCapture cap(0);
	
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
			
			HoughLines newL(edge, 120); // 140
			HoughL lines = newL.HoughTransform();
			
			int acc_size[2];
			newL.getAccumulatorSize(acc_size);
			cv::Mat accu = Helper::Array2D2Mat(newL.getAccumulator(), acc_size[0], acc_size[1]);
			
			HoughL::iterator it = lines.begin();
			HoughL::iterator end = lines.end();
					
			for (; it != end; ++it) 
			{
				cv::line(flip, cv::Point(it->first.first, it->first.second), cv::Point(it->second.first, it->second.second), cv::Scalar(0, 0, 255), 1, 8);
				std::cout << "Line deg: " << atan2((it->second.second - it->first.second), (it->second.first - it->first.first)) * Helper::rad2deg << std::endl;
			}	
			
			cv::imshow("Greyscale image", gray);
			c = cv::waitKey(0);
			cv::imshow("Canny edges", edge);
			c = cv::waitKey(0);
			cv::imshow("Accumulator", accu);
			c = cv::waitKey(0);
			cv::imshow("Lines found", flip);
			c = cv::waitKey(0);

			cv::destroyAllWindows();
			
			if (c == 'q')
				break;
		}
	}
	
	return 0;
	
}


