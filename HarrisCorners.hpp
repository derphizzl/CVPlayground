#pragma once

#include "inc/Gradient.hpp"

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
	std::vector<int> m_response;
	std::vector<int> m_response_out;
	long** m_mat;
	Grad** m_grMat;
	
	
	int computeDetectorResponse(DProd& d);
	void computeProductOfDerivatives(DProd& sum, Grad& gr);
	void computeSumsOfProducts(DProd& sumProd, Grad& gr);
	void Thresholding();
	void NMS();
	H_Corners getOutput();
	
	
	// debug variables //
	Grad m_grad;
	long m_Ix, m_Iy, m_Ixy;
	long m_Sx, m_Sy, m_Sxy;
	long m_detH, m_traceH, m_resp;
	
	void getDebugValues();
	

};