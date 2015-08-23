// **********************************************************************************
//
// BSD License.
// This file is part of a Hough Transformation tutorial,
// see: http://www.keymolen.com/2013/05/hough-transformation-c-implementation.html
//
// Copyright (c) 2013, Bruno Keymolen, email: bruno.keymolen@gmail.com
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
// Redistributions in binary form must reproduce the above copyright notice, this
// list of conditions and the following disclaimer in the documentation and/or other
// materials provided with the distribution.
// Neither the name of "Bruno Keymolen" nor the names of its contributors may be
// used to endorse or promote products derived from this software without specific
// prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// **********************************************************************************

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
		cv::Mat getGradientImg(int lowT, int highT, Algorithm alg);
		/**
		 * \brief returns Gradient vector and value
		 * \param xy pixel coordinates */
		Grad getGradient(int x, int y);
		/**
		 * \brief calculates gradient of input and returns the non maxima suppressed gradient img */
		cv::Mat NonMaxSuppression(Utils::Algorithm alg);
	
	private:
		cv::Mat m_img;
		cv::Mat m_gradientImg;
		Grad** m_gradientParam;
		Thresh** m_threshold;
		cv::Mat m_output;
		cv::Mat m_binary;
		cv::Mat m_testOut;
		int** m_visited;
		Algorithm m_algo;
		
		int m_counter;
		

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