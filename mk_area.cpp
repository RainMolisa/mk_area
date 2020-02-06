#include <iostream>
#include <string>
#include<opencv.hpp>
using namespace std;
using namespace cv;
#define win_name "w1"
typedef struct env_var
{
	int rows;
	int cols;
	float scale;
	//Mat mask_org;
	Mat mask_shw;
	Rect rt;
	Mat img2;
	bool flag;
};
void OnMouse(int event, int x, int y, int flags, void* param);
string get_folder(string file);
Mat mk_show(Mat img, Mat mask);
int main(int argc, char** argv)
{
	string in_file;
	string outFile;
	cin >> in_file;
	string folder = get_folder(in_file);
	outFile = folder + "\\mask.png";
	cout << outFile << endl;
	Mat img = imread(in_file);
	env_var en;
	en.rows = img.rows;
	en.cols = img.cols;
	en.scale = 1;
	//en.mask_org = Mat(en.rows, en.cols, CV_8UC1, Scalar(GC_PR_BGD));
	en.rt = Rect(0, 0, 1, 1);
	en.flag = false;
	//en.mask_org.at<uchar>(en.rows / 2, en.cols / 2) = GC_PR_FGD;
	//en.mask_org.at<uchar>(0, 0) = 1;
	Mat shw = img.clone();
	namedWindow(win_name, WINDOW_AUTOSIZE);
	setMouseCallback(win_name, OnMouse, &en);
	bool flag = true;
	while (flag)
	{
		imshow(win_name, shw);
		int key = waitKey(5);
		switch (key)
		{
		case 'd':
			en.scale = en.scale + 0.02;
			break;
		case 'f':
			en.scale = en.scale - 0.02;
			break;
		case ' ':
			flag = false;
			break;
		default:
			break;
		}
		resize(img, en.img2, Size(en.cols * en.scale, en.rows * en.scale));
		//resize(en.mask_org, en.mask_shw, Size(en.cols * en.scale, en.rows * en.scale));
		
		//Mat bgModel, fgModel;
		//Rect a(0,0,1,1);
		//grabCut(img2, en.mask_shw, Rect(0,0,10,10), bgModel, fgModel, 1, GC_INIT_WITH_RECT);
		//cout << "grabCut finish" << endl;
		//resize(en.mask_shw, en.mask_org, Size(en.cols / en.scale, en.rows / en.scale));
		shw = en.img2.clone();//mk_show(img2, en.mask_shw);
		if (en.flag)
		{
			rectangle(shw, en.rt, Scalar(0, 0, 255));
		}
	}
	destroyAllWindows();
	//
	Mat res = en.mask_shw.clone();
	for (int y = 0; y < res.rows; y++)
	{
		for (int x = 0; x < res.cols; x++)
		{
			int val = res.at<uchar>(y, x);
			if (val == GC_FGD || val == GC_PR_FGD)
			{
				res.at<uchar>(y, x) = 255;
			}
			else
			{
				res.at<uchar>(y, x) = 0;
			}
		}
	}
	resize(res, res, Size(en.cols, en.rows));
	imwrite(outFile, res);
	return 0;
}

void OnMouse(int event, int x, int y, int flags, void* param)
{
	env_var* p = (env_var*)param;
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
	{
		p->flag = true;
		p->rt.x = x;
		p->rt.y = y;
	}
		break;
	case CV_EVENT_LBUTTONUP:
		p->flag = false;
		{
			Mat bgModel, fgModel;
			grabCut(p->img2, p->mask_shw, p->rt, bgModel, fgModel, 1, GC_INIT_WITH_RECT);
		}
		break;
	case CV_EVENT_RBUTTONDOWN:
	{
		
	}
		break;
	case EVENT_MOUSEMOVE:
	{
		if (p->flag)
		{
			p->rt.width = x - p->rt.x;
			p->rt.height = y - p->rt.y;
		}
		break;
	}
	default:
		break;
	}
}

Mat mk_show(Mat img,Mat mask)
{
	int rows = img.rows;
	int cols = img.cols;
	Mat res = img.clone();
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			int mkval = mask.at<uchar>(y, x);
			if (mkval == GC_BGD || mkval == GC_PR_BGD)
			{
				res.at<uchar>(y, 3 * x + 0) = img.at<uchar>(y, 3 * x + 0) / 10;
				res.at<uchar>(y, 3 * x + 1) = img.at<uchar>(y, 3 * x + 1) / 10;
				res.at<uchar>(y, 3 * x + 2) = img.at<uchar>(y, 3 * x + 2) / 10;
			}
		}
	}
	return res;
}

string get_folder(string file)
{
	int i = file.find_last_of('\\');
	string res=file.substr(0, i);
	return res;
}

