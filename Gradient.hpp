#pragma once

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <exception>
#include "Helper.hpp"
#include <iostream>
#include "Filter.hpp"

using namespace Utils;
using namespace filter;

class Gradient 
{
	public:
		Gradient(cv::Mat& input);
		~Gradient();
		
		/**
		 * \brief returns cv::Mat gradient value image
		 * \param threshold sets threshold for b/w values, 0 is no threshold */
		cv::Mat getGradientImg(int lowT, int highT);
		/**
		 * \brief returns Gradient vector and value
		 * \param xy pixel coordinates */
		Grad getGradient(int x, int y);
	
	private:
		cv::Mat m_img;
		cv::Mat m_gradientImg;
		Grad** m_gradientParam;
		Thresh** m_threshold;
		cv::Mat m_output;
		cv::Mat m_binary;
		cv::Mat m_testOut;
		int** m_visited;

		void diffInX(Grad& in, int x, int y);
		void diffInY(Grad& in, int x, int y); 
		void calculateGradientValue(Grad& input);
		Grad calculateGradient(int x, int y);
		void iterateOverImg();  
		void calculateEdgeDirection(Grad& grad);
		void NonMaximumSuppression();
		void Thresholding(int lower, int higher);
		void generateOutput();
		void Hysteresis();
		void Travers(int x, int y);
};  