#ifndef TEXT_LSB_H
#define TEXT_LSB_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <bitset>
#include <cassert>

using namespace cv;
using namespace std;

inline void embedTextLSB(Mat &image, const string &text) {
    int channels = image.channels();
    int bitIndex = 0;
    int textSize = text.size();
    int imageWidth = image.cols;
    int imageHeight = image.rows;

    if (textSize * 8 > imageWidth * imageHeight * channels) {
        throw runtime_error("Text is too large to be embedded");
    }

    for (int i = 0; i < textSize; ++i) {
        char character = text[i];
        for (int bitPos = 7; bitPos >= 0; --bitPos) {
            int bit = (character >> bitPos) & 1;
            int y = bitIndex / (imageWidth * channels);
            int x = (bitIndex % (imageWidth * channels)) / channels;
            int channel = bitIndex % channels;

            if (channels == 1) {
                image.at<uchar>(y, x) = (image.at<uchar>(y, x) & 0xFE) | bit;
            } else {
                if (channels == 4) {
                    image.at<Vec4b>(y, x)[channel] = (image.at<Vec4b>(y, x)[channel] & 0xFE) | bit;  // RGBA
                } else {
                    image.at<Vec3b>(y, x)[channel] = (image.at<Vec3b>(y, x)[channel] & 0xFE) | bit;  // BGR/RGB
                }
            }
            bitIndex++;
        }
    }

    // termination character (NULL) eom
    string termination = "\0";
    for (size_t i = 0; i < termination.size(); ++i) {
        char character = termination[i];
        for (int bitPos = 7; bitPos >= 0; --bitPos) {
            int bit = (character >> bitPos) & 1;
            int y = bitIndex / (imageWidth * channels);
            int x = (bitIndex % (imageWidth * channels)) / channels;
            int channel = bitIndex % channels;

            if (channels == 1) {
                image.at<uchar>(y, x) = (image.at<uchar>(y, x) & 0xFE) | bit;
            } else {
                if (channels == 4) {
                    image.at<Vec4b>(y, x)[channel] = (image.at<Vec4b>(y, x)[channel] & 0xFE) | bit;  // RGBA
                } else {
                    image.at<Vec3b>(y, x)[channel] = (image.at<Vec3b>(y, x)[channel] & 0xFE) | bit;  // BGR/RGB
                }
            }
            bitIndex++;
        }
    }
}

inline string extractTextLSB(const Mat& image) {
    int channels = image.channels();
    if (channels != 3 && channels != 1 && channels != 4) {
        throw runtime_error("Unsupported image format");
    }

    int imageWidth = image.cols;
    int imageHeight = image.rows;
    string decodedText;
    char currentChar = 0;
    int bitIndex = 0;
    bool stopDecoding = false;

    for (int y = 0; y < imageHeight && !stopDecoding; ++y) {
        for (int x = 0; x < imageWidth && !stopDecoding; ++x) {
            for (int channel = 0; channel < channels && !stopDecoding; ++channel) {
                int bit = 0;

                if (channels == 4) {
                    bit = image.at<Vec4b>(y, x)[channel] & 1;  // RGBA
                } else if (channels == 3) {
                    bit = image.at<Vec3b>(y, x)[channel] & 1;  // RGB
                } else if (channels == 1) {
                    bit = image.at<uchar>(y, x) & 1;  // Grayscale
                }

                currentChar |= (bit << (7 - bitIndex));
                ++bitIndex;

                if (bitIndex == 8) {
                    if (currentChar == '\0') {
                        stopDecoding = true;
                        break;
                    }

                    if (isprint(currentChar)) {  // Only printable characters
                        decodedText += currentChar;
                    } else {
                        stopDecoding = true;  // Stop if unprintable char
                        break;
                    }

                    currentChar = 0;  // Reset for next byte
                    bitIndex = 0;
                }
            }
        }
    }

    return decodedText;
}

#endif