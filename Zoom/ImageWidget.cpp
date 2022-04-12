#include "ImageWidget.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QApplication>
#include <QDebug>

#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif // _MSC_VER > 1600

static const int multiples[] = { 20,30,40,50,60,70,80,90,100,110,120,130,140,150,175,200,225,250,275,300,\
								350,400,450,500,600,700,800 ,900,1000,1200,1400,1600,2000,2400,2800,3200 };
static int multipleCount = sizeof(multiples) / sizeof(multiples[0]);

ImageWidget::ImageWidget(QWidget *parent)
	: QLabel(parent)
{
	//m_pix.load("res/1.png");
	m_pix.load("C:/Users/Administrator/Pictures/Screenshots/1.png");
	m_pixWidth = m_pix.width();
	m_pixHeight = m_pix.height();
	
	m_bMove = false;
	m_scaleValue = 0;
	m_scaleDafault = 0;
	m_multipleIndex = 8;
	
	m_pessedPos = QPointF(0, 0);
	m_mousePos = QPointF(0, 0);
	m_newOrigin = QPointF(0, 0);
	m_oldOrigin = m_newOrigin;
	
	initUi();
}

ImageWidget::~ImageWidget()
{
}

void ImageWidget::initUi()
{
	// 设置显示框最小宽高
	setMinimumWidth(946);
	setMinimumHeight(690);
	setScaleDafault(width(), height());

	// 显示缩放比例标签
	m_pLabel = new QLabel(this);
	m_pLabel->setFixedSize(80, 40);
	m_pLabel->setAlignment(Qt::AlignCenter);
	m_pLabel->hide();
	m_pLabel->setStyleSheet("font: 18px \"SimHei\"; background-color:rgb(0,0,0); border-radius:20px; color:rgb(255,255,255);");

	// 给显示缩放比例添加动画
	QGraphicsOpacityEffect* m_pGraphicsOpacityEffect = new QGraphicsOpacityEffect(m_pLabel);
	m_pGraphicsOpacityEffect->setOpacity(1);
	m_pLabel->setGraphicsEffect(m_pGraphicsOpacityEffect);

	m_pAnimation = new QPropertyAnimation(m_pGraphicsOpacityEffect, "opacity", this);
	m_pAnimation->setEasingCurve(QEasingCurve::Linear);
	m_pAnimation->setStartValue(1);
	m_pAnimation->setEndValue(0);
	m_pAnimation->setDuration(1500);
}

void ImageWidget::setScaleDafault(const int width, const int height)
{
	// 获取基准倍数
	m_scaleDafault = width * 1.0 / m_pixWidth;
	if (m_scaleDafault * m_pixHeight - height > 0.001)
	{
		m_scaleDafault = height * 1.0 / m_pixHeight;
	}
	m_scaleValue = m_scaleDafault;
	m_newOrigin = QPointF((width - m_pixWidth * m_scaleValue) / 2.0, (height - m_pixHeight * m_scaleValue) / 2.0);
	m_oldOrigin = m_newOrigin;
}

void ImageWidget::zoomIn()
{
	m_multipleIndex++;
	m_scaleValue = m_scaleDafault * (multiples[m_multipleIndex] / 100.0);
	m_newOrigin = QPointF((width() - m_pixWidth * m_scaleValue) / 2.0, (height() - m_pixHeight * m_scaleValue) / 2.0);
	m_oldOrigin = m_newOrigin;
}

void ImageWidget::zoomOut()
{
	m_multipleIndex--;
	m_scaleValue = m_scaleDafault * (multiples[m_multipleIndex] / 100.0);
	m_newOrigin = QPointF((width() - m_pixWidth * m_scaleValue) / 2.0, (height() - m_pixHeight * m_scaleValue) / 2.0);
	m_oldOrigin = m_newOrigin;
}

void ImageWidget::zoomInAtPos()
{
	//m_scaleValue *= 1.1;// 每次放大10%
	double x = (m_mousePos.x() - m_newOrigin.x()) / m_scaleValue;
	double y = (m_mousePos.y() - m_newOrigin.y()) / m_scaleValue;
	m_multipleIndex++;
	m_scaleValue = m_scaleDafault * (multiples[m_multipleIndex] / 100.0);
	// 求鼠标点和原点缩放后的偏移量
	x = x * m_scaleValue;
	y = y * m_scaleValue;
	m_newOrigin.setX(m_mousePos.x() - x);
	m_newOrigin.setY(m_mousePos.y() - y);
	m_oldOrigin = m_newOrigin;
}

