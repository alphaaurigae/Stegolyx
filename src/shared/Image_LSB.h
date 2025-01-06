#ifndef IMAGE_LSB_H
#define IMAGE_LSB_H


// mockup ... move along.


#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

inline void embedImageLSB(Mat& bg_img, const Mat& fg_img) {
    Mat resized_fg_img;
    resized_fg_img = fg_img.clone();

    for (int row = 0; row < bg_img.rows; row++) {
        for (int col = 0; col < bg_img.cols; col++) {
            Vec4b& bg_pixel = bg_img.at<Vec4b>(row, col);
            Vec3b fg_pixel = resized_fg_img.at<Vec3b>(row, col);

            
            for (int c = 0; c < 3; c++) {
                bg_pixel[c] &= 0xFE;  // Clear the LSB of the background
                bg_pixel[c] |= (fg_pixel[c] & 1);
            }


            bg_pixel[3] = bg_pixel[3];  // Explicitly keep the existing alpha value
        }
    }
}

inline Mat extractImageLSB(const Mat& img, const Mat& fg_img) {

    Mat hidden_img = Mat::zeros(fg_img.size(), CV_8UC3);

    for (int row = 0; row < img.rows; row++) {
        for (int col = 0; col < img.cols; col++) {
            const Vec3b& img_pixel = img.at<Vec3b>(row, col);
            Vec3b& hidden_pixel = hidden_img.at<Vec3b>(row, col);
            for (int c = 0; c < 3; c++) {
                hidden_pixel[c] = (img_pixel[c] & 1) * 255;
            }
        }
    }

    return hidden_img;
}

#endif