#include "qt_static.h"

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

Scalar white = Scalar(255, 255, 255);
uint frames, frame_count;
double t1, t2, dt, fps;
char fps_string[10];
Mat pre_descriptors, descriptors;
vector<KeyPoint> pre_keypoints;
vector<Point2f> pre_image_points;
int cal_count = 0;
vector<vector<Point>> contours;
vector<Vec4i> hierarchy;
QString filename;
bool start_flag = false;
Ptr<SURF> detector = SURF::create(300);
vector<KeyPoint> keypoints;
Ptr<SURF> extractor = SURF::create();
Mat temp_out, input_image_gray, output_warp_image;
Ptr<FlannBasedMatcher > matcher = FlannBasedMatcher::create();
vector<DMatch> match_points;
vector<Point> biggest_contour;

/************************************************************************************************
* 函数名称：qt_static()
* 输入参数说明：QWidget *parent: QWidget 父类指针，主窗口类继承于QMainWindow类
* 输出参数说明：无
* 函数作用说明：主窗口构造函数，构造窗口后，将按钮start禁用
*************************************************************************************************/
qt_static::qt_static(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.start->setEnabled(false);
}

/************************************************************************************************
* 函数名称：~qt_static()
* 输入参数说明：无
* 输出参数说明：无
* 函数作用说明：主窗口析构函数，窗口释放时调用，此程序未进行编写
*************************************************************************************************/
qt_static::~qt_static()
{

}

/************************************************************************************************
* 函数名称：paintEvent()
* 输入参数说明：QPaintEvent *e: QPaintEvent 对象指针
* 输出参数说明：无
* 函数作用说明：QT绘制事件，将Mat矩阵数据显示在label中
*************************************************************************************************/
void qt_static::paintEvent(QPaintEvent * e)
{
	QImage image = QImage((uchar*)(output_image.data), output_image.cols, output_image.rows, QImage::Format_RGB888);
	ui.label->resize(image.size());
	ui.label->setPixmap(QPixmap::fromImage(image));
}

/************************************************************************************************
* 函数名称：read_file_clicked()
* 输入参数说明：无
* 输出参数说明：无
* 函数作用说明：Read File 按钮槽函数，当按下Read File 按钮时执行，读取待处理的文件名
*************************************************************************************************/
void qt_static::read_file_clicked()
{
	filename = QFileDialog::getOpenFileName(this, tr("Choose Image File:"), "", tr("videos (*.avi )"));
	ui.lineEdit->setText(filename);
	ui.start->setEnabled(true);
}

/************************************************************************************************
* 函数名称：start_clicked()
* 输入参数说明：无
* 输出参数说明：无
* 函数作用说明：Start 按钮槽函数，当按下Start 按钮时执行，设置按钮状态，判断文件是否符
  合要求，并开启计时器
*************************************************************************************************/
void qt_static::start_clicked()
{
	if (!start_flag)
	{
		ui.lineEdit->setEnabled(false);
		ui.read_file->setEnabled(false);
		ui.start->setEnabled(false);
		connect(&t, &QTimer::timeout, this, &qt_static::updateImage);//将计时器与更新画面函数绑定
		videoCap.open(filename.toStdString());//由文件名打开视频文件

		//判断视频是否成功读取，矩阵大小是否为640x480									  
		if (videoCap.isOpened() && videoCap.get(CAP_PROP_FRAME_HEIGHT) == 480 && videoCap.get(CAP_PROP_FRAME_WIDTH) == 640)
		{
			frames = videoCap.get(CAP_PROP_FRAME_COUNT);
			input_image = Mat::zeros(480, 640, CV_8UC3);
			pre_input_image = Mat::zeros(480, 640, CV_8UC1);
			t.start(10);//10ms计时器开始
			start_flag = true;
		}
		else//若文件不符合要求，弹出提示框
		{
			QMessageBox::information(NULL, "Notice", "Can't open this file.", QMessageBox::Yes);
			ui.start->setEnabled(false);
			ui.read_file->setEnabled(true);
			ui.lineEdit->setEnabled(true);
		}
	}
}

/************************************************************************************************
* 函数名称：exit_clicked()
* 输入参数说明：无
* 输出参数说明：无
* 函数作用说明：Exit 按钮槽函数，当按下Exit 按钮时执行，退出程序
*************************************************************************************************/
void qt_static::exit_clicked()
{
	QApplication* app;
	app->exit(0);
}

