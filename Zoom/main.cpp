#include "Zoom.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Zoom w;
	w.show();
	return a.exec();
}
