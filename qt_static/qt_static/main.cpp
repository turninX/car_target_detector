#include "qt_static.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qt_static w;
	w.show();
	return a.exec();
}