/************************************************************************************************
* 函数名称：updateImage()
* 输入参数说明：无
* 输出参数说明：无
* 函数作用说明：画面更新函数，读取视频帧序列，通过SURF特征点检测和匹配，计算前后帧仿
射矩阵，并对前帧仿射变换以进行运动补偿，之后对前后帧进行差分运算，得
到实际图像变化的区域，二值化、形态学操作后，在ROI区域中选取具有最多轮
廓点的轮廓，认定位待检测目标，并用矩形将其包围
*************************************************************************************************/
void qt_static::updateImage()
{
	if (start_flag)
	{
		if (cal_count == 0)
			t1 = (double)getTickCount();
		output_warp_image = Mat::zeros(Size(640, 480), CV_8UC1);
		videoCap >> input_image;
		cal_count++;
	}
	else
	{
		frame_count = 0;
		cal_count = 0;
	}

	if (input_image.data && frame_count < frames)
	{
		frame_count++;
		cvtColor(input_image, input_image_gray, CV_BGR2GRAY);
		if (cal_count == 5)//每5帧进行一次计算
		{
			detector->detect(input_image_gray, keypoints, Mat());//使用SURF检测当前帧特征点，储存于keypoints中
			extractor->compute(input_image_gray, keypoints, descriptors);//描述每一个特征点，得到描述矩阵descriptors
			if (pre_descriptors.data && descriptors.data)
			{
				matcher->match(pre_descriptors, descriptors, match_points);//使用FlannBasedMatcher进行特征点匹配，匹配后的特征点对储存于match_points中
				sort(match_points.begin(), match_points.end());//对match_points中的特征点对进行排序，其值越小，则准确度越高

				float k = 0.4;
				if (match_points.size() < 10)//若匹配的特征点小于10个，则取前80%作为较好的匹配点对
					k = 0.8;
				vector<Point2f> pre_image_points, image_points;
				for (int i = 0;i < k*match_points.size();i++)//取match_points中的前40%或80%的特征点作为较好的匹配点对
				{
					pre_image_points.push_back(pre_keypoints[match_points[i].queryIdx].pt);
					image_points.push_back(keypoints[match_points[i].trainIdx].pt);
				}

				Mat warp_mat;
				if (image_points.size() > 3)
				{
					warp_mat = estimateRigidTransform(pre_image_points, image_points, 1);//由经过筛选的较好的匹配点对获得图像仿射矩阵
					if (warp_mat.rows == 2 && warp_mat.cols == 3)//判断仿射矩阵是否合法
					{
						warpAffine(pre_input_image, output_warp_image, warp_mat, Size(640, 480));//对前一帧矩阵作由上一步得到的仿射变换，可得到运动补偿后的矩阵
						threshold(input_image_gray - output_warp_image, temp_out, 6, 255, THRESH_BINARY);//当前帧与运动补偿后的前一帧进行差分，并二值化，可得到实际发生运动的物体的大致信息
						medianBlur(temp_out, temp_out, 3);//中值滤波
						Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));//腐蚀操作，去除微小的、离散的轮廓点，减少干扰
						morphologyEx(temp_out, temp_out, MORPH_ERODE, element);
						element = getStructuringElement(MORPH_RECT, Size(5, 5));//膨胀操作，将经过腐蚀后仍存在的、可能为目标的轮廓点扩大，有利于检测
						morphologyEx(temp_out, temp_out, MORPH_DILATE, element);
						Mat ROI = temp_out(Rect(150, 200, 400, 180));//由先验知识可知，目标大致处于画面的中部偏下部分，设置ROI，可减少检测误差，提高检测效率
						findContours(ROI, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));//在ROI中寻找轮廓
					}
				}
			}

			pre_input_image = input_image_gray.clone();//将当前帧保存为前一帧，以进行下一次计算
			pre_descriptors = descriptors;//保存当前描述子矩阵
			pre_keypoints.clear();//清空前关键点vector
			pre_keypoints = keypoints;//保存当前关键点vector

			//fps
			if (cal_count == 5)//每5帧画面计算一次总时间，并计算帧率
			{
				dt = (t1 - t2) / getTickFrequency();
				fps = 5.0 / dt;
				sprintf(fps_string, "%d", (uint)fps);
				ui.label_3->setText(fps_string);
				t2 = t1;
			}
			cal_count = 0;//清零帧计数器
		}

		output_image = input_image;

		for (int i = 0; i < contours.size(); i++)//获得拥有最多点数的轮廓点，认为是待检测目标
			biggest_contour = contours[i].size() > biggest_contour.size() ? contours[i] : biggest_contour;
		for (int i = 0;i < biggest_contour.size();i++)//平移轮廓坐标，由于轮廓是在ROI中寻找得到的，需转换坐标得到原图像中的轮廓坐标
		{
			biggest_contour[i].x += 145;
			biggest_contour[i].y += 200;
		}

		if (biggest_contour.size())//判断轮廓是否存在
			rectangle(output_image, boundingRect(biggest_contour), cvScalar(220, 0, 150), 2);//在原图像中画出包围目标轮廓的矩形

		if (frame_count == frames)//若视频序列读完，重置按钮，重置计数器等善后操作，等待对下一个视频进行检测
		{
			ui.read_file->setEnabled(true);
			ui.start->setEnabled(true);
			ui.lineEdit->setEnabled(true);
			frame_count = 0;
			start_flag = false;
		}
	}
	biggest_contour.clear();
	this->update();
}
