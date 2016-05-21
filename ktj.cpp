// ktj.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include "tserial.h"
#include "bot_control.h"
#include <iostream>
#include <time.h>  
using namespace cv;
using namespace std;
#define angle_error 5
#define dist_error 10
#define mark1r 255
#define mark1g 0
#define mark1b 0
#define mark2r 0
#define mark2g 255
#define mark2b 0
#define maxcol 0.08
#define mincol 0.05
#define arra 10
#define black1 70
#define black2 120
int nums[10] = { 0 };
char chars[10] = { 0 };
float integers[200][10] = { 0 };
char characters[40][10] = { 0 };
float integer[200][10] = { 0 };
char character[40][10] = { 0 };
int max_int[10] = { 0 };
char max_char[10] = { 0 };
float ans[600] = { 0 };
int z1 = 0;
int k = 0;
int l = 0;
int tempo = 0;

struct pos{
	int x;
	int y;
};
struct number{
	int x;
	int y;
	int num;
};
struct charac{
	int x;
	int y;
	char cha;
};

serial comm;
// Serial comm end ====================================
float bot_angle=0;
struct pos bot_pos ;
float angle[100] = { 1 };
int ang_no = 0;
float dist = 0;
struct pos order[30];
int thresh = 100;
Mat threshold_output;

//Serial com ....................
void start_serial(char* com_name)
{
	comm.startDevice(com_name, 9600);
}

void stop_serial()
{
	comm.stopDevice();
}

