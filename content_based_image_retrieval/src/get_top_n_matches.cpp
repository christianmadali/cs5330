/*
  Christian Madali
  Northeastern University

*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include "../include/matching_helpers.h"
#include "csv_util.cpp"


/*
  Given a directory on the command line, scans through the directory for image files.

  Prints out the full path name for each file.  This can be used as an argument to fopen or to cv::imread.
 */
int main(int argc, char* argv[]) {
    char img_path[256];
    char feat_db[256];
    char feat_db2[256];
    int N;
    int method;

    // check for sufficient arguments
    if (argc < 5) {
        printf("usage: %s <img path> <feature database path> <number of matches> <matching method> \n", argv[0]);
        printf("Matching method: 1 for baseline, 2 for histogram, 3 for multi-histogram");
        exit(-1);
    }
    strcpy(img_path, argv[1]);
    strcpy(feat_db, argv[2]);
    N = atoi(argv[3]) + 1; // +1 because it'll be a perfect match with itself
    method = atoi(argv[4]);

    if (method == 4) {
        if (argc < 6) {
            printf("usage: %s <img path> <color feature database path> <number of matches> <matching method> <texture feature database> \n", argv[0]);
            printf("Matching method: 1 for baseline, 2 for histogram, 3 for multi-histogram, 4 for color and texture");
            exit(-1);
        }
        strcpy(feat_db2, argv[5]);
    }

    std::vector<char*> img_filenames;
    std::vector<std::vector<float>> data;
    read_image_data_csv(feat_db, img_filenames, data, false);

    std::vector<char*> img_filenames2;
    std::vector<std::vector<float>> data2;
    if (method == 4)
        read_image_data_csv(feat_db2, img_filenames2, data2, false);

    std::vector<float> distances;

    cv::Mat image = cv::imread(img_path);
    cv::imshow("Original",image);
    cv::waitKey(0);
    cv::Mat feat;
    cv::Mat feat2;

    if (method == 1)
        get_middle_feat(image, feat, 9);
    else if (method == 2)
        calc_RG_histogram(image, feat);
    else if (method == 3)
        multi_hist_feat(image, feat, feat2);
    else if (method == 4)
        color_texture_feat(image, feat, feat2); // feat is color feat 2 is texture

    std::vector<float> feat_flat;
    std::vector<float> feat_flat2;
    flatten_mat(feat, feat_flat);
    if (method == 3 ) {
        flatten_mat(feat2, feat_flat2);
        feat_flat.insert(feat_flat.end(), feat_flat2.begin(), feat_flat2.end());
    }
    else if (method == 4) {
        flatten_mat(feat2, feat_flat2);
    }
    for (int i = 0; i < img_filenames.size(); i++) {
        if(method == 1)
            distances.push_back(sum_squared_distance(feat_flat, data.at(i)));
        else if (method == 2 || method == 3) {
            distances.push_back(intersection_distance(feat_flat, data.at(i)));
        }
        else if (method == 4) {
            distances.push_back(double_intersection_distance(feat_flat, data.at(i), feat_flat2, data2.at(i)));
        }
    }

    std::vector<char*> n_matches;
    while (N > 0) {
        auto it = std::minmax_element(distances.begin(), distances.end());
        int min_idx = std::distance(distances.begin(), it.first);
        n_matches.push_back(img_filenames.at(min_idx));
        img_filenames.erase(img_filenames.begin() + min_idx);
        data.erase(data.begin() + min_idx);
        distances.erase(distances.begin() + min_idx);
        N--;
    }

    for (int i = 0; i < n_matches.size(); i++) {
        if (i > 0) {
            cv::Mat image = cv::imread(n_matches.at(i));
            cv::imshow("Matches", image);
            cv::waitKey(0);
        }
        if (i == 0)
            std::cout << "Original: " << n_matches.at(i) << std::endl;
        else
            std::cout << "Matches: " << n_matches.at(i) << ' ';
    }
    printf("\nTerminating\n");

    return(0);
}



