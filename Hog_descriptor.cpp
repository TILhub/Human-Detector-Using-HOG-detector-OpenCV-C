#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#define X 320
#define Y 240
using namespace std;
using namespace cv;
HOGDescriptor hog;
void mark_human(Mat &img)
{
		vector<Rect> found, found_filtered;
		hog.detectMultiScale(img, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
		size_t i, j;
		for (i = 0; i<found.size(); i++)
		{
			Rect r = found[i];
			for (j = 0; j<found.size(); j++)
				if (j != i && (r & found[j]) == r)
					break;
			if (j == found.size())
				found_filtered.push_back(r);
		}

		for (i = 0; i<found_filtered.size(); i++)
		{
			Rect r = found_filtered[i];
			r.x += cvRound(r.width*0.1);
			r.width = cvRound(r.width*0.8);
			r.y += cvRound(r.height*0.07);
			r.height = cvRound(r.height*0.8);
			rectangle(img, r.tl(), r.br(), Scalar(rand()%255, rand()%255, rand()%255), 3);
		}

		imshow("opencv", img);
		
	
}
int main(int argc, const char * argv[])
{
	bool still_cam = true;				//to use camera change still_cam to false
										//true -- uses still camera
										//false -- uses video camera
	if (still_cam) {
		Mat img;
		img = imread("e:/test2.jpg", CV_LOAD_IMAGE_COLOR);
		if (img.empty()) {
			cout << "Image not Found\n";
			return -1;
		}
		//imshow("Input", img);
		hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
		mark_human(img);
		waitKey(0);
	}
	else {
		Mat img;
		VideoCapture cap(0);
		if (!cap.isOpened()) {
			cout << "Error! Cann't Open Camera!\n";
			return -1;
		}
		cap.set(CV_CAP_PROP_FRAME_WIDTH, X);		//this is the dimensions for video//
		cap.set(CV_CAP_PROP_FRAME_HEIGHT, Y);		//to increase performance 320x240 resolution is taken
		cap >> img;
		hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
		mark_human(img);
		waitKey(10);
	}
	return 0;
}