void send_serial(char data)
{
	comm.send_data(data);
}
// Serial comm end =====================================
void send_command()
{	
	if (dist < dist_error)
	{
		send_serial('w');
		send_serial('l');
		ang_no++;
	}
	else
	{
		if (bot_angle < angle[ang_no] - angle_error) send_serial('l');
		else if (bot_angle > angle[ang_no] + angle_error) send_serial('r');
		else send_serial('w');

	}
}
void recbox()
{


}
Mat binary(Mat src_gray)
{
	for (int i = 0; i <src_gray.rows; i++)
	{
		for (int j = 0; j < src_gray.cols; j++)
		{


			if (src_gray.at<uchar>(i, j) != 255 && src_gray.at<uchar>(i, j) != 139)
				src_gray.at<uchar>(i, j) = 0;
			else
				src_gray.at<uchar>(i, j) = 255;

		}
	}
	return src_gray;
}
Mat  ProjectedHistogram(Mat img)
{
	Mat temp(img.rows, img.cols + img.rows, CV_8UC1);
	for (int x = 0; x < img.cols; x++)
	{
		int count = 0;
		for (int y = 0; y < img.rows; y++)
		{
			if (img.at<uchar>(y, x) != 0)
				count++;
		}
		for (int y = 0; y < img.rows; y++)
		{
			if (count != 0)
			{
				temp.at<uchar>(y, x) = 255;
				count--;
			}
			else
				temp.at<uchar>(y, x) = 0;

		}
	}
	int z = img.cols;
	for (int x = 0; x < img.rows; x++)
	{
		int count = 0;
		for (int y = 0; y < img.cols; y++)
		{
			if (img.at<uchar>(x, y) != 0)
				count++;
		}
		//	cout << count << endl;

		{
			for (int y = 0; y < img.rows; y++)
			{
				if (count != 0)
				{
					temp.at<uchar>(y, z) = 255;
					count--;
				}
				else
				{
					temp.at<uchar>(y, z) = 0;
				}
			}
		}
		z++;
	}
	return temp;

}
Mat  ProjectedHistogram1(Mat img)
{
	Mat temp(img.rows, img.rows, CV_8UC1);
	int z = 0;
	for (int x = 0; x < img.rows; x++)
	{
		int count = 0;
		for (int y = 0; y < img.cols*0.25; y++)
		{
			if (img.at<uchar>(x, y) != 0)
				count++;
		}
		//	cout << count << endl;

		{
			for (int y = 0; y < img.rows; y++)
			{
				if (count != 0)
				{
					temp.at<uchar>(y, z) = 255;
					count--;
				}
				else
				{
					temp.at<uchar>(y, z) = 0;
				}
			}
		}
		z++;
	}
	return temp;

}
Mat  ProjectedHistogram2(Mat img)
{
	Mat temp(img.rows, img.rows, CV_8UC1);
	int z = 0;
	for (int x = 0; x < img.rows; x++)
	{
		int count = 0;
		for (int y = img.cols - 1; y >= img.cols*0.75; y--)
		{
			if (img.at<uchar>(x, y) != 0)
				count++;
		}
		//	cout << count << endl;

		{
			for (int y = 0; y < img.rows; y++)
			{
				if (count != 0)
				{
					temp.at<uchar>(y, z) = 255;
					count--;
				}
				else
				{
					temp.at<uchar>(y, z) = 0;
				}
			}
		}
		z++;
	}
	return temp;

}
Mat  ProjectedHistogram3(Mat img)
{
	Mat temp(img.rows, img.rows, CV_8UC1);
	int z = 0;
	for (int x = 0; x < img.rows; x++)
	{
		int count = 0;
		for (int y = 0; y <= img.cols*0.25; y++)
		{
			if (img.at<uchar>(x, y) != 0)
				count++;
		}
		//	cout << count << endl;

		{
			for (int y = 0; y < img.rows; y++)
			{
				if (count != 0)
				{
					temp.at<uchar>(y, z) = 255;
					count--;
				}
				else
				{
					temp.at<uchar>(y, z) = 0;
				}
			}
		}
		z++;
	}
	return temp;

}
inline void swap(int &i, int &j)
{
	int t = i;
	i = j;
	j = t;
}
inline void swap_char(char &i, char &j)
{
	char t = i;
	i = j;
	j = t;
}
int factorial(int x)
{
	if (x < 1)return 1;
	else return(x *factorial(x - 1));
}
// full permutation
void perm(int n, int i)
{
	if (i == (n - 1)) //check if reach the end of iteration
	{
		// print out the iteration
		for (int j = 0; j < n; j++)
		{
			//cout<< nums[j];
			integers[k][l] = nums[j];
			l = l + 1;
		}
		//cout <<endl;
		l = 0;
		k = k + 1;
	}
	else
	{
		for (int j = i; j < n; j++)
		{
			swap(nums[i], nums[j]); // swap
			perm(n, i + 1);         // recursive
			swap(nums[i], nums[j]); // swap it back
		}
	}
}
void perm_char(int n, int i)
{
	if (i == (n - 1)) //check if reach the end of iteration
	{
		// print out the iteration
		for (int j = 0; j < n; j++)
		{
			//cout << chars[j];
			characters[k][l++] = chars[j];
		}
		l = 0;
		k = k + 1;
	}
	else
	{
		for (int j = i; j < n; j++)
		{
			swap_char(chars[i], chars[j]); // swap
			perm_char(n, i + 1);         // recursive
			swap_char(chars[i], chars[j]); // swap it back
		}
	}
}
void result(int g, int h, int x, int temp1)
{
	if (x == 1)
	{
		ans[temp1] = integers[g][0];
		//cout << integers[g][0]<<endl;
		//cout << sum[temp1][0]<<endl;
		return;
	}
	else
	{
		int i = 0;
		for (int i = 0; i < x - 1; i++)
		{
			if (characters[h][i] == '/')
			{
				integers[g][i] = integers[g][i] / integers[g][i + 1];
				for (int j = i + 1; j < x - 1; j++)
				{
					integers[g][j] = integers[g][j + 1];
				}
				for (int j = i; j < x - 2; j++)
				{
					characters[h][j] = characters[h][j + 1];
				}
				result(g, h, x - 1, temp1);
				return;
			}
		}
		for (int i = 0; i < x - 1; i++)
		{
			if (characters[h][i] == '*')
			{
				integers[g][i] = integers[g][i] * integers[g][i + 1];
				for (int j = i + 1; j < x - 1; j++)
				{
					integers[g][j] = integers[g][j + 1];
				}
				for (int j = i; j < x - 2; j++)
				{
					characters[h][j] = characters[h][j + 1];
				}
				result(g, h, x - 1, temp1);
				return;
			}
		}
		for (int i = 0; i < x - 1; i++)
		{
			if (characters[h][i] == '-')
			{
				integers[g][i] = integers[g][i] - integers[g][i + 1];
				for (int j = i + 1; j < x - 1; j++)
				{
					integers[g][j] = integers[g][j + 1];
				}
				for (int j = i; j < x - 2; j++)
				{
					characters[h][j] = characters[h][j + 1];
				}
				result(g, h, x - 1, temp1);
				return;
			}
		}
		for (int i = 0; i < x - 1; i++)
		{
			if (characters[h][i] == '+')
			{
				integers[g][i] = integers[g][i] + integers[g][i + 1];
				for (int j = i + 1; j < x - 1; j++)
				{
					integers[g][j] = integers[g][j + 1];
				}
				for (int j = i; j < x - 2; j++)
				{
					characters[h][j] = characters[h][j + 1];
				}
				result(g, h, x - 1, temp1);
				return;
			}
		}
	}
}
void findmax(int n)
{
	for (int i = 0; i < factorial(n); i++)
	{
		for (int j = 0; j < n; j++)
		{
			//cout << integers[i][j];
			integer[i][j] = integers[i][j];
		}
		//cout << endl;
	}
	for (int i = 0; i < factorial(n - 1); i++)
	{
		for (int j = 0; j < n - 1; j++)
		{
			//cout << characters[i][j];
			character[i][j] = characters[i][j];
		}
		//	cout << endl;
	}
	int max = -5000;
	for (int i = 0; i < factorial(n); i++)
	{
		for (int j = 0; j <factorial(n - 1); j++)
		{
			result(i, j, n, tempo);
			if (ans[tempo]>max)
			{
				max = ans[tempo];
				for (int g = 0; g< n; g++)
				{
					max_int[z1++] = integer[i][g];
					max_char[z1++] = character[j][g];
				}
			}
			for (int i = 0; i < factorial(n); i++)
			{
				for (int j = 0; j < n; j++)
				{
					integers[i][j] = integer[i][j];

				}
			}
			for (int i = 0; i < factorial(n - 1); i++)
			{
				for (int j = 0; j < n - 1; j++)
				{
					characters[i][j] = character[i][j];

				}
			}
			//cout << sum[temp]<<endl;
			z1 = 0;
		}
	}

}
Mat filter(Mat image1)
{
	for (int r = 0; r < image1.rows; r++)
	for (int c = 0; c < image1.cols; c++)
	{
		Vec3b rgb = image1.at<Vec3b>(r, c);
		if (rgb.val[0] < black2&&rgb.val[1] < black2&&rgb.val[2] < black2 && (rgb.val[0] - rgb.val[1]) < black1 && (rgb.val[1] - rgb.val[0]) < black1 && (rgb.val[2] - rgb.val[1]) < black1 && (rgb.val[1] - rgb.val[2]) < black1 && (rgb.val[2] - rgb.val[0]) < black1 && (rgb.val[0] - rgb.val[2]) < black1)
		{
			image1.at<Vec3b>(r, c)[0] = 255;
			image1.at<Vec3b>(r, c)[1] = 255;
			image1.at<Vec3b>(r, c)[2] = 255;

		}
		else
		{
			image1.at<Vec3b>(r, c)[0] = 0;
			image1.at<Vec3b>(r, c)[1] = 0;
			image1.at<Vec3b>(r, c)[2] = 0;
		}
	}
	return image1;
}
Mat  ProjectedHistogramfirsthalf(Mat img)
{
	Mat temp(img.rows, img.rows, CV_8UC1);
	int z = 0;
	for (int x = 0; x < img.rows; x++)
	{
		int count = 0;
		for (int y = 0; y <= img.cols*0.50; y++)
		{
			if (img.at<uchar>(x, y) != 0)
				count++;
		}
		//	cout << count << endl;

		{
			for (int y = 0; y < img.rows; y++)
			{
				if (count != 0)
				{
					temp.at<uchar>(y, z) = 255;
					count--;
				}
				else
				{
					temp.at<uchar>(y, z) = 0;
				}
			}
		}
		z++;
	}
	return temp;

}
Mat  ProjectedHistogramother(Mat img)
{
	Mat temp(img.rows, img.rows, CV_8UC1);
	int z = 0;
	for (int x = 0; x < img.rows; x++)
	{
		int count = 0;
		for (int y = img.cols - 1; y >= img.cols*0.50; y--)
		{
			if (img.at<uchar>(x, y) != 0)
				count++;
		}
		//	cout << count << endl;

		{
			for (int y = 0; y < img.rows; y++)
			{
				if (count != 0)
				{
					temp.at<uchar>(y, z) = 255;
					count--;
				}
				else
				{
					temp.at<uchar>(y, z) = 0;
				}
			}
		}
		z++;
	}
	return temp;

}
Mat  ProjectedHistogramthree(Mat img)
{
	Mat temp(img.rows, img.rows, CV_8UC1);
	int z = 0;
	for (int x = 0; x < img.rows; x++)
	{
		int count = 0;
		for (int y = 0; y <= img.cols*0.30; y++)
		{
			if (img.at<uchar>(x, y) != 0)
				count++;
		}
		//	cout << count << endl;

		{
			for (int y = 0; y < img.rows; y++)
			{
				if (count != 0)
				{
					temp.at<uchar>(y, z) = 255;
					count--;
				}
				else
				{
					temp.at<uchar>(y, z) = 0;
				}
			}
		}
		z++;
	}
	return temp;

}
int isvalid(int i, int j, Mat img)
{
	int s = 1;
	if ((i + s) < img.rows && (i - s) >= 0 && (j + s) < img.cols && (j - s) >= 0)
		return 0;
	else
		return 1;
}
Mat erosion(Mat img)
{
	Mat temp = img.clone();
	int is;
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			is = isvalid(i, j, img);
			int bla = 0, q;
			if (is == 0)
			{
				int p = 0;
				for (int k = -1; k <= 1; k++)
				{
					for (int z = -1; z <= 1; z++)
					{
						q = img.at<uchar>(i + k, j + z);

						if (q == 0)
							bla++;
					}
				}
				if (bla > 0)
					temp.at<uchar>(i, j) = 0;
			}
		}
	}
	return temp;
}
Mat dillation(Mat img)
{
	Mat temp = img.clone();
	int is;
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			is = isvalid(i, j, img);
			int wh = 0, q;

			if (is == 0)
			{

				int p = 0;
				for (int k = -1; k <= 1; k++)
				{
					for (int z = -1; z <= 1; z++)
					{
						q = img.at<uchar>(i + k, j + z);

						if (q == 255)
							wh++;
					}
				}
				if (wh > 0)
					temp.at<uchar>(i, j) = 255;
			}
		}
	}
	return temp;

}
int daduki(Mat imageseg)
{
	//	cout << "asd";
	int bl = 0, wt = 0, six = 0, nine = 0, three = 0,ret;
	//	namedWindow("first", CV_WINDOW_AUTOSIZE);
	//	namedWindow("second", CV_WINDOW_AUTOSIZE);
	Mat first = ProjectedHistogramfirsthalf(imageseg);
	Mat second = ProjectedHistogramother(imageseg);
	Mat the = ProjectedHistogramthree(imageseg);
	//	imshow("first", first);
	//	imshow("second", the);

	int i = 0;
	{
		for (int j = 0.25*first.cols; j < first.cols - 1; j++)
		{
			if (first.at<uchar>(i, j) == 255)
				wt++;
			else if (first.at<uchar>(i, j) == 0 && wt >= 2)
			{
				bl++;  wt = 0;
			}


		}
	}

	if (bl>0)
		nine++;
	bl = 0;
	wt = 0;
	{
		for (int j = 1; j < the.cols*0.45; j++)
		{
			if (the.at<uchar>(i, j) == 255)
				wt++;
			else if (the.at<uchar>(i, j) == 0 && wt >= 2)
			{
				bl++;  wt = 0;
			}


		}
	}
	if (bl > 0)
		three++;
	bl = 0;
	wt = 0;
	{
		for (int j = 1; j < second.cols*0.75; j++)
		{
			if (second.at<uchar>(i, j) == 255)
				wt++;
			else if (second.at<uchar>(i, j) == 0 && wt >= 2)
			{
				bl++;   wt = 0;
			}


		}
	}

	if (bl > 0)
		six++;
	if (six > 0 && nine > 0)
	{
		//cout << "5";
		ret=5;
		return ret;
	}
	else if (six > 0)
	{
		ret = 6;
		return ret;
	}
	else	 if (nine > 0)
	{
		if (three > 0)
		{
			ret = 3;
			return ret;
		}
		else
		{
			ret = 9;
			return ret;

		}
	}
	else if (six == 0 && nine == 0)
	{
		ret = 8;
		return ret;
	}
	else
		return 0;
}
void arena(Mat src)
{

	src = filter(src);
	Mat src_gray;
	struct charac strchar[10];
	struct number strnum[10];
	for (int i = 0; i < 10; i++)
	{
		strnum[i].num = 0;
		strnum[i].x = 0;
		strnum[i].y = 0;
		strchar[i].cha = '0';
		strchar[i].x = 0;
		strchar[i].y = 0;
	}
	int countchar = 0, countnum = 0;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	cvtColor(src, src_gray, CV_BGR2GRAY);
	src_gray = binary(src_gray);
	Mat drawing = src.clone();
	//	threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);
	Mat src_gray1 = src_gray.clone();
	findContours(src_gray1, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	//cout << contours.size()<<endl;
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));

	}

	for (int i = 0; i < contours.size(); i++)
	{

		Scalar color = Scalar(255, 255, 255);
		//	rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 1, 8, 0);
		//	recbox();
		//	waitKey(0);
		if (boundRect[i].height < maxcol*src.rows&&boundRect[i].height > mincol*src.rows)
		{

			Mat seg(boundRect[i].br().y - boundRect[i].tl().y - 2, boundRect[i].br().x - boundRect[i].tl().x - 2, CV_8UC1);

			for (int p = boundRect[i].tl().x + 1; p < boundRect[i].br().x - 1; p++)
			{
				for (int q = boundRect[i].tl().y + 1; q < boundRect[i].br().y - 1; q++)
				{
					seg.at<uchar>(q - boundRect[i].tl().y - 1, p - boundRect[i].tl().x - 1) = src_gray.at<uchar>(q, p);
				}
			}
			namedWindow("Contour", CV_WINDOW_AUTOSIZE);
			imshow("Contour", seg);
			vector<vector<Point> > con;
			vector<Vec4i> hierarch;
			vector<vector<Point> > contours_poly1(contours.size());
			vector<Rect> boundRect1(contours.size());
			findContours(seg, con, hierarch, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

			for (int xc = 0; xc < con.size(); xc++)
			{
				approxPolyDP(Mat(con[xc]), contours_poly1[xc], 3, true);
				boundRect1[xc] = boundingRect(Mat(contours_poly1[xc]));

			}
			
			for (int po = 0; po < con.size(); po++)
			{

				rectangle(drawing, boundRect1[po].tl() + boundRect[i].tl(), boundRect[i].tl() + boundRect1[po].br(), color, 1, 8, 0);
				//histogram


				Mat seg1(boundRect1[po].br().y - boundRect1[po].tl().y + 1, boundRect1[po].br().x - boundRect1[po].tl().x + 1, CV_8UC1);
				for (int r = boundRect1[po].tl().x; r <= boundRect1[po].br().x; r++)
				{
					for (int s = boundRect1[po].tl().y; s <= boundRect1[po].br().y; s++)
					{
						seg1.at<uchar>(s - boundRect1[po].tl().y, r - boundRect1[po].tl().x) = src_gray.at<uchar>(s + boundRect[i].tl().y, r + boundRect[i].tl().x);
					}
				}
				Mat vhist = ProjectedHistogram(seg1);
				Mat lowData;
				resize(vhist, lowData, Size(50, 50));

				namedWindow("Contou", CV_WINDOW_AUTOSIZE);
				imshow("Contou", lowData);
				//	imwrite("im(div).jpg", lowData);

				short int i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0, i6 = 0, i7 = 0, i8 = 0, i9 = 0, ipl = 0, imi = 0, imu = 0, idi = 0;
				Mat im1 = imread("im(1).jpg", 0);
				Mat im2 = imread("im(2).jpg", 0);
				Mat im3 = imread("im(3).jpg", 0);
				Mat im4 = imread("im(4).jpg", 0);
				Mat im5 = imread("im(5).jpg", 0);
				Mat im6 = imread("im(6).jpg", 0);
				Mat im7 = imread("im(7).jpg", 0);
				Mat im8 = imread("im(8).jpg", 0);
				Mat im9 = imread("im(9).jpg", 0);
				Mat impl = imread("im(plus).jpg", 0);
				Mat immi = imread("im(minus).jpg", 0);
				Mat immu = imread("im(mul).jpg", 0);
				Mat imdi = imread("im(div).jpg", 0);


				for (int x = 0; x < lowData.rows; x++)
				{
					for (int y = 0; y < lowData.cols; y++)
					{

						if ((lowData.at<uchar>(x, y) != 0 && im1.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && im1.at<uchar>(x, y) == 0))
							i1++;
						if ((lowData.at<uchar>(x, y) != 0 && im2.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && im2.at<uchar>(x, y) == 0))
							i2++;
						if ((lowData.at<uchar>(x, y) != 0 && im3.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && im3.at<uchar>(x, y) == 0))
							i3++;
						if ((lowData.at<uchar>(x, y) != 0 && im4.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && im4.at<uchar>(x, y) == 0))
							i4++;
						if ((lowData.at<uchar>(x, y) != 0 && im5.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && im5.at<uchar>(x, y) == 0))
							i5++;
						if ((lowData.at<uchar>(x, y) != 0 && im6.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && im6.at<uchar>(x, y) == 0))
							i6++;
						if ((lowData.at<uchar>(x, y) != 0 && im7.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && im7.at<uchar>(x, y) == 0))
							i7++;
						if ((lowData.at<uchar>(x, y) != 0 && im8.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && im8.at<uchar>(x, y) == 0))
							i8++;
						if ((lowData.at<uchar>(x, y) != 0 && im9.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && im9.at<uchar>(x, y) == 0))
							i9++;
						if ((lowData.at<uchar>(x, y) != 0 && impl.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && impl.at<uchar>(x, y) == 0))
							ipl++;
						if ((lowData.at<uchar>(x, y) != 0 && immi.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && immi.at<uchar>(x, y) == 0))
							imi++;
						if ((lowData.at<uchar>(x, y) != 0 && immu.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && immu.at<uchar>(x, y) == 0))
							imu++;
						if ((lowData.at<uchar>(x, y) != 0 && imdi.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && imdi.at<uchar>(x, y) == 0))
							idi++;
					}
				}

				if (i5 >= i3&&i5 >= i6&&i5 >= i4&&i5 >= i2&&i5 >= i1&&i5 >= i7&&i5 >= i8&&i5 >= i9&&i5 >= imu&&i5 >= idi&&i5 >= imi&&i5 >= ipl)
				{
					int getnum = daduki(seg1);
					printf("%d \n",getnum);
					strnum[countnum].num = getnum;
					strnum[countnum].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
					strnum[countnum].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
					countnum++;
				}
				else if (i3 >= i5&&i3 >= i6&&i3 >= i2&&i3 >= i1&&i3 >= i4&&i3 >= i7&&i3 >= i8&&i3 >= i9&&i3 >= imu&&i3 >= idi&&i3 >= imi&&i3 >= ipl)
				{
					int ii3 = 0, ii9 = 0;
					Mat sa3 = imread("sa3front.jpg", 0);
					Mat sa9 = imread("sa9front.jpg", 0);
					Mat vhist1 = ProjectedHistogram3(seg1);

					resize(vhist1, lowData, Size(50, 50));
					for (int x = 0; x < lowData.rows; x++)
					{
						for (int y = 0; y < lowData.cols; y++)
						{
							if ((lowData.at<uchar>(x, y) != 0 && sa3.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && sa9.at<uchar>(x, y) == 0))
								ii9++;
							if ((lowData.at<uchar>(x, y) != 0 && sa9.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && sa9.at<uchar>(x, y) == 0))
								ii9++;
						}
					}
					if (ii3>ii9)
					{
						int getnum=daduki(seg1);
						printf("%d \n",getnum);
						strnum[countnum].num = getnum;
						strnum[countnum].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
						strnum[countnum].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
						countnum++;
					}
					else
					{
						int getnum = daduki(seg1);
						printf("%d \n",getnum);
						strnum[countnum].num = getnum;
						strnum[countnum].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
						strnum[countnum].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
						countnum++;
					}

				}
				else if (i6 >= i3&&i6 >= i5&&i6 >= i2&&i6 >= i4&&i6 >= i1&&i6 >= i7&&i6 >= i8&&i6 >= i9&&i6 >= imu&&i6 >= idi&&i6 >= imi&&i6 >= ipl)
				{
					int getnum = daduki(seg1);
					printf("%d \n",getnum);
					strnum[countnum].num = getnum;
					strnum[countnum].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
					strnum[countnum].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
					countnum++;
				}
				else if (i4 >= i3&&i4 >= i5&&i4 >= i6&&i4 >= i2&&i4 >= i1&&i4 >= i7&&i4 >= i8&&i4 >= i9&&i4 >= imu&&i4 >= idi&&i4 >= imi&&i4 >= ipl)
				{

					printf("4 \n");
					strnum[countnum].num = 4;
					strnum[countnum].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
					strnum[countnum].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
					countnum++;
				}
				else if (i2 >= i3&&i2 >= i5&&i2 >= i6&&i2 >= i4&&i2 >= i1&&i2 >= i7&&i2 >= i8&&i2 >= i9&&i2 >= imu&&i2 >= idi&&i2 >= imi&&i2 >= ipl)
				{
					int ii2 = 0, ii3 = 0;
					Mat sa2 = imread("sa2.jpg", 0);
					Mat sa3 = imread("sa3.jpg", 0);
					Mat vhist1 = ProjectedHistogram2(seg1);

					resize(vhist1, lowData, Size(50, 50));
					for (int x = 0; x < lowData.rows; x++)
					{
						for (int y = 0; y < lowData.cols; y++)
						{
							if ((lowData.at<uchar>(x, y) != 0 && sa2.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && sa2.at<uchar>(x, y) == 0))
								ii2++;
							if ((lowData.at<uchar>(x, y) != 0 && sa3.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && sa3.at<uchar>(x, y) == 0))
								ii3++;
						}
					}
					if (ii2>ii3)
					{
						printf("2 \n");
						strnum[countnum].num = 2;
						strnum[countnum].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
						strnum[countnum].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
						countnum++;
					}
					else
					{
						int getnum = daduki(seg1);
						printf("%d \n",getnum);
						strnum[countnum].num = getnum;
						strnum[countnum].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
						strnum[countnum].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
						countnum++;
					}
				}
				else if (i1 >= i3&&i1 >= i5&&i1 >= i6&&i1 >= i2&&i1 >= i4&&i1 >= i7&&i1 >= i8&&i1 >= i9&&i1 >= imu&&i1 >= idi&&i1 >= imi&&i1 >= ipl)
				{
					printf("1 \n");
					strnum[countnum].num = 1;
					strnum[countnum].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
					strnum[countnum].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
					countnum++;
				}
				else if (i7 >= i3&&i7 >= i5&&i7 >= i6&&i7 >= i2&&i7 >= i4&&i7 >= i1&&i7 >= i8&&i7 >= i9&&i7 >= imu&&i7 >= idi&&i7 >= imi&&i7 >= ipl)
				{
					printf("7 \n");
					strnum[countnum].num = 7;
					strnum[countnum].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
					strnum[countnum].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
					countnum++;
				}
				else if (i8 >= i3&&i8 >= i5&&i8 >= i6&&i8 >= i2&&i8 >= i4&&i8 >= i7&&i8 >= i1&&i8 >= i9&&i8 >= imu&&i8 >= idi&&i8 >= imi&&i8 >= ipl)
				{
					int iix = 0, ii8 = 0;
					Mat sax = imread("sax.jpg", 0);
					Mat sa8 = imread("sa8.jpg", 0);
					Mat vhist1 = ProjectedHistogram1(seg1);

					resize(vhist1, lowData, Size(50, 50));
					for (int x = 0; x < lowData.rows; x++)
					{
						for (int y = 0; y < lowData.cols; y++)
						{
							if ((lowData.at<uchar>(x, y) != 0 && sa8.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && sa8.at<uchar>(x, y) == 0))
								ii8++;
							if ((lowData.at<uchar>(x, y) != 0 && sax.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && sax.at<uchar>(x, y) == 0))
								iix++;
						}
					}
					if (ii8>iix)
					{
						int getnum = daduki(seg1);
						printf("%d \n",getnum);
						strnum[countnum].num = getnum;
						strnum[countnum].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
						strnum[countnum].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
						countnum++;
					}
					else
					{
						printf("x \n");
						strchar[countchar].cha = 'x';
						strchar[countchar].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
						strchar[countchar].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
						countchar++;

					}

				}
				else if (i9 >= i3&&i9 >= i5&&i9 >= i6&&i9 >= i2&&i9 >= i4&&i9 >= i7&&i9 >= i8&&i9 >= i1&&i9 >= imu&&i9 >= idi&&i9 >= imi&&i9 >= ipl)
				{
					int getnum = daduki(seg1);
					printf("%d \n",getnum);
					strnum[countnum].num = getnum;
					strnum[countnum].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
					strnum[countnum].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
					countnum++;
				}
				else if (ipl >= i3&&ipl >= i5&&ipl >= i6&&ipl >= i2&&ipl >= i4&&ipl >= i7&&ipl >= i8&&ipl >= i1&&ipl >= imu&&ipl >= idi&&ipl >= imi&&ipl >= i9)
				{
					printf("+ \n");
					strchar[countchar].cha = '+';
					strchar[countchar].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
					strchar[countchar].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
					countchar++;
				}
				else if (imi >= i3&&imi >= i5&&imi >= i6&&imi >= i2&&imi >= i4&&imi >= i7&&imi >= i8&&imi >= i1&&imi >= imu&&imi >= idi&&imi >= i9&&imi >= ipl)
				{
		/*			if (con.size() >= 3)
					{
						printf("/ \n");
						strchar[countchar].cha = '/';
						strchar[countchar].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
						strchar[countchar].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
						countchar++;
					}

					else   */
					{
						printf("- \n");
						strchar[countchar].cha = '-';
						strchar[countchar].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
						strchar[countchar].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
						countchar++;
					}
				}
				else if (imu >= i3&&imu >= i5&&imu >= i6&&imu >= i2&&imu >= i4&&imu >= i7&&imu >= i8&&imu >= i1&&imu >= i9&&imu >= idi&&imu >= imi&&imu >= ipl)
				{
					int iix = 0, ii8 = 0;
					Mat sax = imread("sax.jpg", 0);
					Mat sa8 = imread("sa8.jpg", 0);
					Mat vhist1 = ProjectedHistogram1(seg1);

					resize(vhist1, lowData, Size(50, 50));
					for (int x = 0; x < lowData.rows; x++)
					{
						for (int y = 0; y < lowData.cols; y++)
						{
							if ((lowData.at<uchar>(x, y) != 0 && sa8.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && sa8.at<uchar>(x, y) == 0))
								ii8++;
							if ((lowData.at<uchar>(x, y) != 0 && sax.at<uchar>(x, y) != 0) || (lowData.at<uchar>(x, y) == 0 && sax.at<uchar>(x, y) == 0))
								iix++;
						}
					}
					if (ii8>iix)
					{
						int getnum = daduki(seg1);
						printf("%d \n",getnum);
						strnum[countnum].num = getnum;
						strnum[countnum].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
						strnum[countnum].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
						countnum++;
					}
					else
					{
						printf("*\n");
						strchar[countchar].cha = '*';
						strchar[countchar].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
						strchar[countchar].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
						countchar++;
					}

				}
				else if (idi >= i3&&idi >= i5&&idi >= i6&&idi >= i2&&idi >= i4&&idi >= i7&&idi >= i8&&idi >= i1&&idi >= i9&&idi >= imu&&idi >= imi&&idi >= ipl)
				{
					printf("/ \n");
					strchar[countchar].cha = '/';
					strchar[countchar].x = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
					strchar[countchar].y = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;
					countchar++;
				}







				// = (boundRect1[po].tl().x + boundRect[i].tl().x + boundRect[i].tl().x + boundRect1[po].br().x) / 2;
				//		yaxis[ct] = (boundRect1[po].tl().y + boundRect[i].tl().y + boundRect[i].tl().y + boundRect1[po].br().y) / 2;

			}
		}

	}                                            //img1-5 img10-3  img11-6 sa-7

	//	imwrite("img1.jpg", drawing);
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);


	for (int asd = 0; asd < countnum; asd++)
	{
		cout << strnum[asd].num << " " << strnum[asd].x << " " << strnum[asd].y << "\n";
		//nums[asd] = strnum[asd].num;
	}
	for (int asd = 0; asd < countchar; asd++)
	{
		cout << strchar[asd].cha << " " << strchar[asd].x << " " << strchar[asd].y << "\n";
		//	chars[asd] = strchar[asd].cha;
	}

	//nums = new int[countnum];
	//chars = new char[countchar];
	for (int i = 0; i<countnum; i++)
	{
		nums[i] = strnum[i].num;
	}
	for (int i = 0; i<countchar; i++)
	{
		chars[i] = strchar[i].cha;
	}
	perm(countnum, 0);
	l = 0;
	k = 0;
	perm_char(countchar, 0);
	findmax(countnum);
	k = 0;
	for (int i = 0; i < countnum + countchar; i = i + 2)
	{
		//cout << max_int[i];
		for (int j = 0; j < countnum; j++)
		{
			if (strnum[j].num == max_int[i])
			{
				order[k].x = strnum[j].x;
				order[k].y = strnum[j].y;
				cout << strnum[j].num << " " << order[k].x << " " << order[k].y << "\n";
				k++;
			}
		}
		for (int j = 0; j < countchar; j++)
		{
			if (strchar[j].cha == max_char[i + 1])
			{
				order[k].x = strchar[j].x;
				order[k].y = strchar[j].y;
				cout << strchar[j].cha << " " << order[k].x << " " << order[k].y << "\n";
				k++;
			}

		}
		//cout << max_char[i + 1];
	}

}



