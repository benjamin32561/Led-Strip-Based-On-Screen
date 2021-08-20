#include <Windows.h>
#include <iostream>
#include <atlimage.h>
#include <opencv2/opencv.hpp>
#include "structs.cpp"

using namespace cv;
using namespace std;

string numToStr(int num);
string buildData(Mat image);
BITMAPINFOHEADER createBitmapHeader(int width, int height);