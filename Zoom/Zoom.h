#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Zoom.h"

class Zoom : public QMainWindow
{
	Q_OBJECT

public:
	Zoom(QWidget *parent = Q_NULLPTR);

private:
	Ui::ZoomClass ui;
};