int main()
{
	
	start_serial("COM4");
	namedWindow("my", CV_WINDOW_AUTOSIZE);
	int count = 0;
	bot_pos.x = 1;
	bot_pos.y = 1;
	Mat temp;
	for (int i = 0; i < 30; i++)
	{
		order[i].x = 1;
		order[i].y = 1;
	}
	struct pos mark1, mark2;
	int m1 = 1, m2 = 1;
	mark1.x = 0, mark1.y = 0, mark2.x = 0, mark2.y = 0;
	VideoCapture vid(1);
	//vid.set(CV_CAP_PROP_FRAME_WIDTH, 900);
	//vid.set(CV_CAP_PROP_FRAME_HEIGHT, 900);// The comm port for arduino. Required to start communication
		//cin >> data;
		//cout << data << endl;
		//send_serial('w'); // Example to send the character 
		
	//	Mat temp=imread("img1.png",1);
	while (1)
	{
		vid.read(temp);
		for (int r = 0; r < temp.rows; r++)
		for (int c = 0; c < temp.cols; c++)
		{
			if (temp.at<Vec3b>(r, c)[0] == mark1b&&temp.at<Vec3b>(r, c)[1] == mark1g&&temp.at<Vec3b>(r, c)[2] == mark1r)
			{
				mark1.x = mark1.x + c;
				mark1.y = mark1.y + r;
				m1++;
			}
			if (temp.at<Vec3b>(r, c)[0] == mark2b&&temp.at<Vec3b>(r, c)[1] == mark2g&&temp.at<Vec3b>(r, c)[2] == mark2r)
			{
				mark2.x = mark2.x + c;
				mark2.y = mark2.y + r;
				m2++;
			}
		}
		if (count == 20)
		{
			temp = dillation(temp);
			temp = erosion(temp);
			arena(temp);
		}
		mark1.x = mark1.x / m1;
		mark1.y = mark1.y / m1;
		mark2.x = mark2.x / m2;
		mark2.y = mark2.y / m2;
		//cout << mark1.x << " " << mark1.y << "\n";
		//bot_pos.x = (mark1.x + mark2.x) / 2;
		//bot_pos.y = (mark1.y + mark2.y) / 2;
		//dist = sqrt((bot_pos.x - order[ang_no].x)*(bot_pos.x - order[ang_no].x) + (bot_pos.y - order[ang_no].y)*(bot_pos.y - order[ang_no].y));
		//cout << dist << endl;
		bot_angle = atan((temp.rows - bot_pos.y) / bot_pos.x);
		float zxc = atan((temp.rows - order[ang_no].y) / (1 + order[ang_no].x));
		//cout << zxc <<"\n";
		angle[ang_no] = zxc;
		//send_command();
		imshow("my", temp);
		count++;
		waitKey(50);
	}
	//stop_serial(); // Required at the last after you have send all the commands 
	return(0);
}