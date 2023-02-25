#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(void) {

	// Read image in GrayScale mode
	Mat image = imread("northeastern.jpg", 0);

	// Save grayscale image
	imwrite("northeastern_gray.jpg", image);

	while (true) {
		imshow("Grayscale image", image);
		char key = cv::waitKey(30);
		if (key == 'q') {
			break;
		}
	}
	

	return 0;
}