void ImageWidget::zoomOutAtPos()
{
	//m_scaleValue *= 1 / 1.1;// 每次缩小10%
	double x = (m_mousePos.x() - m_newOrigin.x()) / m_scaleValue;
	double y = (m_mousePos.y() - m_newOrigin.y()) / m_scaleValue;
	m_multipleIndex--;
	m_scaleValue = m_scaleDafault * (multiples[m_multipleIndex] / 100.0);
	x = x * m_scaleValue;
	y = y * m_scaleValue;
	m_newOrigin.setX(m_mousePos.x() - x);
	m_newOrigin.setY(m_mousePos.y() - y);
	m_oldOrigin = m_newOrigin;
}

void ImageWidget::adjustImagePos()
{
	// 图片宽大于等于显示框的宽，且图片高大于等于显示框的高
	if ((m_pixWidth * m_scaleValue > width()) && (m_pixHeight * m_scaleValue > height()))
	{
		if (m_newOrigin.x() >= 0)
		{
			m_newOrigin.setX(0);
			m_oldOrigin.setX(m_newOrigin.x());
		}
		if ((m_newOrigin.x() + m_pixWidth * m_scaleValue) < width())
		{
			m_newOrigin.setX(width() - m_pixWidth * m_scaleValue);
			m_oldOrigin.setX(m_newOrigin.x());
		}
		if (m_newOrigin.y() >= 0)
		{
			m_newOrigin.setY(0);
			m_oldOrigin.setY(m_newOrigin.y());
		}
		if ((m_newOrigin.y() + m_pixHeight * m_scaleValue) < height())
		{
			m_newOrigin.setY(height() - m_pixHeight * m_scaleValue);
			m_oldOrigin.setY(m_newOrigin.y());
		}
	}
	// 图片宽大于显示框的宽
	else if (m_pixWidth * m_scaleValue > width())
	{
		// 图片原点x在正半轴
		if (m_newOrigin.x() >= 0)
		{
			m_newOrigin.setX(0);
			m_oldOrigin.setX(m_newOrigin.x());
		}
		// 图片原点x在负半轴
		if ((m_newOrigin.x() + m_pixWidth * m_scaleValue) < width())
		{
			m_newOrigin.setX(width() - m_pixWidth * m_scaleValue);
			m_oldOrigin.setX(m_newOrigin.x());
		}
		// y保持不变
		m_newOrigin.setY(m_oldOrigin.y());
	}
	// 图片高大于显示框的高
	else if (m_pixHeight * m_scaleValue > height())
	{
		// 图片原点y在正半轴
		if (m_newOrigin.y() >= 0)
		{
			m_newOrigin.setY(0);
			m_oldOrigin.setY(m_newOrigin.y());
		}
		// 图片原点y在负半轴
		if ((m_newOrigin.y() + m_pixHeight * m_scaleValue) < height())
		{
			m_newOrigin.setY(height() - m_pixHeight * m_scaleValue);
			m_oldOrigin.setY(m_newOrigin.y());
		}
		// x保持不变
		m_newOrigin.setX(m_oldOrigin.x());
	}
}

void ImageWidget::showMultiple()
{
	m_pLabel->move((width() - m_pLabel->width()) / 2, (height() - m_pLabel->height()) / 2);
	m_pLabel->setText(QString("%1%").arg(multiples[m_multipleIndex]));
	m_pLabel->show();
	m_pAnimation->stop();
	m_pAnimation->start(QAbstractAnimation::KeepWhenStopped);
}

void ImageWidget::mousePressEvent(QMouseEvent* event)
{
	// 右键按下且图片宽高有一边没有铺面控件允许拖动
	if ((event->button() == Qt::LeftButton) && ((m_pixWidth * m_scaleValue > width()) || (m_pixHeight * m_scaleValue > height())))
	{
		setCursor(Qt::SizeAllCursor);
		m_pessedPos = event->pos();// 鼠标左击时，获取当前鼠标在图片中的坐标，
		m_bMove = true;			   // 标记鼠标左键被按下
	}
	return QLabel::mousePressEvent(event);
}

void ImageWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bMove)
	{
		// 计算偏移量
		QPointF offsetPos = (event->pos() - m_pessedPos);
		m_newOrigin = m_oldOrigin + offsetPos;
		adjustImagePos();
		// 更新绘画事件
		update();
	}
	return QLabel::mouseMoveEvent(event);
}

void ImageWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_bMove)
	{
		m_bMove = false;		  // 清除鼠标左键按下标志
		setCursor(Qt::ArrowCursor);
		m_oldOrigin = m_newOrigin;// 记录新原点 
		
	}
	return QLabel::mouseReleaseEvent(event);
}

void ImageWidget::wheelEvent(QWheelEvent* event)
{
	if (event->delta() > 0) // 鼠标滚轮向前滚动(放大)
	{
		// 放大到最大
		if (m_scaleValue >= (m_scaleDafault * (multiples[multipleCount - 1] / 100.0)))
		{
			return;
		}
		// 图像缩小到和显示框大小一样之后进行图片中心缩放
		else if((m_scaleValue * m_pixWidth < width() || m_scaleValue * m_pixHeight < height()))
		{
			zoomIn();
		}
		// 使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的
		else 
		{
			m_mousePos = event->pos();
			zoomInAtPos();
			adjustImagePos();
		}
	}
	else 
	{
		// 缩小到最小
		if (m_scaleValue <= m_scaleDafault * (multiples[0] / 100.0))
		{
			return;
		}
		// 图像缩小到和显示框大小一样之后进行图片中心缩放
		else if (((m_scaleDafault * (multiples[m_multipleIndex - 1] / 100.0)) * m_pixWidth < width() || (m_scaleDafault * (multiples[m_multipleIndex - 1] / 100.0)) * m_pixHeight < height()))
		{
			//qDebug() << "----------------------------------------";
			//qDebug() << "m_scaleValue * m_pixWidth" << m_scaleValue * m_pixWidth;
			//qDebug() << "width()" << width();
			//qDebug() << "m_scaleValue * m_pixHeight " << m_scaleValue * m_pixHeight;
			//qDebug() << "height()" << height();
			//qDebug() << "cen" << multiples[m_multipleIndex];
			zoomOut();
		}
		// 使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的
		else
		{
			//qDebug() << "----------------------------------------";
			//qDebug() << "m_scaleValue * m_pixWidth" << m_scaleValue * m_pixWidth;
			//qDebug() << "width()" << width();
			//qDebug() << "m_scaleValue * m_pixHeight " << m_scaleValue * m_pixHeight;
			//qDebug() << "height()" << height();
			//qDebug() << "pos" << multiples[m_multipleIndex];
			m_mousePos = event->pos();
			zoomOutAtPos();
			adjustImagePos();
		}
	}
	// 显示缩放倍数
	showMultiple();
	update();
	return QLabel::wheelEvent(event);
}

void ImageWidget::paintEvent(QPaintEvent*)
{
	if (!m_pix.isNull())
	{
		QPainter painter(this);
		// 消除抗锯齿
		painter.setRenderHint(QPainter::Antialiasing);

		// 坐标变换矩阵
		QTransform matrix;		
		// 移动绘画坐标原点
		matrix.translate(m_newOrigin.x(), m_newOrigin.y());
		// 缩放坐标系
		matrix.scale(m_scaleValue, m_scaleValue);
		painter.setWorldTransform(matrix);
		painter.drawPixmap(0, 0, m_pix);
	}
}

