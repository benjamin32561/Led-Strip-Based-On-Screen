#include "func.h"

string numToStr(int num)
{
	string to_ret = to_string(num);
	while (to_ret.size() < 3)
		to_ret = '0' + to_ret;
	return to_ret;
}

string buildData(Mat image)
{
	string to_ret = "";
	int height = image.rows;
	int width = image.cols;
	int totalPix = height * width;
	rgb rgb_data;
	rgb_data.b = 0;
	rgb_data.g = 0;
	rgb_data.r = 0;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j += 4)//checking only every 4th pixel
		{
			cv::Vec4b colors = image.at<cv::Vec4b>(i, j);
			rgb_data.r += colors[2];
			rgb_data.g += colors[1];
			rgb_data.b += colors[0];
		}
	}
	rgb_data.b /= totalPix;
	rgb_data.g /= totalPix;
	rgb_data.r /= totalPix;

	to_ret += numToStr(rgb_data.r);
	to_ret += numToStr(rgb_data.g);
	to_ret += numToStr(rgb_data.b);
	to_ret += ';';

	return to_ret;
}

BITMAPINFOHEADER createBitmapHeader(int width, int height)
{
	BITMAPINFOHEADER  bi;

	// create a bitmap
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	return bi;
}