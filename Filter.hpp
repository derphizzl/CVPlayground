#pragma once

#include "iostream"
#include "vector"
#include "math.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <cstdarg>
#include <stdarg.h>
#include "Helper.hpp"

using namespace Utils;

namespace filter
{
	
	class Filter 
	{
	private:
		cv::Mat m_input;
		cv::Mat m_output;
		int m_kernelSize;
		FilterKernel inputKernel;
		double m_FilterSum;
		
		
		void iterateOverImg();
		void executeConvolution(int row, int col);
		void edgeLinking(uint x, uint y);
		void calcFilterSum();
		
		
		
	public: 
		Filter(cv::Mat in, uint size);
		~Filter();
		
		void setInputKernel(FilterKernel kern, uint size);
		cv::Mat getFilteredImg();
		static double SobelX(cv::Mat& input, uint row, uint col);
		static double SobelY(cv::Mat& input, uint row, uint col);
		
		
		void gaussian();
		
	};
}