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
#include "ToolBarWidget.h"

//#define FEA_WITH_BTN

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

    void  onSlotGetCmd(int iCmd);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);


public:
#ifdef FEA_WITH_BTN
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
#endif
    QGraphicsView *m_graphicsView;
    QGraphicsScene *m_scene;
    QPoint m_pStart;
    QPen m_pen;
    bool m_isPressed;
    bool m_isAllowDrawing;
    bool m_isClear;

    ToolBarWidget *mToolBarPtr;
};
#endif // _TOP_PAINTER_WIDGET_H_
