#include "Gradient.hpp"

using namespace Utils;

////////////////////////////////////////////////////////////////////////////

Gradient::Gradient(cv::Mat& input)
{
	// set input img for gradient calculation
	this->m_gradientImg = cv::Mat::zeros(input.rows, input.cols, /*CV_64FC1*/CV_8UC1);
	this->m_output = cv::Mat::zeros(input.rows, input.cols, /*CV_64F*/CV_8UC1);
	this->m_binary = cv::Mat::zeros(input.rows, input.cols, /*CV_64F*/CV_8UC1);
	this->m_img = input;
	
	this->m_threshold = createMatrix<Thresh>(this->m_img.rows, this->m_img.cols);
	this->m_gradientParam = createMatrix<Grad>(this->m_img.rows, this->m_img.cols);
	this->m_visited = createMatrix<int>(this->m_img.rows, this->m_img.cols);

	m_counter = 0;
	
	return;
}

////////////////////////////////////////////////////////////////////////////////////////

Gradient::~Gradient()
{

	clearMatrix<Thresh>(this->m_threshold, this->m_img.rows);
	clearMatrix<Grad>(this->m_gradientParam, this->m_img.rows);
	clearMatrix<int>(this->m_visited, this->m_img.rows);
}


////////////////////////////////////////////////////////////////////////////////////////

cv::Mat Gradient::getGradientImg(int lowT, int highT, Algorithm alg) 
{
	m_algo = alg;
	iterateOverImg();
	NonMaximumSuppression();
	Thresholding(lowT, highT);
	Hysteresis();
	generateOutput();

	return this->m_output; // m_output
}

//////////////////////////////////////////////////////////////////////////

void Gradient::calculateGradientValue(Grad& input)
{
	if (isnan(input.val = sqrt(input.dx * input.dx + input.dy * input.dy)))
		input.val = .0;
	if (isinf(input.val))
		input.val = .0;
	
	return;
}

//////////////////////////////////////////////////////////////////////////

