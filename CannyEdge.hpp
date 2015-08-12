#pragma once

#include "Gradient.hpp"
#include "Filter.hpp"
#include "Helper.hpp"

class canny {
	public:
		static void getCannyEdge(cv::Mat& input, cv::Mat& output, int lowT, int highT, int kernelSize) 
		{
			Filter f(input, kernelSize);

			cv::Mat filtered = f.getFilteredImg();
	
			Gradient gr(filtered);
			output = gr.getGradientImg(2, 6);
			
			return;
		}
	
};