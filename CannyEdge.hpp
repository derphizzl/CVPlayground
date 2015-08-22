#pragma once

#include "Gradient.hpp"
#include "Filter.hpp"
#include "Helper.hpp"

class canny {
	public:
		/** \brief Canny edge detection
		 * \param input input image
		 * \param output output image (still cv::Mat)
		 * \param lowT low threshold for hysteresis
		 * \param highT high threshold for hysteresis
		 * \param kernelSize size of the gaussian smoothing kernel, MUST be an odd number starting with 3!!
		 * \param alg Algorithm to choose: sobel: Sobel filter kernel (3x3), diffQ: partial derivative using 2 neighbourhood pixels in x and y direction, diffQN: uses an evolved formula for 		the partial derivative */
		static void getCannyEdge(cv::Mat& input, cv::Mat& output, float lowT, float highT, int kernelSize, Algorithm alg) 
		{
			Filter f(input, kernelSize);

			cv::Mat filtered = f.getFilteredImg();
	
			Gradient gr(filtered);
			output = gr.getGradientImg(lowT, highT, alg);
			
			return;
		}
	
		static void showCannyEdgeDetection(int argc, char **argv) 
		{
			if (argc < 2) 
			{
				std::cout << "ERROR: no input file given!! Closing program!" << std::endl;
				return;
			}	
			int kernelSize = 3;
			std::string file = argv[1];
	
			int highT = 2 , lowT = 1;
	
			Algorithm alg = diffQ;
	
			cv::Mat input = cv::imread(file, CV_LOAD_IMAGE_COLOR);	
			if (input.data == NULL) 
			{
				std::cout << "image value --> NULL" << std::endl;
				return;
			}

			cv::Mat grey = Helper::color2grey(input);

			bool flag = true;
	
			for(;;) 
			{
	
				cv::Mat out, rgb;
		
				if (highT < 1)
					highT = 1;
				if (lowT < 1)
					lowT = 1;
				if (lowT >= highT)
					lowT--;
				if(kernelSize < 3)
					kernelSize = 3;
			
		
				canny::getCannyEdge(grey, out, lowT, highT, kernelSize, alg);
				cv::cvtColor(out, rgb, CV_GRAY2RGB);
		
				if (flag) 
				{
					char buffer [50];
					sprintf (buffer, "High Threshold: %d", highT);
		
					char buffer1 [50];
					sprintf (buffer1, "Low Threshold: %d", lowT);

					char buffer2 [50];
					if (alg == Utils::diffQ)
						sprintf (buffer2, "Algorithm: diffQ");
					else if(alg == sobel)
						sprintf (buffer2, "Algorithm: Sobel");
					else if(alg == Utils::diffQN)
						sprintf (buffer2, "Algorithm: diffQN");
		
					char buffer3 [50];
					sprintf (buffer3, "Gaussian kernel size: %d", kernelSize);
		
					cv::Point textOrg(1400, 60);
					cv::Point textOrg1(1400, 100);
					cv::Point textOrg2(1400, 140);
					cv::Point textOrg3(1400, 180);
					double fontScale = 1;
					int thickness = 3;
					int fontFace = cv::FONT_ITALIC;
					cv::putText(rgb, buffer, textOrg, fontFace, fontScale, cv::Scalar(0, 0, 255), thickness,8);
					cv::putText(rgb, buffer1, textOrg1, fontFace, fontScale, cv::Scalar(0, 0, 255), thickness,8);
					cv::putText(rgb, buffer2, textOrg2, fontFace, fontScale, cv::Scalar(0, 0, 255), thickness,8);
					cv::putText(rgb, buffer3, textOrg3, fontFace, fontScale, cv::Scalar(0, 0, 255), thickness,8);
				}
		

		
				cv::imshow("Canny Edge Detection", rgb);
	
				int inp = cv::waitKey(0);
				if (inp == 'q')
					break;
		
				else if (inp == 'p')
					highT++;
	
				else if (inp == 'l')
					highT--;
		
				else if (inp == 'o')
					lowT++;
	
				else if (inp == 'k')
					lowT--;
		
				else if (inp == 's')
					alg = sobel;
		
				else if (inp == 'd')
					alg = diffQ;
				
				else if (inp == 'f')
					alg = diffQN;
		
				else if (inp == 'i')
					kernelSize +=2;
		
				else if (inp == 'j')
					kernelSize -=2;
		
				else if (inp == 'u')
					flag == true;
		
				else if (inp == 'h')
					flag == false;

			}
			return;
		}
	
};