Grad Gradient::calculateGradient(int x, int y) 
{
	Grad grad;
	
	diffInX(grad, x, y);
	diffInY(grad, x, y);	
	calculateEdgeDirection(grad);
	calculateGradientValue(grad);
	
	
	m_gradientParam[x][y] = grad;
	
	return grad;	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gradient::calculateEdgeDirection(Grad& grad) 
{
	double num = atan2(grad.dy, grad.dx);
	num *= Helper::rad2deg;
	/*if (m_counter < 200) 
	{
		std::cout << "DEG: " << num << std::endl;
		std::cout << "Dx: " << grad.dx << std::endl;
		std::cout << "Dy: " << grad.dy << std::endl;
	}*/	
	if (num < 22.5 && num > 0) 
	{
		grad.deg = 0;
		return;
	}	
	else if (num < 0 && num > -22.5) 
	{
		grad.deg = 0;
		return;
	}
	else if (num > 157.5) 
	{
		grad.deg = 0;
		return;
	}
	else if (num < -157.5) 
	{
		grad.deg = 0;
		return;
	}
	else if (num < 67.5 && num > 22.5) 
	{
		grad.deg = 45;
		return;
	}
	else if (num < -22.5 && num > -67.5) 
	{
		grad.deg = 45;
		return;
	}
	else if (num < 112.5 && num > 67.5) 
	{
		grad.deg = 90;
		return;
	}
	else if (num < -67.5 && num > -112.5) 
	{
		grad.deg = 90;
		return;
	}
	else if (num < 157.5 && num > 112.5) 
	{
		grad.deg = 135;
		return;
	}
	else if (num < -112.5 && num > -157.5) 
	{
		grad.deg = 135;
		return;
	}
	
	
	
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gradient::diffInX(Grad& in, int x, int y) 
{
	if (x < 1)
		x = 1;
	
	else if (x >= this->m_img.rows - 1)
		x = m_img.rows - 1;
	
	
	if (y < 1)
		y = 1;
	
	else if (y >= this->m_img.cols - 1)
		y = m_img.cols - 1;
	
	if(m_algo == diffQ)
		in.dx = this->m_img.at<uchar>(x + 1, y) - this->m_img.at<uchar>(x, y);
	else if(m_algo == diffQN)
		in.dx = ((this->m_img.at<uchar>(x, y + 1) - this->m_img.at<uchar>(x, y - 1) + this->m_img.at<uchar>(x - 1, y + 1) - this->m_img.at<uchar>(x - 1, y - 1) + 
			this->m_img.at<uchar>(x + 1, y + 1) - this->m_img.at<uchar>(x + 1, y - 1))  / 2);
	else if(m_algo == sobel)
		in.dx = Filter::SobelX(this->m_img, x, y);
	else
		in.dx = this->m_img.at<uchar>(x + 1, y) - this->m_img.at<uchar>(x, y);
	
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gradient::diffInY(Grad& in, int x, int y) 
{
	
	if (x < 1)
		x = 1;
	
	else if (x >= this->m_img.rows - 1)
		x = m_img.rows - 1;
	
	
	if (y < 1)
		y = 1;
	
	else if (y >= this->m_img.cols - 1)
		y = m_img.cols - 1;
	
	if(m_algo == diffQ)
		in.dy = this->m_img.at<uchar>(x, y + 1) - this->m_img.at<uchar>(x, y);
	
	else if(m_algo == diffQ)
		in.dy = ((this->m_img.at<uchar>(x + 1, y) - this->m_img.at<uchar>(x - 1, y) + this->m_img.at<uchar>(x + 1, y - 1) - this->m_img.at<uchar>(x - 1, y - 1) + 
			this->m_img.at<uchar>(x + 1, y + 1) - this->m_img.at<uchar>(x - 1, y + 1))  / 2);
	if(m_algo == sobel)
		in.dy = Filter::SobelY(this->m_img, x, y);
	
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gradient::iterateOverImg()
{	
	// performs the gradient calculation over the whole image
	for(int row = 0; row < this->m_gradientImg.rows; ++row) 
	{
		for(int col = 0; col < this->m_gradientImg.cols; ++col) 
		{
			this->m_gradientImg.at<uchar>(row, col) = abs(calculateGradient(row, col).val);
		}
	}
	
	this->m_testOut = this->m_gradientImg;
	
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Grad Gradient::getGradient(int x, int y)
{
	return calculateGradient(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gradient::NonMaximumSuppression()
{	
	for(int row = 0; row < this->m_gradientImg.rows; ++row) 
	{
		for(int col = 0; col < this->m_gradientImg.cols; ++col) 
		{
			switch (m_gradientParam[row][col].deg) 
			{
				case 0:
					m_gradientParam[row][col].dirX = 0.0;					
					m_gradientParam[row][col].dirY = 0.0f;
					break;
				case 45:
					m_gradientParam[row][col].dirX = 1.0f;
					m_gradientParam[row][col].dirY = -1.0f;
					break;
				case 90:
					m_gradientParam[row][col].dirX  = 0.0f;
					m_gradientParam[row][col].dirY = -1.0f;
					break;
				case 135:
					m_gradientParam[row][col].dirX = -1.0f;
					m_gradientParam[row][col].dirY = -1.0f;
					break;
			}
		
			double anker, plusDir, minusDir;
			
			anker = this->m_gradientImg.at<uchar>(row, col);
			plusDir = this->m_gradientImg.at<uchar>(row + m_gradientParam[row][col].dirX, col + m_gradientParam[row][col].dirY);
			minusDir = this->m_gradientImg.at<uchar>(row - m_gradientParam[row][col].dirX, col - m_gradientParam[row][col].dirY);

			if (plusDir > anker || minusDir > anker) 
			{
				this->m_gradientImg.at<uchar>(row, col) = 0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gradient::Thresholding(int lower, int higher) 
{
	
	for(int row = 0; row < this->m_gradientImg.rows; ++row) 
	{
		for(int col = 0; col < this->m_gradientImg.cols; ++col) 
		{
			if ((int) abs(this->m_gradientImg.at<uchar>(row, col)) < lower) 
			{
				this->m_gradientImg.at<uchar>(row, col) = 0.0;
				this->m_threshold[row][col].lowerThresh = 0.0;
				this->m_threshold[row][col].higherThresh = 0.0;
			}	
			else if ((int) abs(this->m_gradientImg.at<uchar>(row, col)) >= lower && (double) abs(this->m_gradientImg.at<uchar>(row, col)) < higher)
			{
				this->m_threshold[row][col].lowerThresh = (double) abs(this->m_gradientImg.at<uchar>(row, col));
				this->m_threshold[row][col].higherThresh = 0.0;
			}
			else if ((int) abs(this->m_gradientImg.at<uchar>(row, col)) >= higher)
			{
				this->m_threshold[row][col].lowerThresh = 0.0;
				this->m_threshold[row][col].higherThresh = (double) abs(this->m_gradientImg.at<uchar>(row, col));
			}			
		}
	}
	
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gradient::generateOutput()
{		
	this->m_output = this->m_binary;
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gradient::Hysteresis() 
{
	//initialize m_visited with 0
	for(int row = 0; row < this->m_gradientImg.rows; ++row) 
	{
		for(int col = 0; col < this->m_gradientImg.cols; ++col) 
		{			
			this->m_visited[row][col] = 0;
		}
	}
	
	
	for(int row = 0; row < this->m_gradientImg.rows; ++row) 
	{
		for(int col = 0; col < this->m_gradientImg.cols; ++col) 
		{			
			if (this->m_threshold[row][col].higherThresh > 0.0)
			{
				Travers(row, col);
				this->m_visited[row][col] = 1;
				this->m_binary.at<uchar>(row, col) = 255;
				
			}
		}
	}
	
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gradient::Travers(int x, int y) 
{
	
	// follows the edges and connects them
	// function works recursively
	
	if (x < 1)
		return;
	if (y < 1)
		return;
	if (x >= this->m_binary.rows - 1)
		return;
	if (y >= this->m_binary.cols - 1)
		return;
	
	if (m_visited[x][y] == 1)
		return;
	
	if (this->m_visited[x + 1][y] < 1 && this->m_threshold[x + 1][y].lowerThresh > 0.0 && this->m_gradientParam[x + 1][y].dirX == this->m_gradientParam[x][y].dirX && 
	    this->m_gradientParam[x + 1][y].dirY == this->m_gradientParam[x][y].dirY) 
	{
// 		this->m_visited[x][y] = 1;
		this->m_visited[x + 1][y] = 1;
// 		this->m_binary.at<uchar>(x, y) = 255;
		this->m_binary.at<uchar>(x + 1, y) = 255;
		Travers(x + 1, y);
		return;
		
	}
	
	else if (this->m_visited[x][y + 1] < 1 && this->m_threshold[x][y + 1].lowerThresh > 0.0 && this->m_gradientParam[x][y  + 1 ].dirX == this->m_gradientParam[x][y].dirX &&
		this->m_gradientParam[x][y + 1].dirY == this->m_gradientParam[x][y].dirY) 
	{
// 		this->m_visited[x][y] = 1;
		this->m_visited[x][y  + 1] = 1;
// 		this->m_binary.at<uchar>(x, y) = 255;
		this->m_binary.at<uchar>(x, y  + 1) = 255;
		Travers(x, y  + 1);
		return;
		
	}
	
	else if (this->m_visited[x - 1][y] < 1 && this->m_threshold[x - 1][y].lowerThresh > 0.0 && this->m_gradientParam[x - 1][y].dirX == this->m_gradientParam[x][y].dirX &&
		this->m_gradientParam[x - 1][y].dirY == this->m_gradientParam[x][y].dirY) 
	{
// 		this->m_visited[x][y] = 1;
		this->m_visited[x - 1][y] = 1;
// 		this->m_binary.at<uchar>(x, y) = 255;
		this->m_binary.at<uchar>(x - 1, y) = 255;
		Travers(x - 1, y);
		return;
		
	}
	
	else if (this->m_visited[x][y - 1] < 1 && this->m_threshold[x][y - 1].lowerThresh > 0.0 && this->m_gradientParam[x][y - 1].dirX == this->m_gradientParam[x][y].dirX &&
		this->m_gradientParam[x][y - 1].dirY == this->m_gradientParam[x][y].dirY) 
	{
// 		this->m_visited[x][y] = 1;
		this->m_visited[x][y - 1] = 1;
// 		this->m_binary.at<uchar>(x, y) = 255;
		this->m_binary.at<uchar>(x, y - 1) = 255;
		Travers(x, y - 1);
		return;
		
	}
	
	else if (this->m_visited[x - 1][y - 1] < 1 && this->m_threshold[x - 1][y - 1].lowerThresh > 0.0 && this->m_gradientParam[x - 1][y - 1].dirX == this->m_gradientParam[x][y].dirX &&
		this->m_gradientParam[x - 1][y - 1].dirY == this->m_gradientParam[x][y].dirY) 
	{
// 		this->m_visited[x][y] = 1;
		this->m_visited[x - 1][y - 1] = 1;
// 		this->m_binary.at<uchar>(x, y) = 255;
		this->m_binary.at<uchar>(x - 1, y - 1) = 255;
		Travers(x - 1, y - 1);
		return;
		
	}
	
	else if (this->m_visited[x + 1][y - 1] < 1 && this->m_threshold[x + 1][y - 1].lowerThresh > 0.0 && this->m_gradientParam[x + 1][y - 1].dirX == this->m_gradientParam[x][y].dirX &&
		this->m_gradientParam[x + 1][y - 1].dirY == this->m_gradientParam[x][y].dirY) 
	{
// 		this->m_visited[x][y] = 1;
		this->m_visited[x + 1][y - 1] = 1;
// 		this->m_binary.at<uchar>(x, y) = 255;
		this->m_binary.at<uchar>(x + 1, y - 1) = 255;
		Travers(x + 1, y - 1);
		return;
		
	}
	
	else if (this->m_visited[x - 1][y + 1] < 1 && this->m_threshold[x - 1][y + 1].lowerThresh > 0.0 && this->m_gradientParam[x - 1][y + 1].dirX == this->m_gradientParam[x][y].dirX &&
		this->m_gradientParam[x - 1][y + 1].dirY == this->m_gradientParam[x][y].dirY) 
	{
		this->m_visited[x][y] = 1;
		this->m_visited[x - 1][y + 1] = 1;
// 		this->m_binary.at<uchar>(x, y) = 255;
		this->m_binary.at<uchar>(x - 1, y + 1) = 255;
		Travers(x - 1, y + 1);
		return;
		
	}
	
	else if (this->m_visited[x + 1][y + 1] < 1 && this->m_threshold[x + 1][y + 1].lowerThresh > 0.0 && this->m_gradientParam[x + 1][y + 1].dirX == this->m_gradientParam[x][y].dirX &&
		this->m_gradientParam[x + 1][y + 1].dirY == this->m_gradientParam[x][y].dirY) 
	{
		this->m_visited[x][y] = 1;
		this->m_visited[x + 1][y + 1] = 1;
// 		this->m_binary.at<uchar>(x, y) = 255;
		this->m_binary.at<uchar>(x + 1, y + 1) = 255;
		Travers(x + 1, y + 1);
		return;
		
	}
	
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////