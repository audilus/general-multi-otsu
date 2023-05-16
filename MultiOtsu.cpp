#include "MultiOtsu.h"


void MultiOtsu::findThresholds(cv::InputArray& hist, cv::InputArray& src, int classCount, std::vector<int>& outThresholds, int previousT)
{
	//	Get reference to our data at this point in the stack.
	otsuData* oData = &otsuDataStruct[classCount - 1];
	oData->c = 0;
	oData->m = 0;
	oData->p = 0;
	oData->w = 0;

	//	TODO: cache this.
	float* hPtr = hist.getMat().ptr<float>();

	//	If we are at the last threshold in the stack, do not spawn another.
	if (classCount == 1)
	{
		auto sum = [](const std::vector<otsuData>& v) -> otsuData {
			otsuData ret;
			for (const auto x : v) {
				ret.c += x.c;
				ret.m += x.m;
				ret.p += x.p;
				ret.w += x.w;
			}
			return ret;
		};
		auto odSummed = sum(otsuDataStruct);

		oData->p = 1 - odSummed.p;
		oData->w = Mt - odSummed.w;
		oData->m = oData->w / oData->p;

		oData->c = oData->p * std::pow((oData->m - Mt), 2);

		double newSigma = odSummed.c;
		if (maxVar < newSigma)
		{
			maxVar = newSigma;
			
			for (size_t i = 0; i < threshCount; i++)
			{
				outThresholds[i] = thresholds[i];
				//std::cout << outThresholds[i] << std::endl;
			}
		}
	}
	else
	{
		for (size_t i = previousT; i < 256; i++)
		{
			oData->p += hPtr[i];
			oData->w += (i * hPtr[i]);
			if (oData->p != 0)
			{
				oData->m = oData->w / oData->p;
			}
			
			oData->c = oData->p * std::pow((oData->m - Mt), 2);

			thresholds[classCount - 1] = i;
			findThresholds(hist, src, classCount - 1, outThresholds, i + 1);
		}
		
	}
}

void MultiOtsu::multiOtsu(cv::InputArray& src, cv::OutputArray& dst, int classCount)
{
	cv::Mat greysc, histogram;

	cv::copyTo(src, greysc, src);

	//	Quick fix for class count being one less than intended input
	//	
	classCount++;

	threshCount = classCount;	


	//	Histogram boilerplate
	int histSize[] = { 256 };
	float bRanges[] = { 0.0, 256.0 };
	const float* ranges[] = { bRanges };
	int channels[] = { 0 };

	//	Generate histogram to find classes
	cv::calcHist(&greysc, 1, channels, cv::Mat(), histogram, 1, histSize, ranges);

	//	Populate struct vector
	std::vector<int> finalThresholds;
	for (size_t i = 0; i < classCount; i++)
	{
		otsuDataStruct.push_back(otsuData());
		thresholds.push_back(0);
		thresholdsOptimal.push_back(0);
		finalThresholds.push_back(0);
	}
	
	//	Do first step for Q
	float* hPtr = histogram.ptr<float>();
	for (size_t y = 0; y < histogram.rows; y++)
	{
		Mt += y * hPtr[y];
	}

	//	Recursively find thresholds. 
	//	Important: One threshold is erroneously placed at 0, which is why classCount is incremented at the start of 
	//			   the multiotsu method. Just a quick patch to get my assignment out on time.
	findThresholds(histogram, src, classCount, finalThresholds);

	std::sort(finalThresholds.begin(), finalThresholds.end());


	//	Initialize a black output image.
	cv::Mat outputImage = cv::Mat::zeros(dst.size(), src.type());
	
	//	Assign pixels in the output a value equal to the highest threshold it is under.
	for (size_t y = 0; y < src.rows(); y++)
	{
		auto* srcPtr = src.getMat().ptr<uchar>(y);
		auto* dstPtr = outputImage.ptr<uchar>(y);
		
		for (size_t x = 0; x < src.cols(); x++)
		{
			int i = 0;
			while (srcPtr[x] > finalThresholds[i])
			{
				i++;
			}
			dstPtr[x] = finalThresholds[i];

		}
	}


	outputImage.copyTo(dst);
	//std::cout << "foobar check" << std::endl;
}