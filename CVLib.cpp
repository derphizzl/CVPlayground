#include "CVLib.h"

void getCannyEdges(cv::Mat& input, cv::Mat& output, float lowT, float highT, int kernelSize, Algorithm alg)
{
	canny::getCannyEdge(input, output, lowT, highT, kernelSize, alg);
	
	return;
}
