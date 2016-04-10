// File Description
// Author: Philip Salvaggio

#include "local_laplacian.h"

#include <iostream>
#include <sstream>

using namespace std;

void OutputBinaryImage(const std::string& filename, cv::Mat image) {
  FILE* f = fopen(filename.c_str(), "wb");
  for (int x = 0; x < image.cols; x++) {
    for (int y = 0; y < image.rows; y++) {
      double tmp = image.at<double>(y, x);
      fwrite(&tmp, sizeof(double), 1, f);
    }
  }
  fclose(f);
}


const char *version = "v0.1";

// http://people.csail.mit.edu/sparis/publi/2011/siggraph/additional_results/detail_manipulation/lenna.html

const char *keys =
{
    "{ input     |       | input filename                            }"
    "{ output    |       | output filename                           }"
    "{ levels    |   3   | number of pyramid levels                  }"
    "{ sigma     |  0.3  | threshold distinguishing details from edges. Smaller values limit the manipulation to smaller-amplitude variations }"
    "{ alpha     |  2.0  | controls how details are modified: 0<a<1 amplifies detail, while a>1 attenuates it.     }"
    "{ beta      |  1.0  | intensity range: beta > 1.0 performs expansion, while beta < 1.0 performs compression.  }"
    "{ threads   | false | use worker threads when possible          }"
    "{ verbose   | false | print verbose diagnostics                 }"
    "{ build     | false | print the OpenCV build information        }"
    "{ help      | false | print help message                        }"
    "{ version   | false | print the application version             }"
};

int main(int argc, char *argv[])
{
    cv::CommandLineParser parser(argc, argv, keys);
    
    if(argc < 2 || parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    else if(parser.has("build"))
    {
        std::cout << cv::getBuildInformation() << std::endl;
        return 0;
    }
    else if(parser.has("version"))
    {
        std::cout << argv[0] << " v" << version << std::endl;
        return 0;
    }

    const double kSigmaR = parser.get<double>("sigma"); // 0.3
    const double kAlpha = parser.get<double>("alpha");  // 2.0
    const double kBeta = parser.get<double>("beta");    // 1.0
    const int kLevels = parser.get<int>("levels");      // 3

    std::cout << "sigma = " << kSigmaR << " alpha = " << kAlpha << " beta " << kBeta << std::endl;

    std::string sInput = parser.get<std::string>("input");

    if(sInput.empty())
    {
        std::cerr << "Must specify input filename" << std::endl;
        return 1;
    }

    std::string sOutput = parser.get<std::string>("output");

    if(sOutput.empty())
    {
        std::cerr << "Msut specify output filename" << std::endl;
        return 1;
    }

    cv::Mat input = cv::imread(sInput);
    if(input.empty())
    {
        std::cerr << "Unable to read input file " << sInput << std::endl;
        return 1;
    }

    input.convertTo(input, CV_64F, 1 / 255.0);

    cout << "Input image: " << sInput << " Size: " << input.cols << " x " << input.rows << " Channels: " << input.channels() << endl;

    cv::Mat output;
    if (input.channels() == 1) {
        output = LocalLaplacianFilter<double>(input, kAlpha, kBeta, kSigmaR, kLevels);
    } else if (input.channels() == 3) {
        output = LocalLaplacianFilter<cv::Vec3d>(input, kAlpha, kBeta, kSigmaR, kLevels);
    } else {
        cerr << "Input image must have 1 or 3 channels." << endl;
        return 1;
    }

    output *= 255;
    output.convertTo(output, CV_8UC3);
    
    cv::imwrite(sOutput, output);

    return 0;
}
