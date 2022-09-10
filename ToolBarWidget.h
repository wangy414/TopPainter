#ifndef _TOOLBAR_WIDGET_H_
#define _TOOLBAR_WIDGET_H_
 
#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPen>
#include "ImageButton.h"
#include <QPixmap>
#include <QToolBar>

class ToolBarWidget : public QWidget
{

    Q_OBJECT
public:
    explicit ToolBarWidget(QWidget *parent = 0);
    ~ToolBarWidget();

   void setLocalGeometry(int x, int y, int w, int h);
public slots:
    void  onSlotLockMouseEvent();
    void  onSlotSaveMouseEvent();
    void  onSlotPenBtn();
    void  onSlotClearBtn();
    void  onSlotClearAllBtn();

signals:
    void sendICmd(int iCmd);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

public:
    QWidget *parentPtr;
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


    QPoint mousePoint;
    bool mouse_press;
    bool m_isAllowDrawing;
};
#endif // _TOP_PAINTER_WIDGET_H_
