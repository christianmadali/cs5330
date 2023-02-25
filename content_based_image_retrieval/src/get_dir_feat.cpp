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
#include "../include/dirent.h"


/*
  Given a directory on the command line, scans through the directory for image files.

  Prints out the full path name for each file.  This can be used as an argument to fopen or to cv::imread.
 */
int main(int argc, char* argv[]) {
    char dirname[256];
    char buffer[256];
    FILE* fp;
    DIR* dirp;
    struct dirent* dp;
    int i;
    int method;

    // check for sufficient arguments
    if (argc < 3) {
        printf("usage: %s <directory path> <matching method>\n", argv[0]);
        printf("Matching method: 1 for baseline, 2 for histogram, 3 for multi-histogram, 4 for color and texture");
        exit(-1);
    }

    // get the directory path
    strcpy(dirname, argv[1]);
    printf("Processing directory %s\n", dirname);
    method = atoi(argv[2]);
    // open the directory
    dirp = opendir(dirname);
    if (dirp == NULL) {
        printf("Cannot open directory %s\n", dirname);
        exit(-1);
    }

    char feat_db_filename[256];
    strcpy(feat_db_filename, dirname);

    char feat_db_filename2[256];
    strcpy(feat_db_filename2, dirname);
    if (method == 1)
        strcat(feat_db_filename, "baseline_features");
    else if (method == 2)
        strcat(feat_db_filename, "histogram_features");
    else if (method == 3)
        strcat(feat_db_filename, "multi_hist_features");
    else if (method == 4) {
        strcat(feat_db_filename, "text_col_hist_features");
        strcat(feat_db_filename2, "text_hist_features");

    }

    bool reset_file = true;
    // loop over all the files in the image file listing
    while ((dp = readdir(dirp)) != NULL) {

        // check if the file is an image
        if (strstr(dp->d_name, ".jpg") ||
            strstr(dp->d_name, ".png") ||
            strstr(dp->d_name, ".ppm") ||
            strstr(dp->d_name, ".tif")) {

            //printf("processing image file: %s\n", dp->d_name);

            // build the overall filename
            strcpy(buffer, dirname);
            strcat(buffer, "/");
            strcat(buffer, dp->d_name);


            cv::Mat image = cv::imread(buffer);
            cv::Mat feat;
            cv::Mat feat2;
            if (method == 1)
                get_middle_feat(image, feat, 9);
            else if (method == 2)
                calc_RG_histogram(image, feat);
            else if (method == 3)
                multi_hist_feat(image, feat, feat2);
            else if (method == 4)
                color_texture_feat(image, feat, feat2);

            std::vector<float> feat_flat;
            std::vector<float> feat_flat2;
            flatten_mat(feat, feat_flat);
            if (method == 3) {
                flatten_mat(feat2, feat_flat2);
                feat_flat.insert(feat_flat.end(), feat_flat2.begin(), feat_flat2.end());
            }
            else if (method == 4)
                flatten_mat(feat2, feat_flat2);

            append_image_data_csv(feat_db_filename, buffer, feat_flat, reset_file);
            if (method == 4)
                append_image_data_csv(feat_db_filename2, buffer, feat_flat2, reset_file);
            if (reset_file) {
                reset_file = false;
            }

        }
    }
    printf("Features database: %s\n", feat_db_filename);
    if (method == 4)
        printf("Features database 2: %s\n", feat_db_filename2);

    printf("Terminating\n");

    return(0);
}