void ImageWidget::resizeEvent(QResizeEvent* event)
{
	// 宽变化
	if (event->size().width() != event->oldSize().width())
	{
		// 拉伸时，显示控件右侧边缘超出图片右侧边缘时
		if (event->size().width() > (m_newOrigin.x() + m_pixWidth * m_scaleValue))
		{
			// 移动
			m_newOrigin.setX(m_newOrigin.x() + (event->size().width() - event->oldSize().width()));
			// 图片左侧边缘坐标超出显示控件左侧边缘(x大于0)
			if (m_newOrigin.x() > 0)
			{
				// 居中
				m_newOrigin.setX((event->size().width() - m_pixWidth * m_scaleValue) / 2.0);
			}	
		}
		//  缩小时
		if ((event->size().width() <= event->oldSize().width()))
		{
			// 图片原点在显示框里，且显示控件的宽大于图片宽
			if (m_newOrigin.x() > 0 && (event->size().width() > (m_pixWidth * m_scaleValue)))
			{
				// 居中
				m_newOrigin.setX((event->size().width() - m_pixWidth * m_scaleValue) / 2.0);
			}
			// 图片原点在显示框里，且显示控件的宽小于于图片宽
			else if(m_newOrigin.x() > 0 &&  (event->size().width() <= (m_pixWidth * m_scaleValue)))
			{
				m_newOrigin.setX(0);
				
			}
		}
	}
	// 高变化
	if (event->size().height() != event->oldSize().height())
	{

		if (event->size().height() > (m_newOrigin.y() + m_pixHeight * m_scaleValue))
		{
			m_newOrigin.setY(m_newOrigin.y() + (event->size().height() - event->oldSize().height()));
			if (m_newOrigin.y() > 0)
			{
				m_newOrigin.setY((event->size().height() - m_pixHeight * m_scaleValue) / 2.0);
			}
		}

		if ((event->size().height() <= event->oldSize().height()))
		{
			if (m_newOrigin.y() > 0 && (event->size().height() > (m_pixHeight * m_scaleValue)))
			{
				m_newOrigin.setY((event->size().height()  - m_pixHeight * m_scaleValue)/ 2.0);
			}
			else if (m_newOrigin.y() > 0 && (event->size().height() <= (m_pixHeight * m_scaleValue)))
			{
				m_newOrigin.setY(0);
			}
		}
	}
	m_oldOrigin = m_newOrigin;
	
	return QLabel::resizeEvent(event);
}

void ImageWidget::onZoom()
{
	QPushButton* btn = dynamic_cast<QPushButton*>(sender());
	if (nullptr == btn)
		return;
	QString str = btn->objectName();

	if ("ZoomIn" == str) // 鼠标滚轮向前滚动(放大)
	{
		// 放大到最大
		if (m_scaleValue >= (m_scaleDafault * (multiples[multipleCount - 1] / 100.0)))
		{
			return;
		}
		// 图像缩小到和显示框大小一样之后进行图片中心缩放
		else if ((m_scaleValue * m_pixWidth < width() || m_scaleValue * m_pixHeight < height()))
		{
			zoomIn();
		}
		// 使图片缩放的效果看起来像是以屏幕中心进行缩放的
		else
		{
			// 获取显示控件中心点
			m_mousePos = QPointF(width() / 2.0, height() / 2.0);
			zoomInAtPos();
			adjustImagePos();
		}
	}
	else
	{
		// 缩小到最小
		if (m_scaleValue <= m_scaleDafault * (multiples[0] / 100.0))
		{
			return;
		}
		// 图像缩小到和显示框大小一样之后进行图片中心缩放
		else if (((m_scaleDafault * (multiples[m_multipleIndex - 1] / 100.0)) * m_pixWidth < width() || (m_scaleDafault * (multiples[m_multipleIndex - 1] / 100.0)) * m_pixHeight < height()))
		{
			zoomOut();
		}
		// 使图片缩放的效果看起来像是以屏幕中心进行缩放的
		else
		{
			// 获取显示控件中心点
			m_mousePos = QPointF(width() / 2.0, height() / 2.0);
			zoomOutAtPos();
			adjustImagePos();
		}
	}
	// 显示缩放倍数
	showMultiple();
	update();
}

void ImageWidget::onZoomTo100()
{
	m_scaleDafault = width() * 1.0 / m_pixWidth;
	if (m_scaleDafault * m_pixHeight - height() > 0.001)
	{
		m_scaleDafault = height() * 1.0 / m_pixHeight;
	}
	m_scaleValue = m_scaleDafault;
	
	// 更新倍数数组下标
	m_multipleIndex = 8; // 1.0倍
	m_newOrigin = QPointF((width() - m_pixWidth * m_scaleValue) / 2.0, (height() - m_pixHeight * m_scaleValue) / 2.0);
	m_oldOrigin = m_newOrigin;

	// 显示缩放倍数
	showMultiple();
	update();
}