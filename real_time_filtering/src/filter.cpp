#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;
int greyscale(cv::Mat& src, cv::Mat& dst) {
	vector<Mat> bgr_channels(3);
	split(src, bgr_channels);

	vector<Mat> channels;

	channels.push_back(bgr_channels[1]); 
	channels.push_back(bgr_channels[1]); 
	channels.push_back(bgr_channels[1]); 

	merge(channels, dst);

	return 0;

}

string type2str(int type) {
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}

int blur5x5(cv::Mat& src, cv::Mat& dst) {

	/*src.copyTo(dst);
	int channels = dst.channels();

	int nRows = dst.rows;
	int nCols = dst.cols * channels;

	if (dst.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j, k;
	uchar* p;
	for (i = 2*channels; i < nRows-2 * channels; ++i)
	{
		p = dst.ptr<uchar>(i);
		for (j = 2 * channels; j < nCols-2 * channels; ++j)
		{
			for (k = 0; k < channels; k++)
			{

			}
		}
	}*/




	float data[5] = { 1,2,4,2,1 };
	Mat filter = Mat(1, 5, CV_32FC1, data);
	Mat filter_t;
	transpose(filter, filter_t);
	Mat whole_filt = filter_t * filter;
	cout << whole_filt << "\n";
	Scalar denom = sum(whole_filt)[0];
	cout << denom << "\n";
	src.copyTo(dst);
	for (int j = 2; j < src.rows - 2; j++) {
		for (int i = 2; i < src.cols - 2; i++) {

			Mat temp_mat = Mat(src,Rect(i-2, j-2, 5, 5));
			Mat diff_channels[3];
			split(temp_mat, diff_channels);
			Mat b = diff_channels[0];
			b.convertTo(b, CV_32F);
			b = filter/10 * b;
			b = filter_t/10 * b;
			//b.convertTo(b, CV_8UC1);

			Mat g = diff_channels[1];
			g.convertTo(g, CV_32F);
			g = filter * g;
			g = filter_t * g;

			Mat r = diff_channels[2];
			r.convertTo(r, CV_32F);
			r = filter * r;
			r = filter_t * r;
			
			/*Mat fin;
			diff_channels[0] = b;
			diff_channels[1] = g;
			diff_channels[2] = r;
			vector<Mat> channels;
			channels.push_back(diff_channels[0]);
			channels.push_back(diff_channels[1]);
			channels.push_back(diff_channels[2]);
			merge(channels, fin);*/
			//Scalar sum_channels = sum(fin);
			double sum_b =sum(b)[0]/255.0;
			double sum_g = sum(g)[0] / 255.0;
			double sum_r = sum(r)[0] / 255.0;
			cout << "sum b" << sum_b << "\n";
			vector<double> res = { sum_b, sum_g, sum_r };
			//Vec3b fin = Vec3b(sum(b)[0], sum(g)[0] , sum(r)[0]);
			//Mat fin = Mat(1, 1, CV_32FC3, final_data);
			//cout << sum(b)[0] << "\n";
			//cout << type2str(diff_channels[0].type()) << "\n";
			//cout << type2str(filter_t.type()) << "\n";
			/*cout << fin << "\n";*/
			dst.at<vector<double>>(i, j) = res;
		}
	}

	return 0;
}