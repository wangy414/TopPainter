#ifndef _TOP_PAINTER_WIDGET_H_
#define _TOP_PAINTER_WIDGET_H_
 
#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPen>
#include "ImageButton.h"
#include <QPixmap>
#include <QToolBar>

class TopPainterWidget : public QWidget
{

    Q_OBJECT
public:
    explicit TopPainterWidget(QWidget *parent = 0);
    ~TopPainterWidget();

public slots:
    void  onSlotLockMouseEvent();
    void  onSlotSaveMouseEvent();
    void  onSlotPenBtn();
    void  onSlotClearBtn();
    void  onSlotClearAllBtn();
    
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

public:

    CImageButton *lockBtn;
    QPixmap      *pixLock;
    QPixmap      *pixUnlock;

    CImageButton *saveBtn;
    QPixmap      *pixSave;

    CImageButton *penBtn;
    QPixmap      *pixPen;
    
    CImageButton *clearBtn;
    QPixmap      *pixClear;
    
    CImageButton *clearAllBtn;
    QPixmap      *pixAllClear;

    QPixmap       *pixExit;
    CImageButton *exitBtn;

    QGraphicsView *m_graphicsView;
    QGraphicsScene *m_scene;
    QPoint m_pStart;
    QPen m_pen;
    bool m_isPressed;
    bool m_isAllowDrawing;
    bool m_isClear;
};
#endif // _TOP_PAINTER_WIDGET_H_
