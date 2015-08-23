

#include <iostream>
#include <cstdlib>
#include <memory>

#include "HoughLines.hpp"

using namespace std;
using namespace houghline;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                
HoughL HoughLines::HoughTransform()
{
	

	// perform the transform	
	for (int y = 0; y < this->m_h; y++) 
	{
		for (int x = 0; x < this->m_w; x++) 
		{
			if (this->m_input.at<uchar>(y, x) > 254) 
			{
				for (int deg = 0; deg < m_accu_w; deg++) 
				{
					double r = (((double) x - m_center_x) * cos((double) deg * Helper::deg2rad)) + (((double) y - m_center_y) * sin((double) deg * Helper::deg2rad));
				
					int h = (int) round(r + ((double)(m_accu_h / 2)));
					m_Accumulator[h][deg]++;
				}	
			}
		}
	}
	
	// display the Hough Space
// 	cv::Mat testOut = cv::Mat::zeros(m_accu_h, m_accu_w, CV_8UC1);
// 	for (uint i = 0; i < m_accu_h; ++i) 
// 	{
// 		for (uint j = 0; j < m_accu_w; ++j) 
// 		{
// 			testOut.at<uchar>(i, j) = m_Accumulator[i][j];
// 		}
// 	}
// 	
// 	cv::imshow("bla", testOut);
// 	cv::waitKey(0);
	
	//search for peaks
	return HoughPeaks(m_Accumulator);

}

 
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
HoughL HoughLines::HoughPeaks(int** Accumulator)
{
	
	HoughL lines;
	
	NMS(Accumulator);
	
	// display the Hough Space
	cv::Mat testOut = cv::Mat::zeros(m_accu_h, m_accu_w, CV_8UC1);
	for (uint i = 0; i < m_accu_h; ++i) 
	{
		for (uint j = 0; j < m_accu_w; ++j) 
		{
			testOut.at<uchar>(i, j) = m_Accumulator[i][j];
		}
	}
	
// 	cv::imshow("bla", testOut);
// 	cv::waitKey(0);
	
	for (int r = 0; r < m_accu_h; r++) 
	{
		for (int t = 0; t < m_accu_w; t++) 
		{
			if (m_Accumulator[r][t] > 118) 
			{	
				int max = m_Accumulator[r][t];
				for (int lx = -5; lx <= 5; ++lx) 
				{
					for (int ly = -5; ly <= 5; ++ly) 
					{
						if ((lx + r >= 0) && (lx + r < m_accu_h) && (ly + t >= 0) && (ly + t < m_accu_w)) 
						{
							if (m_Accumulator[r + lx][t + ly] > max) 
							{
								max = m_Accumulator[r + lx][t + ly];
								ly = lx = 6;
							}
						}
					}
				}
				
				if (max > m_Accumulator[r][t])
					continue;
				
				int x1, y1, x2, y2;
				x1 = y1 = x2 = y2 = 0;
				
				if(t >= 45 && t <= 135)
// 				if (r < 45 || r > 135)
				{
					x1 = 0;
					y1 = ((double)(r-(m_accu_h/2)) - ((x1 - (m_w/2) ) * cos((t) * Helper::deg2rad))) / sin((t) * Helper::deg2rad) + (m_h/2);
					x2 = m_w - 0;
					y2 = ((double)(r-(m_accu_h/2)) - ((x2 - (m_w/2) ) * cos((t)  * Helper::deg2rad))) / sin((t)  * Helper::deg2rad) + (m_h/2);
				}
				else 
				{
					y1 = 0;
					x1 = ((double)(r-(m_accu_h/2)) - ((y1 - (m_h/2) ) * sin((t) * Helper::deg2rad))) / cos((t)  * Helper::deg2rad) + (m_w/2);
					y2 = m_h - 0;
					x2 = ((double)(r-(m_accu_h/2)) - ((y2 - (m_h/2) ) * sin((t) * Helper::deg2rad))) / cos((t)  * Helper::deg2rad) + (m_w/2);
				}
				
				std::pair<int, int> one, two;
				std::pair<std::pair<int, int>, std::pair<int, int> > together;
				one.first = x1;
				one.second = y1;
				two.first = x2;
				two.second = y2;
				together.first = one;
				together.second = two;
				lines.push_back(together);
				
			}
		}
	}
	std::cout << "lines: " << lines.size() << std::endl;  
	return lines;
}


 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 int** HoughLines::getAccumulator() 
 {
	 return m_Accumulator;
 }

 
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
void HoughLines::NMS(int** Accum)
{
	cv::Mat in = cv::Mat::zeros(m_accu_h, m_accu_w, CV_8UC1);
	
	for (uint x = 0; x < m_accu_h; ++x) 
	{
		for (uint y = 0; y < m_accu_w; ++y) 
		{
			in.at<uchar>(x, y) = Accum[x][y];
		}
	}
	
	Gradient grad(in);
	cv::Mat out = grad.NonMaxSuppression(Utils::diffQ);
	
	for (uint x = 0; x < m_accu_h; ++x) 
	{
		for (uint y = 0; y < m_accu_w; ++y) 
		{
			Accum[x][y] = (int) out.at<uchar>(x, y);
		}
	}
	
	return;
}

 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


