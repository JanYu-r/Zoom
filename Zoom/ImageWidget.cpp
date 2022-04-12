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
	// ������ʾ����С���
	setMinimumWidth(946);
	setMinimumHeight(690);
	setScaleDafault(width(), height());

	// ��ʾ���ű�����ǩ
	m_pLabel = new QLabel(this);
	m_pLabel->setFixedSize(80, 40);
	m_pLabel->setAlignment(Qt::AlignCenter);
	m_pLabel->hide();
	m_pLabel->setStyleSheet("font: 18px \"SimHei\"; background-color:rgb(0,0,0); border-radius:20px; color:rgb(255,255,255);");

	// ����ʾ���ű�����Ӷ���
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
	// ��ȡ��׼����
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
	//m_scaleValue *= 1.1;// ÿ�ηŴ�10%
	double x = (m_mousePos.x() - m_newOrigin.x()) / m_scaleValue;
	double y = (m_mousePos.y() - m_newOrigin.y()) / m_scaleValue;
	m_multipleIndex++;
	m_scaleValue = m_scaleDafault * (multiples[m_multipleIndex] / 100.0);
	// �������ԭ�����ź��ƫ����
	x = x * m_scaleValue;
	y = y * m_scaleValue;
	m_newOrigin.setX(m_mousePos.x() - x);
	m_newOrigin.setY(m_mousePos.y() - y);
	m_oldOrigin = m_newOrigin;
}

void ImageWidget::zoomOutAtPos()
{
	//m_scaleValue *= 1 / 1.1;// ÿ����С10%
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
	// ͼƬ����ڵ�����ʾ��Ŀ���ͼƬ�ߴ��ڵ�����ʾ��ĸ�
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
	// ͼƬ�������ʾ��Ŀ�
	else if (m_pixWidth * m_scaleValue > width())
	{
		// ͼƬԭ��x��������
		if (m_newOrigin.x() >= 0)
		{
			m_newOrigin.setX(0);
			m_oldOrigin.setX(m_newOrigin.x());
		}
		// ͼƬԭ��x�ڸ�����
		if ((m_newOrigin.x() + m_pixWidth * m_scaleValue) < width())
		{
			m_newOrigin.setX(width() - m_pixWidth * m_scaleValue);
			m_oldOrigin.setX(m_newOrigin.x());
		}
		// y���ֲ���
		m_newOrigin.setY(m_oldOrigin.y());
	}
	// ͼƬ�ߴ�����ʾ��ĸ�
	else if (m_pixHeight * m_scaleValue > height())
	{
		// ͼƬԭ��y��������
		if (m_newOrigin.y() >= 0)
		{
			m_newOrigin.setY(0);
			m_oldOrigin.setY(m_newOrigin.y());
		}
		// ͼƬԭ��y�ڸ�����
		if ((m_newOrigin.y() + m_pixHeight * m_scaleValue) < height())
		{
			m_newOrigin.setY(height() - m_pixHeight * m_scaleValue);
			m_oldOrigin.setY(m_newOrigin.y());
		}
		// x���ֲ���
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
	// �Ҽ�������ͼƬ�����һ��û������ؼ������϶�
	if ((event->button() == Qt::LeftButton) && ((m_pixWidth * m_scaleValue > width()) || (m_pixHeight * m_scaleValue > height())))
	{
		setCursor(Qt::SizeAllCursor);
		m_pessedPos = event->pos();// ������ʱ����ȡ��ǰ�����ͼƬ�е����꣬
		m_bMove = true;			   // ���������������
	}
	return QLabel::mousePressEvent(event);
}

void ImageWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bMove)
	{
		// ����ƫ����
		QPointF offsetPos = (event->pos() - m_pessedPos);
		m_newOrigin = m_oldOrigin + offsetPos;
		adjustImagePos();
		// ���»滭�¼�
		update();
	}
	return QLabel::mouseMoveEvent(event);
}

void ImageWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_bMove)
	{
		m_bMove = false;		  // ������������±�־
		setCursor(Qt::ArrowCursor);
		m_oldOrigin = m_newOrigin;// ��¼��ԭ�� 
		
	}
	return QLabel::mouseReleaseEvent(event);
}

