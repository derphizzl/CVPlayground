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