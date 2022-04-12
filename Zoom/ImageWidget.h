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
    
    void zoomIn();          // 按图片中心点放大
    void zoomOut();         // 按图片中心点缩小
    void zoomInAtPos();     // 按点放大
    void zoomOutAtPos();    // 按点缩小
    void adjustImagePos();  // 调整图片显示位置
    void showMultiple();    // 显示缩放倍数
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
    double  m_scaleValue;    // 当前缩放值
    double  m_scaleDafault;  // 基准缩放值
    QPixmap m_pix;           // 操作的图片
    int m_pixWidth;          // 操作的图片宽(没有缩放的)
    int m_pixHeight;         // 操作的图片高(没有缩放的)
    uint m_multipleIndex;
    QPointF m_pessedPos;     // 鼠标按下的位置
    QPointF m_mousePos;      // 鼠标位置
    QPointF m_oldOrigin;     // 图片当前绘画原点
    QPointF m_newOrigin;     // 图片变化后绘画原点
    bool   m_bMove;          // 图片是否可以拖动标志位

    // 显示缩放倍数动画 
    QPropertyAnimation* m_pAnimation;
    QLabel* m_pLabel;
};
