// LSB start _ end marker set

#ifndef TEXT_LSB_H
#define TEXT_LSB_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <bitset>
#include <cassert>
#include <unordered_map>

using namespace cv;
using namespace std;

// ¯\_(ツ)_/¯
unordered_map<char, string> obfuscationMap = {
    {'A', "\xEF\xBB\xBF"}, {'B', "\xC2\xAD"}, {'C', "\xE2\x81\xAF"}, {'D', "\xE2\x80\x8E"},
    {'E', "\xE2\x80\x8F"}, {'F', "\xE2\x80\xAA"}, {'G', "\xE2\x80\xAB"}, {'H', "\xE2\x80\xAC"},
    {'I', "\xE2\x80\xAD"}, {'J', "\xE2\x80\xAE"}, {'K', "\xE2\x80\xAF"}, {'L', "\xE2\x80\xB0"},
    {'M', "\xE2\x80\xB1"}, {'N', "\xE2\x80\xB2"}, {'O', "\xE2\x80\xB3"}, {'P', "\xE2\x80\xB4"},
    {'Q', "\xE2\x80\xB5"}, {'R', "\xE2\x80\xB6"}, {'S', "\xE2\x80\xB7"}, {'T', "\xE2\x80\xB8"},
    {'U', "\xE2\x80\xB9"}, {'V', "\xE2\x80\xBA"}, {'W', "\xE2\x80\xBB"}, {'X', "\xF0\x9F\xA0\xAF"},
    {'Y', "\xE2\x80\xBD"}, {'Z', "\xE2\x80\xBE"}, 
    {'a', "\xE2\x80\x8A"}, {'b', "\xE2\x82\x99"}, {'c', "\x28\x00"}, {'d', "\xE2\x82\x9B"},
    {'e', "\xE2\x80\x89"}, {'f', "\xD8\x9F\xD8\xA0"}, {'g', "\xE1\x9B\xB4"}, {'h', "\xE1\xA0\x8B"},
    {'i', "\xE1\xA0\x8D"}, {'j', "\xF0\x9D\x85\xBA"}, {'k', "\xE2\x82\x9A"}, {'l', "\xF0\x9D\x85\xB5"},
    {'m', "\xF0\x9D\x85\xB4"}, {'n', "\xF0\x9D\x85\xB3"}, {'o', "\xF0\x9D\x85\x99"}, {'p', "\xF3\x98\x9C"},
    {'q', "\xEF\xB8\xBC"}, {'r', "\xF0\x9F\xA0\xA6"}, {'s', "\xF0\x9F\xA0\xA7"}, {'t', "\xF0\x9F\xA0\xA8"},
    {'u', "\xE2\x80\x87"}, {'v', "\xF0\x9F\xA0\xA9"}, {'w', "\xE2\x80\x80"}, {'x', "\x20\x2E"},
    {'y', "\xE2\x80\x83"}, {'z', "\xE2\x80\x85"},
    {'0', "\xE2\x80\x8D"}, {'1', "\xE2\x80\x8C"}, {'2', "\xE2\x80\x88"}, {'3', "\xE2\x81\xAE"},
    {'4', "\xE2\x80\x8B"}, {'5', "\xCC\x8F"}, {'6', "\xD8\xA0\xD8\xA1"}, {'7', "\xE1\x9B\xB5"},
    {'8', "\xE1\xA0\x8C"}, {'9', "\xF0\x9D\x85\xB6"},
    {' ', "\xC2\xA0"}, {'!', "\xE2\x80\x86"}, {'"', "\xF0\x9F\xA0\xAE"}, {'#', "\xE1\xA0\x8E"},
    {'$', "\xE2\x80\xBC"}, {'%', "\xE2\x80\x81"}, {'&', "\xE2\x80\x84"}, {'\'', "\xE2\x80\xBF"},
    {'(', "\xE0\x02\x09"}, {')', "\xEF\xA0\x8A"}, {'*', "\xE2\x81\x82"}, {'+', "\xEF\xA0\x8B"},
    {',', "\xE2\x81\x84"}, {'-', "\xE2\x81\x85"}, {'.', "\xE2\x81\x86"}, {'/', "\xE2\x81\x87"},
    {':', "\xE2\x81\x88"}, {';', "\xE2\x81\x89"}, {'<', "\xE2\x81\x8A"}, {'=', "\xE2\x81\x8B"},
    {'>', "\xE2\x81\x8C"}, {'?', "\xE2\x81\x8D"}, {'@', "\xE2\x81\x8E"}, {'[', "\xE2\x81\x8F"},
    {'\\', "\xE2\x81\x90"}, {']', "\xE2\x81\x91"}, {'^', "\xE2\x81\x92"}, {'_', "\xE2\x81\x93"},
    {'`', "\xE2\x81\x94"}, {'{', "\xE2\x81\x95"}, {'|', "\xE2\x81\x96"}, {'}', "\xE2\x81\x97"},
    {'~', "\xE2\x81\x98"}
};

inline string obfuscateText(const string &text) {
    string obfuscatedText;
    for (char c : text) {
        auto it = obfuscationMap.find(c);
        if (it != obfuscationMap.end()) {
            obfuscatedText += it->second;
        } else {
            obfuscatedText += c;
        }
    }
    return obfuscatedText;
}

inline string deobfuscateText(const string &obfuscatedText) {
    string deobfuscatedText;
    string currentInvisible;
    for (char c : obfuscatedText) {
        currentInvisible += c;
        for (const auto &entry : obfuscationMap) {
            if (entry.second == currentInvisible) {
                deobfuscatedText += entry.first;
                currentInvisible.clear();
                break;
            }
        }
        if (currentInvisible.empty()) {
            continue;
        }
    }
    return deobfuscatedText;
}

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

    if (static_cast<size_t>((text.size() + 6) * 8) > static_cast<size_t>(imageWidth * imageHeight * channels)) {
        throw runtime_error("Insufficient image capacity for embedding text.");
    }

    string startMarker = "\xE2\x80\x8B";
    for (char character : startMarker) {
        embedBit(image, bitIndex, channels, character);
    }

    string obfuscatedText = obfuscateText(text);

    uint32_t textSize = obfuscatedText.size();
    for (int i = 3; i >= 0; --i) {
        char lengthByte = (textSize >> (i * 8)) & 0xFF;
        embedBit(image, bitIndex, channels, lengthByte);
    }

    for (char character : obfuscatedText) {
        embedBit(image, bitIndex, channels, character);
    }

    string endMarker = "\xE2\x80\x8C";
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

    string startMarker = "\xE2\x80\x8B";
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

    string endMarker = "\xE2\x80\x8C";
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



