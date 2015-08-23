

#pragma once

#include "CannyEdge.hpp"

void getCannyEdges(cv::Mat& input, cv::Mat& output, float lowT, float highT, int kernelSize, Algorithm alg);