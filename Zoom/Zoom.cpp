#include "Zoom.h"
#include <QToolButton>

Zoom::Zoom(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.zoomInBtn->setObjectName("ZoomIn");
	ui.zoomOutBtn->setObjectName("ZoomOut");
	connect(ui.zoomInBtn, &QPushButton::clicked, ui.label, &ImageWidget::onZoom);
	connect(ui.zoomOutBtn, &QPushButton::clicked, ui.label, &ImageWidget::onZoom);
	connect(ui.fullBtn, &QPushButton::clicked, ui.label, &ImageWidget::onZoomTo100);
}
