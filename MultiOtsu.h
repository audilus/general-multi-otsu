#pragma once
#include <iostream>	//remove after debugging
#include <algorithm>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

//	Recursive implementation of the Otsu binarization algorithm
//	Inspired by natan337 on StackOverflow
class MultiOtsu
{
public:
	struct otsuData
	{
		double w = 0;
		double m = 0;
		double c = 0;
		double p = 0;

	};

	void findThresholds(cv::InputArray& hist, cv::InputArray& src, int thresholdCount, std::vector<int>& outThresholds, int previousT = 0);

	void multiOtsu(cv::InputArray& src, cv::OutputArray& dst, int tCount);
	
private:
	std::vector<int> thresholds;
	std::vector<int> thresholdsOptimal;
	std::vector<otsuData> otsuDataStruct;


	double Mt;
	double maxVar = 0;

	int threshCount;


};

