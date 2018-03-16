/*! *****************************************************************************
*   \file testRenderGl_main.cpp
*   \author moennen
*   \brief 
*   \date 2018-03-02
*   *****************************************************************************/
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

const string keys =
        "{help h usage ? |         | print this message   }"
        "{@imageA        |         | image1 for compare   }"
        "{@imageB        |<none>   | image2 for compare   }"
        "{@output        |/tmp/out | NNField              }" ;

int main(int argc, char *argv[])
{
   CommandLineParser parser(argc, argv, keys);
   if (parser.has("help")) {parser.printMessage(); return (0);}
   string inputFilenameA = parser.get<string>("@imageA");
   string inputFilenameB = parser.get<string>("@imageB");

   string outputFilename = parser.get<string>("@output");

   return(0);
}

