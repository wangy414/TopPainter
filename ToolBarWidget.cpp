#include <QDebug>
#include "ToolBarWidget.h"
#include <QPainter>
#include<QDesktopWidget>
#include<QApplication>
#include "XGlobalHook.h"
#include <QDateTime>
#include <QMessageBox>
#include <QList>
#include "ICmd.h"

#ifdef Q_OS_WIN
 #include <Windows.h>
#endif

ToolBarWidget::ToolBarWidget(QWidget *parent) :
    QWidget(parent)
{
    //setWindowFlags(Qt::FramelessWindowHint);
    //窗口设置透明
    //setAttribute(Qt::WA_TranslucentBackground,true);
    //得到客户区矩形
    parentPtr =parent;
    m_isAllowDrawing = 1;
}

ToolBarWidget::~ToolBarWidget()
{

}
void ToolBarWidget::setLocalGeometry(int x, int y, int w, int h)
{
    QRect rt(x,y,w,h);
    setGeometry(rt);

    QRect clientRect = rect();
    //resize(clientRect.width(),100);

    int toolYStart = 10;
    int btnCnt = 5;
    int btnSize = 72;
    int toolXStart = 10;

    pixLock = new QPixmap();
    pixLock->load("./img/lock.png");
    pixUnlock = new QPixmap();
    pixUnlock->load("./img/unlock.png");

    lockBtn=new CImageButton(this);
    lockBtn->SetPixmap(pixLock,pixLock,pixLock);
    //redBtn->setText(QString::fromLocal8Bit("天亮了"));
    //redBtn->setStyleSheet("color: rgb(255, 255, 255);");
    lockBtn->setGeometry(toolXStart,toolYStart,pixLock->width(),pixLock->height());
    connect( lockBtn, SIGNAL(clicked()), this, SLOT(onSlotLockMouseEvent()) );
    exitBtn=new CImageButton(this);
    pixExit = new QPixmap();
    pixExit->load("./img/exit.png");
    exitBtn->SetPixmap(pixExit,pixExit,pixExit);
    exitBtn->setGeometry(toolXStart+btnSize,toolYStart,pixExit->width(),pixExit->height());
    connect( exitBtn, SIGNAL(clicked()), qApp, SLOT(quit()) );

    saveBtn=new CImageButton(this);
    pixSave = new QPixmap();
    pixSave->load("./img/save.png");
    saveBtn->SetPixmap(pixSave,pixSave,pixSave);
    saveBtn->setGeometry(toolXStart+btnSize*2,toolYStart,pixSave->width(),pixSave->height());
    connect( saveBtn, SIGNAL(clicked()), this, SLOT(onSlotSaveMouseEvent()) );


    penBtn=new CImageButton(this);
    pixPen = new QPixmap();
    pixPen->load("./img/pen.png");
    penBtn->SetPixmap(pixPen,pixPen,pixPen);
    penBtn->setGeometry(toolXStart+btnSize*3,toolYStart,pixPen->width(),pixPen->height());
    connect( penBtn, SIGNAL(clicked()), this, SLOT(onSlotPenBtn()) );

    clearBtn=new CImageButton(this);
    pixClear = new QPixmap();
    pixClear->load("./img/clear.png");
    clearBtn->SetPixmap(pixClear,pixClear,pixClear);
    clearBtn->setGeometry(toolXStart+btnSize*4,toolYStart,pixClear->width(),pixClear->height());
    connect( clearBtn, SIGNAL(clicked()), this, SLOT(onSlotClearBtn()) );

    clearAllBtn=new CImageButton(this);
    pixAllClear = new QPixmap();
    pixAllClear->load("./img/clearall.png");
    clearAllBtn->SetPixmap(pixAllClear,pixAllClear,pixAllClear);
    clearAllBtn->setGeometry(toolXStart+btnSize*5,toolYStart,pixAllClear->width(),pixAllClear->height());
    connect( clearAllBtn, SIGNAL(clicked()), this, SLOT(onSlotClearAllBtn()) );
}
void ToolBarWidget::mousePressEvent(QMouseEvent *event)
{
    if( (event->button() == Qt::LeftButton) ){
        mouse_press = true;
        mousePoint = event->globalPos() - this->pos();
//        event->accept();
    }
    else if(event->button() == Qt::RightButton){
        //如果是右键
        this->close();

    }
}

void ToolBarWidget::mouseMoveEvent(QMouseEvent *event)
{ 
    qDebug("ToolBarWidget mouseMoveEvent %d \n",mouse_press);
    //    if(event->buttons() == Qt::LeftButton){  //如果这里写这行代码，拖动会有点问题
        if(mouse_press){
            move(event->globalPos() - mousePoint);
    //        event->accept();
        }
}
void ToolBarWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_press = false;
}

void ToolBarWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
#ifdef WIN32
    painter.fillRect(this->rect(), QColor(255, 255, 255, 1));
#else
    //mac电脑上需要将透明度设置为15，小于15鼠标控制画不上，15是临界值，是根据测试得来的，没有具体研究
    painter.fillRect(this->rect(), QColor(255, 255, 255, 15));
#endif
}
void ToolBarWidget::onSlotLockMouseEvent()
{
    if(m_isAllowDrawing == 1){
        XGlobalHook::instance()->installMouseHook();
        //XGlobalHook::instance()->eventRevObj = this;
        XGlobalHook::instance()->eventRevLst.append(exitBtn);
        XGlobalHook::instance()->eventRevLst.append(lockBtn);
        m_isAllowDrawing =  0;
        lockBtn->SetPixmap(pixUnlock,pixUnlock,pixUnlock);
        emit sendICmd(CMD_UNLOCK);
   }else if(m_isAllowDrawing == 0){
        XGlobalHook::instance()->uninstallMouseHook();
        XGlobalHook::instance()->eventRevLst.clear();
        m_isAllowDrawing = 1;
        lockBtn->SetPixmap(pixLock,pixLock,pixLock);
        emit sendICmd(CMD_LOCK);
        update();
    }
}
void  ToolBarWidget::onSlotSaveMouseEvent()
{
    emit sendICmd(CMD_SAVE);
}
void  ToolBarWidget::onSlotPenBtn()
{   
    emit sendICmd(CMD_PEN);
}
void  ToolBarWidget::onSlotClearBtn()
{
    emit sendICmd(CMD_CLEAR);
}

void  ToolBarWidget::onSlotClearAllBtn()
{
    emit sendICmd(CMD_CLEAR_ALL);
}
