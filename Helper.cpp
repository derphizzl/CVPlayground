#include "Helper.hpp"

using namespace Utils;

///////////////////////////////////////////////////////////////////////////////////////////////

void Helper::printImage(cv::Mat img, const string windowName, int colormap)
{
	cv::Mat color;
// 	cv::applyColorMap(img, color, colormap);
	cv::imshow(windowName, img);
	cv::waitKey(0);
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////

cv::Mat Helper::color2grey(cv::Mat input)
{
	cv::Mat out;
	cvtColor(input, out, cv::COLOR_BGR2GRAY);
	return out;
}
