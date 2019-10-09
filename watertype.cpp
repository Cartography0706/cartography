
#include "stdafx.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
#include "math.h"





string shen = "07";
string yi = "a";
string lang = "06";


string lujingbuilding = "experarea/bigtestthree/" + shen + "/region" + shen + "_" + yi + "water" + lang + ".bmp";
string centrepointbuilding = "experarea/bigtestthree/" + shen + "/region" + shen + "_" + yi + "centrePoint" + lang + ".bmp";
string outbuilding = "experarea/bigtestthree/" + shen + "/region" + shen + "_" + yi + "water" + lang + "shenyilang.bmp";
string markbuilding = "experarea/bigtestthree/" + shen + "/region" + shen + "_" + yi + "water" + lang + "s.jpg";
string markbuilding2 = "experarea/bigtestthree/" + shen + "/region" + shen + "_" + yi + "water" + lang + "o.bmp";
string markbuilding3 = "experarea/bigtestthree/" + shen + "/region" + shen + "_" + yi + "water" + lang + "p.bmp";



double averarea()
{
	Mat src; Mat src_gray;
	int thresh = 30;
	int max_thresh = 255;
	RNG rng(12345);
	src = imread(lujingbuilding, CV_LOAD_IMAGE_COLOR);
	cvtColor(src, src_gray, CV_BGR2GRAY);//灰度化
	GaussianBlur(src, src, Size(3, 3), 0.1, 0, BORDER_DEFAULT);
	blur(src_gray, src_gray, Size(3, 3)); //滤波
	//namedWindow("image", CV_WINDOW_AUTOSIZE);
	//imshow("image", src);
	//moveWindow("image", 20, 20);
	//定义Canny边缘检测图像
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//利用canny算法检测边缘
	Canny(src_gray, canny_output, thresh, thresh * 3, 3);
	//namedWindow("canny", CV_WINDOW_AUTOSIZE);
	//imshow("canny", canny_output);
	//moveWindow("canny", 550, 20);

	//查找轮廓
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//计算轮廓矩
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}
	//计算轮廓的质心
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2d(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	//画轮廓及其质心并显示
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	printf("\t\t features\n");


	//旋转角度
	double angle;
	double ceterx;
	double cetery;
	double sumarea = 0;
	double avearea;

	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(255, 255, 255);
		Scalar color2 = Scalar(0, 0, 255);

		drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 4, color2, -1, 8, 0);
		//rectangle(drawing, boundingRect(contours.at(i)), cvScalar(0, 255, 0));


		printf("object%d - area:%.2f - length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength(contours[i], true));




		double area = contourArea(contours[i]);//面积





		sumarea = sumarea + area;


		i = i + 1;
	}
	avearea = sumarea / (contours.size() / 2);
	printf("aveangle:%.2f ", avearea);







	src.release();
	src_gray.release();
	return avearea;


}





int zuixiaowanjiejuxing(double aveangle, double hei, double wid, double centerx, double centery)
{


	Mat 原图 = imread(lujingbuilding);
	//imshow("原图", 原图);
	Mat 灰度图, 二值图;
	cvtColor(原图, 二值图, CV_BGR2GRAY);
	//blur(灰度图, 二值图, Size(5, 5));//模糊一下，可以不要
	threshold(二值图, 二值图, 0, 255, CV_THRESH_OTSU);//自适应二值化
	//二值图 = 255 - 二值图;//颜色反转
	//imshow("二值图", 二值图);

	//寻找最外层轮廓  
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(二值图, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point());

	Mat 画布 = Mat::zeros(二值图.size(), CV_8UC1); //最小外接矩形画布  
	for (int i = 0; i<contours.size(); i++)
	{
		//绘制轮廓  
		drawContours(画布, contours, i, Scalar(255), 1, 8, hierarchy);

		//绘制轮廓的最小外结矩形  
		RotatedRect rect = minAreaRect(contours[i]);


		RotatedRect rect2 = RotatedRect(Point2f(centery, centerx), Size2f(hei, wid), aveangle);


		//rectangle(画布, rect2.boundingRect(), Scalar(55));
		Point2f P[4];

		rect2.points(P);
		for (int j = 0; j <= 3; j++)
		{
			line(原图, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 2);
			line(画布, P[j], P[(j + 1) % 4], Scalar(111), 2);
		}

		/*
		//绘制轮廓的最小外结圆
		Point2f center; float radius;
		minEnclosingCircle(contours[i], center, radius);
		circle(画布1, center, radius, Scalar(255), 2);
		*/

	}
	//imshow("最小外接矩形", 画布);
	//imshow("标注出矩形",原图 );
	cv::imwrite(outbuilding, 原图);

	waitKey(0);
	return 0;



}












int main(int argc, char** argv)
{
	double centrepointx = 0;
	double centrepointy = 0;

	Mat srccentrepoint = imread(centrepointbuilding);

	//cv::imshow("dst", srccentrepoint);

	for (int row = 0; row < srccentrepoint.rows; row++)
	{
		for (int col = 0; col < srccentrepoint.cols; col++)
		{
			if (srccentrepoint.at<Vec3b>(row, col)[2] == 255 && srccentrepoint.at<Vec3b>(row, col)[1] == 255 && srccentrepoint.at<Vec3b>(row, col)[0] == 255)
			{
				centrepointx = row;
				centrepointy = col;

			}


		}
	}


	/*double aveangle = ju();*/
	double maxangle = 0;
	double avearea = averarea();









	Mat src = imread(lujingbuilding);

	int largest_area = 0;
	int largest_contour_index = 0;
	Rect bounding_rect;
	RotatedRect rect;
	double hei = 0;
	double wid = 0;
	double xhei = 0;
	double xwid = 0;
	Mat thr;
	cvtColor(src, thr, COLOR_BGR2GRAY); //Convert to gray
	threshold(thr, thr, 125, 255, THRESH_BINARY); //Threshold the gray
	//bitwise_not(thr, thr); //这里先变反转颜色

	vector<vector<Point> > contours; // Vector for storing contours

	findContours(thr, contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE); // Find the contours in the image

	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}
	//计算轮廓的质心
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2d(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}





	CvPoint offset;


	for (size_t i = 0; i< contours.size(); i++) // iterate through each contour.
	{
		double area = contourArea(contours[i]);  //  Find the area of contour

		if (area > largest_area)
		{
			largest_area = area;
			largest_contour_index = i;               //Store the index of largest contour
			bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
			rect = minAreaRect(contours[i]);
			hei = rect.size.height;
			wid = rect.size.width;
			 
			double tiaohe = 1;// 
			 
			double x;
		 

			x = sqrt(largest_area*tiaohe / (hei*wid));


			// x = sqrt(avearea*tiaohe / (hei*wid));

			xhei = hei*x;
			xwid = x*wid;


			RotatedRect r = fitEllipse(contours[i]);
			maxangle = r.angle - 0;

			offset = cvPoint(centrepointy - mu[i].m10 / mu[i].m00, centrepointx - mu[i].m01 / mu[i].m00);


		}
	}
	drawContours(src, contours, largest_contour_index, Scalar(0, 255, 0), 2, 8, noArray(), INT_MAX, offset); // Draw the largest contour using previously stored index.
	printf("hei:%.2f ", hei);
	printf("wid:%.2f ", wid);

	imshow("result", src);
	cv::imwrite(outbuilding, src);



	waitKey(2000);
	return 0;

}
