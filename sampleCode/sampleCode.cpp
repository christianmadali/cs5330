#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(void) {

	// Read image in GrayScale mode
	Mat image = imread("boy.jpg", 0);

	// Save grayscale image
	imwrite("boyGray.jpg", image);
	
	imshow("Grayscale image", image);
	waitKey(0);

	return 0;
}