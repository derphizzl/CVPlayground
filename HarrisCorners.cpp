#include "HarrisCorners.hpp"

Harris::Harris(cv::Mat input)
{
	this->m_input = input.clone();
}

//////////////////////////////////////////////////////////////////////////////////////////

Harris::~Harris()
{

}

//////////////////////////////////////////////////////////////////////////////////////////

H_Corners Harris::getHarrisCorners()
{
	Gradient g(m_input);
	for (uint i = 0; i < this->m_input.rows; ++i) 
	{
		for (uint j = 0; j < this->m_input.cols; ++j) 
		{
			
			Grad gr;
			DProd dp;
			gr = g.getGradient(i, j);
			
			
			computeProductOfDerivatives(dp, gr);
		
			computeSumsOfProducts(dp, gr);
			m_response.push_back(computeDetectorResponse(dp));
		}
		if (i % 10 == 0)
		{
			std::cout << "row" << i << std::endl;
		}	
	}
	
	Thresholding();
	
	return getOutput();
}

//////////////////////////////////////////////////////////////////////////////////////////

void Harris::computeProductOfDerivatives(DProd& dp, Grad& gr)
{
	dp.Ix = gr.dx * gr.dx;
	dp.Iy = gr.dy * gr.dy;
	dp.Ixy = gr.dx * gr.dy;
	
}

//////////////////////////////////////////////////////////////////////////////////////////

void Harris::computeSumsOfProducts(DProd& sumProd, Grad& gr)
{
	sumProd.Sx = gr.val * sumProd.Ix;
	sumProd.Sy = gr.val * sumProd.Iy;
	sumProd.Sxy = gr.val * sumProd.Ixy;
}

///////////////////////////////////////////////////////////////////////////////////////////

double Harris::computeDetectorResponse(DProd& p)
{
	double detH = p.Sx * p.Sy - p.Sxy * p.Sxy;
	double traceH = p.Sxy * p.Sxy;
	
	double response = detH - (0.04 * traceH * traceH);
	
// 	std::cout << "response: " << response << std::endl;
	
	return response;
}

////////////////////////////////////////////////////////////////////////////////////////////

void Harris::setThreshold(double threshold)
{
	this->m_threshold = threshold;
}

////////////////////////////////////////////////////////////////////////////////////////////

void Harris::Thresholding()
{
	auto it = m_response.begin(), end = m_response.end();
	for (; it != end; ++it) 
	{
// 		std::cout << *it << std::endl;
		if(*it > .0 - 100000000000000.0 )
			*it = .0;			
	}
}

////////////////////////////////////////////////////////////////////////////////////////////

H_Corners Harris::getOutput()
{
	H_Corners out;
	auto it = m_response.begin();
	for (uint i = 0; i < m_input.rows; ++i) 
	{
		for (uint j = 0; j < m_input.cols; ++j) 
		{
			cv::Point p;
			if (*it != 0) 
			{
				p.x = j;
				p.y = i;
				out.push_back(p);
			}
			++it;
		}	
	}
	
	return out;		
}

/////////////////////////////////////////////////////////////////////////////////////////////