void ImageWidget::wheelEvent(QWheelEvent* event)
{
	if (event->delta() > 0) // ��������ǰ����(�Ŵ�)
	{
		// �Ŵ����
		if (m_scaleValue >= (m_scaleDafault * (multiples[multipleCount - 1] / 100.0)))
		{
			return;
		}
		// ͼ����С������ʾ���Сһ��֮�����ͼƬ��������
		else if((m_scaleValue * m_pixWidth < width() || m_scaleValue * m_pixHeight < height()))
		{
			zoomIn();
		}
		// ʹͼƬ���ŵ�Ч��������������������ڵ�Ϊ���Ľ������ŵ�
		else 
		{
			m_mousePos = event->pos();
			zoomInAtPos();
			adjustImagePos();
		}
	}
	else 
	{
		// ��С����С
		if (m_scaleValue <= m_scaleDafault * (multiples[0] / 100.0))
		{
			return;
		}
		// ͼ����С������ʾ���Сһ��֮�����ͼƬ��������
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
		// ʹͼƬ���ŵ�Ч��������������������ڵ�Ϊ���Ľ������ŵ�
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
	// ��ʾ���ű���
	showMultiple();
	update();
	return QLabel::wheelEvent(event);
}

void ImageWidget::paintEvent(QPaintEvent*)
{
	if (!m_pix.isNull())
	{
		QPainter painter(this);
		// ���������
		painter.setRenderHint(QPainter::Antialiasing);

		// ����任����
		QTransform matrix;		
		// �ƶ��滭����ԭ��
		matrix.translate(m_newOrigin.x(), m_newOrigin.y());
		// ��������ϵ
		matrix.scale(m_scaleValue, m_scaleValue);
		painter.setWorldTransform(matrix);
		painter.drawPixmap(0, 0, m_pix);
	}
}

void ImageWidget::resizeEvent(QResizeEvent* event)
{
	// ��仯
	if (event->size().width() != event->oldSize().width())
	{
		// ����ʱ����ʾ�ؼ��Ҳ��Ե����ͼƬ�Ҳ��Եʱ
		if (event->size().width() > (m_newOrigin.x() + m_pixWidth * m_scaleValue))
		{
			// �ƶ�
			m_newOrigin.setX(m_newOrigin.x() + (event->size().width() - event->oldSize().width()));
			// ͼƬ����Ե���곬����ʾ�ؼ�����Ե(x����0)
			if (m_newOrigin.x() > 0)
			{
				// ����
				m_newOrigin.setX((event->size().width() - m_pixWidth * m_scaleValue) / 2.0);
			}	
		}
		//  ��Сʱ
		if ((event->size().width() <= event->oldSize().width()))
		{
			// ͼƬԭ������ʾ�������ʾ�ؼ��Ŀ����ͼƬ��
			if (m_newOrigin.x() > 0 && (event->size().width() > (m_pixWidth * m_scaleValue)))
			{
				// ����
				m_newOrigin.setX((event->size().width() - m_pixWidth * m_scaleValue) / 2.0);
			}
			// ͼƬԭ������ʾ�������ʾ�ؼ��Ŀ�С����ͼƬ��
			else if(m_newOrigin.x() > 0 &&  (event->size().width() <= (m_pixWidth * m_scaleValue)))
			{
				m_newOrigin.setX(0);
				
			}
		}
	}
	// �߱仯
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

	if ("ZoomIn" == str) // ��������ǰ����(�Ŵ�)
	{
		// �Ŵ����
		if (m_scaleValue >= (m_scaleDafault * (multiples[multipleCount - 1] / 100.0)))
		{
			return;
		}
		// ͼ����С������ʾ���Сһ��֮�����ͼƬ��������
		else if ((m_scaleValue * m_pixWidth < width() || m_scaleValue * m_pixHeight < height()))
		{
			zoomIn();
		}
		// ʹͼƬ���ŵ�Ч����������������Ļ���Ľ������ŵ�
		else
		{
			// ��ȡ��ʾ�ؼ����ĵ�
			m_mousePos = QPointF(width() / 2.0, height() / 2.0);
			zoomInAtPos();
			adjustImagePos();
		}
	}
	else
	{
		// ��С����С
		if (m_scaleValue <= m_scaleDafault * (multiples[0] / 100.0))
		{
			return;
		}
		// ͼ����С������ʾ���Сһ��֮�����ͼƬ��������
		else if (((m_scaleDafault * (multiples[m_multipleIndex - 1] / 100.0)) * m_pixWidth < width() || (m_scaleDafault * (multiples[m_multipleIndex - 1] / 100.0)) * m_pixHeight < height()))
		{
			zoomOut();
		}
		// ʹͼƬ���ŵ�Ч����������������Ļ���Ľ������ŵ�
		else
		{
			// ��ȡ��ʾ�ؼ����ĵ�
			m_mousePos = QPointF(width() / 2.0, height() / 2.0);
			zoomOutAtPos();
			adjustImagePos();
		}
	}
	// ��ʾ���ű���
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
	
	// ���±��������±�
	m_multipleIndex = 8; // 1.0��
	m_newOrigin = QPointF((width() - m_pixWidth * m_scaleValue) / 2.0, (height() - m_pixHeight * m_scaleValue) / 2.0);
	m_oldOrigin = m_newOrigin;

	// ��ʾ���ű���
	showMultiple();
	update();
}