#include "HarrisCorners.hpp"

Harris::Harris(cv::Mat input)
{
	this->m_input = input.clone();
	m_grMat = createMatrix<Grad>(m_input.rows, m_input.cols);
	m_mat = createMatrix<long>(m_input.rows, m_input.cols);
	
	for (uint i = 0; i < m_input.rows; ++i) 
	{
		for (uint j = 0; j < m_input.cols; ++j) 
		{
			this->m_mat[i][j] = 0;
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////

Harris::~Harris()
{
	clearMatrix<Grad>(m_grMat, m_input.rows);
	clearMatrix<long>(m_mat, m_input.rows);
}

//////////////////////////////////////////////////////////////////////////////////////////

H_Corners Harris::getHarrisCorners()
{
	Gradient g(m_input);
	
	
	
	g.setAlgorithm(Utils::diffQ);
	for (uint i = 0; i < this->m_input.rows; ++i) 
	{
		for (uint j = 0; j < this->m_input.cols; ++j) 
		{
			
			Grad gr;
			DProd dp;
			gr = g.getGradient(i, j);
			m_grMat[i][j] = gr;
			m_grad = gr;
			computeProductOfDerivatives(dp, gr);
		
			computeSumsOfProducts(dp, gr);
			int r = computeDetectorResponse(dp);
			m_response.push_back(r);
// 			if (i > 10 && j > 10)
// 				std::cout << "Row: " << i << "\tCol: " << j << std::endl;
			
		}
	}
	
	
	
	Thresholding();
	NMS();
	
	getDebugValues();
	
	return getOutput();
}

//////////////////////////////////////////////////////////////////////////////////////////

void Harris::computeProductOfDerivatives(DProd& dp, Grad& gr)
{
	dp.Ix = gr.dx * gr.dx;
	m_Ix = 0;
	m_Ix = (long) round(dp.Ix);
	
	dp.Iy = gr.dy * gr.dy;
	m_Iy = 0;
	m_Iy = (long) round(dp.Iy);
	
	dp.Ixy = gr.dx * gr.dy;
	m_Ixy = 0;
	m_Ixy = (long) round(dp.Ixy);
	
}

//////////////////////////////////////////////////////////////////////////////////////////

void Harris::computeSumsOfProducts(DProd& sumProd, Grad& gr)
{
	sumProd.Sx = gr.val * sumProd.Ix;
	m_Sx = 0;
	m_Sx = (long) round(sumProd.Sx);
	
	sumProd.Sy = gr.val * sumProd.Iy;
	m_Sy = 0;
	m_Sy = (long) round(sumProd.Sy);
	
	sumProd.Sxy = gr.val * sumProd.Ixy;
	m_Sxy = 0;
	m_Sxy = (long) round(sumProd.Sxy);
}

///////////////////////////////////////////////////////////////////////////////////////////

int Harris::computeDetectorResponse(DProd& p)
{
// 	double detH = (p.Sx * p.Sy) - (p.Sxy * p.Sxy) < -1000000000000.0 ? .0 : (p.Sx * p.Sy) - (p.Sxy * p.Sxy);
	m_detH = 0;
	m_detH = (m_Sx * m_Sy) - (m_Sxy * m_Sxy);
// 	std::cout << "det: " << detH << std::endl;
// 	double traceH = (p.Sxy * p.Sxy) < -1000000000000 ? .0 : (p.Sxy * p.Sxy);
	m_traceH = 0;
	m_traceH = m_Sxy * m_Sxy;	
// 	std::cout << "trace: " << traceH << std::endl;
	
	long response = 0;
	if (m_traceH != 0 || m_detH != 0) 
	{
		response = (long) round((double) m_detH - (0.004 * (double) m_traceH * (double) m_traceH));
		m_resp = response;
	}	
	
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
		if(*it > m_threshold)
			*it = 0;			
	}
}

////////////////////////////////////////////////////////////////////////////////////////////

H_Corners Harris::getOutput()
{
	H_Corners out;
	
	auto it = m_response_out.begin();
	
	
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

void Harris::NMS()
{
	
	auto it = m_response.begin();
		
	for (uint x = 0; x < m_input.rows; ++x) 
	{
		for (uint y = 0; y < m_input.cols; ++y) 
		{
			m_mat[x][y] = (long) round(*it);
			++it;
		}
	}
	
	for (uint x = 0; x < m_input.rows; ++x) 
	{
		for (uint y = 0; y < m_input.cols; ++y) 
		{
			
			long max = m_mat[x][y];
			for (int lx = -4; lx <= 4; ++lx) 
			{
				for (int ly = -4; ly <= 4; ++ly) 
				{
					if ((lx + x >= 0) && (lx + x < m_input.rows) && (ly + y >= 0) && (ly + y < m_input.cols)) 
					{
						if (m_mat[x + lx][y + ly] > max) 
						{
							max = m_mat[x + lx][y + ly];
							ly = lx = 6;
						}
						
					}
				}
			}

			this->m_response_out.push_back(max);
		}
	}
	
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////


void Harris::getDebugValues() 
{
// 	std::sort(m_response.begin(), m_response.end());
	auto it = m_response_out.begin(), end = m_response_out.end();
	long min = 100000000000;
	long max = -100000000000;
	int i = 0;
	for (; it != end; ++it) 
	{
		if (*it < min)
			min = *it;
		if (*it > max)
			max = *it;

// 		std::cout << "Nr: " << i << "\tVal: " << *it << std::endl;

		++i;
	}
	
	std::cout << "Length: " << m_response_out.size() << std::endl;
	std::cout << "Max: " << max << std::endl;
	std::cout << "Min: " << min << std::endl;

}

