

#include "Corners.hpp"
using namespace houghline;

void Corners::displayHoughLines()
{
	cv::VideoCapture cap(0);
	
	cv::Mat dst, dst_norm, dst_norm_scaled, cdst;
	

	for (;;) 
	{
		cv::Mat in, out, gray;
		
		cap >> in;
		if (in.data == NULL)
			continue;
// 		in = cv::imread("Edge1.jpg", CV_LOAD_IMAGE_COLOR);
		
		cv::cvtColor(in, gray, CV_BGR2GRAY);
		
// 		cv::imshow("bla", gray);
// 		char c = cv::waitKey(30);
// 		
// 		if (c == 'q')
// 			break;
// 		if (c == 'd') 
// 		{
			canny::getCannyEdge(gray, dst, 1, 7, 5, Utils::diffQ);
			#if 0
				vector<cv::Vec2f> lines;
				cv::HoughLines(dst, lines, 1, CV_PI/180, 180, 0, 0 );

				for( size_t i = 0; i < lines.size(); i++ )
				{
					float rho = lines[i][0], theta = lines[i][1];
					cv::Point pt1, pt2;
					double a = cos(theta), b = sin(theta);
					double x0 = a*rho, y0 = b*rho;
					pt1.x = cvRound(x0 + 1000*(-b));
					pt1.y = cvRound(y0 + 1000*(a));
					pt2.x = cvRound(x0 - 1000*(-b));
					pt2.y = cvRound(y0 - 1000*(a));
					cv::line( gray, pt1, pt2, cv::Scalar(255,0,0), 3, CV_AA);
				}
// 			#if 0
// 			#else
				vector<cv::Vec4i> lines;
				cv::HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
				for( size_t i = 0; i < lines.size(); i++ )
				{
					cv::Vec4i l = lines[i];
					cv::line( gray, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255,0,0), 3, CV_AA);
				}
			#endif
			
// 			cv::imshow("bla", dst);
// 			cv::waitKey(0);

			HoughLines newL(dst);
			HoughL lines = newL.HoughTransform();
			
			HoughL::iterator it = lines.begin();
			HoughL::iterator end = lines.end();
			
			
			for (; it != end; ++it) 
			{
				cv::line(gray, cv::Point(it->first.first, it->first.second), cv::Point(it->second.first, it->second.second), 255, 1, 0);
			}
						
			
			cv::imshow("bla", gray);
			char c = cv::waitKey(30);
			
			if (c == 'q')
				break;
// 		}
	}
	
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
