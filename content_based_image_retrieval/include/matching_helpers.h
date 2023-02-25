/*
  Christian Madali
  Northeastern University

*/
#pragma once
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"

int calc_RG_histogram(cv::Mat& src, cv::Mat& hist) {
    const int Hsize = 32;
    const int divisor = 256 / Hsize;
    int i, j;

    int dim[2] = { Hsize, Hsize };

    // allocate an initialize a histogram
    hist = cv::Mat::zeros(2, dim, CV_32S);


    for (i = 0; i < src.rows; i++) {
        cv::Vec3b* sptr = src.ptr<cv::Vec3b>(i);
        for (j = 0; j < src.cols; j++) {
            int r = sptr[j][2] / divisor;
            int g = sptr[j][1] / divisor;
            hist.at<int>(r, g)++;
        }
    }


    return(0);
}

int flatten_mat(cv::Mat& src, std::vector<float>& dest) {
    cv::Mat flat = src.reshape(1, src.total()*src.channels());
    dest = src.isContinuous() ? flat : flat.clone();

    return 0;
}

float intersection_distance(std::vector<float>& ha, std::vector<float>& hb) {
    double intersection = 0.0;

    //compute sum of histogram buckets to make sure it is normalized
    double suma = 0.0;
    double sumb = 0.0;
    for (int i = 0; i < ha.size(); i++) {
        suma += ha[i];
        sumb += hb[i];
    }

    for (int i = 0; i < ha.size(); i++) {
        double af = ha[i] / suma;
        double bf = hb[i] / sumb;
        intersection += af < bf ? af : bf;
    }

    return (1.0 - intersection);
}

float sum_squared_distance(std::vector<float>& ha, std::vector<float>& hb) {
    double distance = 0.0;

    for (int i = 0; i < ha.size(); i++) {
        distance += (ha[i] - hb[i]) * (ha[i] - hb[i]);
    }

    return distance;
}

int get_middle_feat(cv::Mat& src, cv::Mat& dst, int square_size) {
    int i, j;

    int dim[2] = { square_size, square_size };

    // allocate an initialize a histogram
    dst = cv::Mat::zeros(2, dim, CV_32SC3);

    for (i = 0; i < dst.rows; i++) {
        cv::Vec3b* sptr = src.ptr<cv::Vec3b>(i + src.rows / 2 - square_size / 2);
        cv::Vec3b* dptr = dst.ptr<cv::Vec3b>(i);
        for (j = 0; j < dst.cols; j++) {
            dptr[j][0] = sptr[j + src.cols / 2 - square_size / 2][0];
            dptr[j][1] = sptr[j + src.cols / 2 - square_size / 2][1];
            dptr[j][2] = sptr[j + src.cols / 2 - square_size / 2][2];
        }
    }

    return(0);
}

int multi_hist_feat(cv::Mat& src, cv::Mat& hist1, cv::Mat& hist2) {
    calc_RG_histogram(src, hist1);
    cv::Mat temp;
    get_middle_feat(src, temp, 100);
    calc_RG_histogram(temp, hist2);

    hist1 = hist1 * 0.70; // give 70% weight
    hist2 = hist2 * 0.30; // give 30% weight

    return 0;
}

int sobel_hist_feat(cv::Mat& src, cv::Mat& hist) {
    cv::Mat src_gray;
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    cv::Mat grad;

    int dst_type = CV_16S;

    cv::Sobel(src_gray, grad_x, dst_type, 1, 0, 1, 1, 0, cv::BORDER_DEFAULT);
    cv::Sobel(src_gray, grad_y, dst_type, 0, 1, 1, 1, 0, cv::BORDER_DEFAULT);

    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad); // Weigh equally as approximation

    const int Hsize = 16;
    const int divisor = 256 / Hsize;
    int i, j;

    // allocate an initialize a histogram
    hist = cv::Mat::zeros(1, Hsize, CV_32S);


    for (i = 0; i < grad.rows; i++) {
        uchar* sptr = grad.ptr<uchar>(i);
        for (j = 0; j < grad.cols; j++) {
            int gray = sptr[j] / divisor;
            hist.at<int>(gray)++;
        }
    }

    return(0);
}

int color_texture_feat(cv::Mat& src, cv::Mat& hist1, cv::Mat& hist2) {
    calc_RG_histogram(src, hist1);
    sobel_hist_feat(src, hist2);

    return 0;
}

float double_intersection_distance(std::vector<float>& ha, std::vector<float>& hb, std::vector<float>& hc, std::vector<float>& hd) {
    return intersection_distance(ha, hb) * 0.5 + intersection_distance(hc, hd) * 0.5;
}