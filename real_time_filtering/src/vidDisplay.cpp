#include <iostream>
#include <opencv2/core.hpp>     
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  
#include "opencv2/imgproc/imgproc.hpp"
#include "filter.cpp"

enum options { default = 0, cv_greyscale_trans = 1, alt_greyscale_trans = 2, blur_trans = 3};
int main(int argc, char* argv[]) {
    cv::VideoCapture* capdev;

    // open the video device
    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return(-1);
    }

    // get some properties of the image
    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
        (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    cv::namedWindow("Video", 1); // identifies a window
    cv::Mat frame = Mat::zeros(refS.width, refS.height, CV_32FC3);
    options options_toggle = default;

    while (true) {
        *capdev >> frame; // get a new frame from the camera, treat as a stream
        if (frame.empty()) {
            printf("frame is empty\n");
            break;
        }
        cv::Mat gray_frame;
        cv::Mat blur_frame;
        if (options_toggle == cv_greyscale_trans) {
            cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);
            cv::imshow("Video", gray_frame);
        }
        else if (options_toggle == alt_greyscale_trans){
            greyscale(frame, gray_frame);
            cv::imshow("Video", gray_frame);
        }
        else if (options_toggle == blur_trans) {
            blur5x5(frame, blur_frame);
            cv::imshow("Video", blur_frame);
        }
        else
            cv::imshow("Video", frame);
        
        // see if there is a waiting keystroke
        char key = cv::waitKey(10);
        if (key == 'g') {
            options_toggle = cv_greyscale_trans;
            std::cout << "Converting to grayscale \n";
        }
        else if (key == 'h') {
            options_toggle = alt_greyscale_trans;
            std::cout << "Converting to grayscale \n";
        }
        else if (key == 'b') {
            options_toggle = blur_trans;
            std::cout << "Blurring \n";
        }
        else if (key == 'q') {
            if (options_toggle == cv_greyscale_trans && !gray_frame.empty()) {
                cv::imwrite("vid_frame_gray.jpg", gray_frame);
                cv::imwrite("vid_frame_gray_original.jpg", frame);
            }
                
            else if (options_toggle == alt_greyscale_trans && !gray_frame.empty()) {
                cv::imwrite("vid_frame_gray_alt.jpg", gray_frame);
                cv::imwrite("vid_frame_gray_alt_original.jpg", frame);
            }
            break;
        }
        
    }
    Mat gauss_frame;

    delete capdev;
    return(0);
}