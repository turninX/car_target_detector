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
* �������ƣ�qt_static()
* �������˵����QWidget *parent: QWidget ����ָ�룬��������̳���QMainWindow��
* �������˵������
* ��������˵���������ڹ��캯�������촰�ں󣬽���ťstart����
*************************************************************************************************/
qt_static::qt_static(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.start->setEnabled(false);
}

/************************************************************************************************
* �������ƣ�~qt_static()
* �������˵������
* �������˵������
* ��������˵�������������������������ͷ�ʱ���ã��˳���δ���б�д
*************************************************************************************************/
qt_static::~qt_static()
{

}

/************************************************************************************************
* �������ƣ�paintEvent()
* �������˵����QPaintEvent *e: QPaintEvent ����ָ��
* �������˵������
* ��������˵����QT�����¼�����Mat����������ʾ��label��
*************************************************************************************************/
void qt_static::paintEvent(QPaintEvent * e)
{
	QImage image = QImage((uchar*)(output_image.data), output_image.cols, output_image.rows, QImage::Format_RGB888);
	ui.label->resize(image.size());
	ui.label->setPixmap(QPixmap::fromImage(image));
}

/************************************************************************************************
* �������ƣ�read_file_clicked()
* �������˵������
* �������˵������
* ��������˵����Read File ��ť�ۺ�����������Read File ��ťʱִ�У���ȡ��������ļ���
*************************************************************************************************/
void qt_static::read_file_clicked()
{
	filename = QFileDialog::getOpenFileName(this, tr("Choose Image File:"), "", tr("videos (*.avi )"));
	ui.lineEdit->setText(filename);
	ui.start->setEnabled(true);
}

/************************************************************************************************
* �������ƣ�start_clicked()
* �������˵������
* �������˵������
* ��������˵����Start ��ť�ۺ�����������Start ��ťʱִ�У����ð�ť״̬���ж��ļ��Ƿ��
  ��Ҫ�󣬲�������ʱ��
*************************************************************************************************/
void qt_static::start_clicked()
{
	if (!start_flag)
	{
		ui.lineEdit->setEnabled(false);
		ui.read_file->setEnabled(false);
		ui.start->setEnabled(false);
		connect(&t, &QTimer::timeout, this, &qt_static::updateImage);//����ʱ������»��溯����
		videoCap.open(filename.toStdString());//���ļ�������Ƶ�ļ�

		//�ж���Ƶ�Ƿ�ɹ���ȡ�������С�Ƿ�Ϊ640x480									  
		if (videoCap.isOpened() && videoCap.get(CAP_PROP_FRAME_HEIGHT) == 480 && videoCap.get(CAP_PROP_FRAME_WIDTH) == 640)
		{
			frames = videoCap.get(CAP_PROP_FRAME_COUNT);
			input_image = Mat::zeros(480, 640, CV_8UC3);
			pre_input_image = Mat::zeros(480, 640, CV_8UC1);
			t.start(10);//10ms��ʱ����ʼ
			start_flag = true;
		}
		else//���ļ�������Ҫ�󣬵�����ʾ��
		{
			QMessageBox::information(NULL, "Notice", "Can't open this file.", QMessageBox::Yes);
			ui.start->setEnabled(false);
			ui.read_file->setEnabled(true);
			ui.lineEdit->setEnabled(true);
		}
	}
}

/************************************************************************************************
* �������ƣ�exit_clicked()
* �������˵������
* �������˵������
* ��������˵����Exit ��ť�ۺ�����������Exit ��ťʱִ�У��˳�����
*************************************************************************************************/
void qt_static::exit_clicked()
{
	QApplication* app;
	app->exit(0);
}

