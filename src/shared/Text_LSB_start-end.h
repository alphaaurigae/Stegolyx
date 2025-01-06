#ifndef TEXT_LSB_H
#define TEXT_LSB_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <bitset>
#include <cassert>
#include <unordered_map>
#include "obv.h"

using namespace cv;
using namespace std;

inline int extractBitAndUpdate(int &bitIndex, const Mat &image, unsigned char &currentChar, int imageWidth, int imageHeight, int channels) {
    if (bitIndex >= imageWidth * imageHeight * channels) {
        throw runtime_error("Text length decoding error.");
    }

    int y = bitIndex / (imageWidth * channels);
    int x = (bitIndex % (imageWidth * channels)) / channels;
    int channel = bitIndex % channels;

    int bit;
    if (channels == 1) {
        bit = (image.at<uchar>(y, x) & 1);
    } else if (channels == 3) {
        bit = (image.at<Vec3b>(y, x)[channel] & 1);
    } else if (channels == 4) {
        bit = (image.at<Vec4b>(y, x)[channel] & 1);
    } else {
        throw runtime_error("Unsupported channel configuration.");
    }

    currentChar |= (bit << (7 - (bitIndex % 8)));
    bitIndex++;
    return bit;
}

inline void extractBits(int &bitIndex, const Mat &image, unsigned char &currentChar, int imageWidth, int imageHeight, int channels) {
    for (int bitPos = 7; bitPos >= 0; --bitPos) {
        extractBitAndUpdate(bitIndex, image, currentChar, imageWidth, imageHeight, channels);
    }
}

inline void embedBit(Mat &image, int &bitIndex, int channels, uint8_t byte) {
    for (int bitPos = 7; bitPos >= 0; --bitPos) {
        int bit = (byte >> bitPos) & 1;
        int y = bitIndex / (image.cols * channels);
        int x = (bitIndex % (image.cols * channels)) / channels;
        int channel = bitIndex % channels;
        if (channels == 1) {
            image.at<uchar>(y, x) = (image.at<uchar>(y, x) & 0xFE) | bit;
        } else if (channels == 3) {
            image.at<Vec3b>(y, x)[channel] = (image.at<Vec3b>(y, x)[channel] & 0xFE) | bit;
        } else if (channels == 4) {
            image.at<Vec4b>(y, x)[channel] = (image.at<Vec4b>(y, x)[channel] & 0xFE) | bit;
        }
        bitIndex++;
    }
}

inline void embedTextLSB(Mat &image, const string &text) {
    int channels = image.channels();
    int bitIndex = 0;
    int imageWidth = image.cols;
    int imageHeight = image.rows;

    string startMarker = "\x20\x2E";
    string endMarker = "\xC2\xAD";

    string obfuscatedText = obfuscateText(text);
    uint32_t obfuscatedTextSize = obfuscatedText.size();

    if (static_cast<size_t>((startMarker.size() + 4 + obfuscatedTextSize + endMarker.size()) * 8) > static_cast<size_t>(imageWidth * imageHeight * channels)) {
        throw runtime_error("Insufficient image capacity for embedding text.");
    }

    for (char character : startMarker) {
        embedBit(image, bitIndex, channels, character);
    }

    for (int i = 3; i >= 0; --i) {
        char lengthByte = (obfuscatedTextSize >> (i * 8)) & 0xFF;
        embedBit(image, bitIndex, channels, lengthByte);
    }

    for (char character : obfuscatedText) {
        embedBit(image, bitIndex, channels, character);
    }

    for (char character : endMarker) {
        embedBit(image, bitIndex, channels, character);
    }
}

inline string extractTextLSB(const Mat &image) {
    int channels = image.channels();
    int imageWidth = image.cols;
    int imageHeight = image.rows;
    string decodedText;
    unsigned char currentChar = 0;
    int bitIndex = 0;

    string startMarker = "\x20\x2E";
    string endMarker = "\xC2\xAD";

    for (char markerChar : startMarker) {
        currentChar = 0;
        extractBits(bitIndex, image, currentChar, imageWidth, imageHeight, channels);
        if (currentChar != static_cast<unsigned char>(markerChar)) {
            throw runtime_error("Start marker mismatch.");
        }
    }

    uint32_t textLength = 0;
    for (int i = 3; i >= 0; --i) {
        currentChar = 0;
        extractBits(bitIndex, image, currentChar, imageWidth, imageHeight, channels);
        textLength |= (currentChar << (i * 8));
    }

    for (uint32_t i = 0; i < textLength; ++i) {
        currentChar = 0;
        extractBits(bitIndex, image, currentChar, imageWidth, imageHeight, channels);
        decodedText.push_back(currentChar);
    }

    for (char markerChar : endMarker) {
        currentChar = 0;
        extractBits(bitIndex, image, currentChar, imageWidth, imageHeight, channels);
        if (currentChar != static_cast<unsigned char>(markerChar)) {
            throw runtime_error("End marker mismatch.");
        }
    }

    return deobfuscateText(decodedText);
}

#endif