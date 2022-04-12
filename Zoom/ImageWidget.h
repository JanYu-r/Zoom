#pragma once

#include <QLabel>
#include <QImage>
#include <QPropertyAnimation>

class QMouseEvent;
class QWheelEvent;
class QPaintEvent;
class QResizeEvent;

class ImageWidget : public QLabel
{
	Q_OBJECT

public:
	ImageWidget(QWidget *parent = nullptr);
	~ImageWidget();
private:
    void initUi();
    void setScaleDafault(const int w, const int h);
    
    void zoomIn();          // ��ͼƬ���ĵ�Ŵ�
    void zoomOut();         // ��ͼƬ���ĵ���С
    void zoomInAtPos();     // ����Ŵ�
    void zoomOutAtPos();    // ������С
    void adjustImagePos();  // ����ͼƬ��ʾλ��
    void showMultiple();    // ��ʾ���ű���
protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
signals:
    void zoomMultiple(int multiple);
public slots:
    void onZoom();
    void onZoomTo100();
private:
    double  m_scaleValue;    // ��ǰ����ֵ
    double  m_scaleDafault;  // ��׼����ֵ
    QPixmap m_pix;           // ������ͼƬ
    int m_pixWidth;          // ������ͼƬ��(û�����ŵ�)
    int m_pixHeight;         // ������ͼƬ��(û�����ŵ�)
    uint m_multipleIndex;
    QPointF m_pessedPos;     // ��갴�µ�λ��
    QPointF m_mousePos;      // ���λ��
    QPointF m_oldOrigin;     // ͼƬ��ǰ�滭ԭ��
    QPointF m_newOrigin;     // ͼƬ�仯��滭ԭ��
    bool   m_bMove;          // ͼƬ�Ƿ�����϶���־λ

    // ��ʾ���ű������� 
    QPropertyAnimation* m_pAnimation;
    QLabel* m_pLabel;
};
