#ifndef QT_STATIC_H
#define QT_STATIC_H

#include <QtWidgets/QMainWindow>
#include "ui_qt_static.h"
#include <opencv2\opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "QTimer.h"
#include "iostream"
#include "QFileDialog.h"
#include "QMessageBox.h"
#include <stdlib.h>

#include <QtCore\QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)

using namespace cv;

class qt_static : public QMainWindow
{
	Q_OBJECT

public:
	qt_static(QWidget *parent = 0);
	~qt_static();
	QTimer t;
	Mat input_image, pre_input_image;
	Mat output_image;
	Mat temp_image;
	Mat template_image;
	VideoCapture videoCap;
	void paintEvent(QPaintEvent *e);

private slots:
	void read_file_clicked();
	void start_clicked();
	void exit_clicked();

public slots:
	void updateImage();

private:
	Ui::qt_staticClass ui;
};

#endif // QT_STATIC_H
