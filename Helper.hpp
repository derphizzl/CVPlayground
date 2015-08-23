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

#include "iostream"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace std;

namespace Utils {

 typedef struct {
	double dx;
	double dy;
	double val;
	int deg;
	int dirX;
	int dirY;
} Grad;

typedef struct {
	double lowerThresh;
	double higherThresh;
} Thresh;

typedef struct 
{
	double** matrix;
	uint* ankerPosition;
} FilterKernel;

  
  
enum Algorithm {sobel, diffQ, diffQN};

class Helper 
{
	public:
		static const double rad2deg = 180/M_PI;
		static const double deg2rad = M_PI/180;
		static void printImage(cv::Mat img, const string windowName, int colormap);
		static cv::Mat color2grey(cv::Mat input);
		
	private:	
	
};

template<class TYPE> void clearMatrix(TYPE** data, int size) 
{
	for( int i = 0 ; i < size; i++ )
	{
		delete[] data[i]; // delete array within matrix
	}
	
	delete[] data;
}

template<typename TYPE> TYPE** createMatrix(int rows, int cols) 
{
	TYPE** data; 
	data = new TYPE*[rows];
	for (int i = 0; i < rows; ++i) 
	{
		data[i] = new TYPE[cols];
	}

	return data;
}	

}