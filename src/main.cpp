//#include "Text_LSB_start-end.h"
#include "Text_LSB_null.h"  //(for testing instead of Text_LSB_start-end.h)
#include "Image_LSB.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>  // For transform()

using namespace std;
using namespace cv;

void saveTextToFile(const string& output_file, const string& text) {
    ofstream ofs(output_file, ios::out | ios::trunc);
    if (!ofs) {
        cerr << "Error: Cannot open output file to save text!" << endl;
        exit(1);
    }
    ofs << text;
    ofs.close();
}

int main(int argc, char** argv) {
    string mode, method = "lsb", input_file, output_file, fg_file, text;
    bool decrypt = false;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-mode" && i + 1 < argc) mode = argv[++i];
        else if (arg == "-i" && i + 1 < argc) input_file = argv[++i];
        else if (arg == "-o" && i + 1 < argc) output_file = argv[++i];
        else if (arg == "-fg" && i + 1 < argc) fg_file = argv[++i];
        else if (arg == "-t" && i + 1 < argc) text = argv[++i];
        else if (arg == "-d") decrypt = true;
        else if (arg == "-method" && i + 1 < argc) method = argv[++i];
        else {
            cerr << "Error: Unknown or malformed argument: " << arg << endl;
            return 1;
        }
    }

    if (input_file.empty()) {
        cerr << "Error: Input file not specified. Use -i <input_file>." << endl;
        return 1;
    }

    if (output_file.empty() && !decrypt) {
        cerr << "Error: Output file not specified. Use -o <output_file>." << endl;
        return 1;
    }

    if (mode != "text" && mode != "image") {
        cerr << "Error: Invalid mode specified! Use 'text' or 'image'." << endl;
        return 1;
    }

    Mat img = imread(input_file, IMREAD_COLOR | IMREAD_UNCHANGED);
    if (img.empty()) {
        cerr << "Error: Cannot open input file!" << endl;
        return 1;
    }

    if (decrypt) {
        string extracted_text;
        if (method == "lsb") {
            extracted_text = extractTextLSB(img);
        }

        if (!extracted_text.empty()) {
            saveTextToFile(output_file, extracted_text);
            cout << "Text saved to " << output_file << endl;
        } else {
            cerr << "Error: No text extracted" << endl;
            return 1;
        }
    } else {
        if (method == "lsb") {
            if (mode == "text") {
                embedTextLSB(img, text);
            } else if (mode == "image") {
                Mat fg_img = imread(fg_file, IMREAD_COLOR | IMREAD_UNCHANGED);
                if (fg_img.empty()) {
                    cerr << "Error: Cannot open foreground image!" << endl;
                    return 1;
                }
                embedImageLSB(img, fg_img);
            }
        }

        // Lowercase the extension check
        transform(output_file.begin(), output_file.end(), output_file.begin(), ::tolower);
        vector<int> compression_params;
        string extension = output_file.substr(output_file.find_last_of(".") + 1);

        if (extension == "png") {
    compression_params.push_back(IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);//0-9


    //compression_params.push_back(IMWRITE_PNG_STRATEGY);
   // compression_params.push_back(IMWRITE_PNG_STRATEGY_FILTERED);
  //  compression_params.push_back(IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY);
   // compression_params.push_back(IMWRITE_PNG_STRATEGY_RLE);
        }
        else if (extension == "jpg" || extension == "jpeg") {
            compression_params.push_back(IMWRITE_JPEG_QUALITY);
            compression_params.push_back(100);
        }

        if (!imwrite(output_file, img, compression_params)) {
            cerr << "Error: Cannot save output image!" << endl;
            return 1;
        }
    }

    return 0;
}