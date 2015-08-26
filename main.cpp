#include <iostream>
#include "HarrisCorners.hpp"

int main(int argc, char **argv) {
    
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) 
	{
		std::cout << "Failed initializing camera" << std::endl;
		return -1;
	}
	
	int kernelSize = 5;
	
	
	for (;;) 
	{
		cv::Mat in, fl, grey;;
		cap >> in;
		if (in.data == NULL)
			continue;
		
		cv::flip(in, fl, 1);
		cv::cvtColor(fl, grey, CV_BGR2GRAY);
		
		cv::imshow("xXx", fl);
		char c = cv::waitKey(30);
		if (c == 'q')
			break;
		if(c == 'd') 
		{
			Filter f(grey, kernelSize);

			cv::Mat filtered = f.getFilteredImg();
			
			Harris h(filtered);
			h.setThreshold(1000000000.0);
			H_Corners hc = h.getHarrisCorners();
			
			H_Corners::iterator it = hc.begin(), end = hc.end();
			for (; it != end; ++it) 
			{
				cv::circle( fl, *it, 4, cv::Scalar( 255, 0, 0 ), 2, 8 );
			}	
					
			cv::imshow("Harris Corners", fl);
			c = cv::waitKey(0);
			if (c == 'q')
				break;
			else
				cv::destroyWindow("Harris Corners");
		}
	}
	
	
	
	
    return 0;
}
