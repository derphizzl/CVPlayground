#pragma once

#include "/home/phil/Libs/LHT/Corners.hpp"

typedef std::vector<cv::Point> H_Corners;

typedef struct 
{
	double Ix;
	double Iy;
	double Ixy;
	double Sx;
	double Sy;
	double Sxy;
} DProd;




class Harris
{
	
public:
	Harris(cv::Mat input);
	~Harris();
	void setThreshold(double threshold);
	H_Corners getHarrisCorners();
	
	
private:
	cv::Mat m_input;
	double m_threshold;
	std::vector<double> m_response;
	
	
	double computeDetectorResponse(DProd& d);
	void computeProductOfDerivatives(DProd& sum, Grad& gr);
	void computeSumsOfProducts(DProd& sumProd, Grad& gr);
	void Thresholding();
	void NMS(cv::Mat in);
	H_Corners getOutput();
	
};