/************************************************************************************************
* �������ƣ�updateImage()
* �������˵������
* �������˵������
* ��������˵����������º�������ȡ��Ƶ֡���У�ͨ��SURF���������ƥ�䣬����ǰ��֡��
����󣬲���ǰ֡����任�Խ����˶�������֮���ǰ��֡���в�����㣬��
��ʵ��ͼ��仯�����򣬶�ֵ������̬ѧ��������ROI������ѡȡ���������
������������϶�λ�����Ŀ�꣬���þ��ν����Χ
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
		if (cal_count == 5)//ÿ5֡����һ�μ���
		{
			detector->detect(input_image_gray, keypoints, Mat());//ʹ��SURF��⵱ǰ֡�����㣬������keypoints��
			extractor->compute(input_image_gray, keypoints, descriptors);//����ÿһ�������㣬�õ���������descriptors
			if (pre_descriptors.data && descriptors.data)
			{
				matcher->match(pre_descriptors, descriptors, match_points);//ʹ��FlannBasedMatcher����������ƥ�䣬ƥ����������Դ�����match_points��
				sort(match_points.begin(), match_points.end());//��match_points�е�������Խ���������ֵԽС����׼ȷ��Խ��

				float k = 0.4;
				if (match_points.size() < 10)//��ƥ���������С��10������ȡǰ80%��Ϊ�Ϻõ�ƥ����
					k = 0.8;
				vector<Point2f> pre_image_points, image_points;
				for (int i = 0;i < k*match_points.size();i++)//ȡmatch_points�е�ǰ40%��80%����������Ϊ�Ϻõ�ƥ����
				{
					pre_image_points.push_back(pre_keypoints[match_points[i].queryIdx].pt);
					image_points.push_back(keypoints[match_points[i].trainIdx].pt);
				}

				Mat warp_mat;
				if (image_points.size() > 3)
				{
					warp_mat = estimateRigidTransform(pre_image_points, image_points, 1);//�ɾ���ɸѡ�ĽϺõ�ƥ���Ի��ͼ��������
					if (warp_mat.rows == 2 && warp_mat.cols == 3)//�жϷ�������Ƿ�Ϸ�
					{
						warpAffine(pre_input_image, output_warp_image, warp_mat, Size(640, 480));//��ǰһ֡����������һ���õ��ķ���任���ɵõ��˶�������ľ���
						threshold(input_image_gray - output_warp_image, temp_out, 6, 255, THRESH_BINARY);//��ǰ֡���˶��������ǰһ֡���в�֣�����ֵ�����ɵõ�ʵ�ʷ����˶�������Ĵ�����Ϣ
						medianBlur(temp_out, temp_out, 3);//��ֵ�˲�
						Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));//��ʴ������ȥ��΢С�ġ���ɢ�������㣬���ٸ���
						morphologyEx(temp_out, temp_out, MORPH_ERODE, element);
						element = getStructuringElement(MORPH_RECT, Size(5, 5));//���Ͳ�������������ʴ���Դ��ڵġ�����ΪĿ������������������ڼ��
						morphologyEx(temp_out, temp_out, MORPH_DILATE, element);
						Mat ROI = temp_out(Rect(150, 200, 400, 180));//������֪ʶ��֪��Ŀ����´��ڻ�����в�ƫ�²��֣�����ROI���ɼ��ټ������߼��Ч��
						findContours(ROI, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));//��ROI��Ѱ������
					}
				}
			}

			pre_input_image = input_image_gray.clone();//����ǰ֡����Ϊǰһ֡���Խ�����һ�μ���
			pre_descriptors = descriptors;//���浱ǰ�����Ӿ���
			pre_keypoints.clear();//���ǰ�ؼ���vector
			pre_keypoints = keypoints;//���浱ǰ�ؼ���vector

			//fps
			if (cal_count == 5)//ÿ5֡�������һ����ʱ�䣬������֡��
			{
				dt = (t1 - t2) / getTickFrequency();
				fps = 5.0 / dt;
				sprintf(fps_string, "%d", (uint)fps);
				ui.label_3->setText(fps_string);
				t2 = t1;
			}
			cal_count = 0;//����֡������
		}

		output_image = input_image;

		for (int i = 0; i < contours.size(); i++)//���ӵ���������������㣬��Ϊ�Ǵ����Ŀ��
			biggest_contour = contours[i].size() > biggest_contour.size() ? contours[i] : biggest_contour;
		for (int i = 0;i < biggest_contour.size();i++)//ƽ���������꣬������������ROI��Ѱ�ҵõ��ģ���ת������õ�ԭͼ���е���������
		{
			biggest_contour[i].x += 145;
			biggest_contour[i].y += 200;
		}

		if (biggest_contour.size())//�ж������Ƿ����
			rectangle(output_image, boundingRect(biggest_contour), cvScalar(220, 0, 150), 2);//��ԭͼ���л�����ΧĿ�������ľ���

		if (frame_count == frames)//����Ƶ���ж��꣬���ð�ť�����ü��������ƺ�������ȴ�����һ����Ƶ���м��
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
