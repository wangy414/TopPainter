#include <QDebug>
#include "TopPainterWidget.h"
#include <QPainter>
#include<QDesktopWidget>
#include<QApplication>
#include "XGlobalHook.h"
#include <QDateTime>
#include <QMessageBox>
#include <QList>

#ifdef Q_OS_LINUX
 XShapeCombineRectangles(QX11Info::display(), winId(), ShapeInput, 0,
       0, NULL, 0, ShapeSet, YXBanded);
#endif
#ifdef Q_OS_WIN
 #include <Windows.h>
#endif

TopPainterWidget::TopPainterWidget(QWidget *parent) :
    QWidget(parent),
    m_isPressed(false),
    m_isAllowDrawing(1)
{

    //ui->setupUi(this);
    //设置无边界窗口并置顶

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setMouseTracking(true);
    //窗口设置透明
    setAttribute(Qt::WA_TranslucentBackground,true);
   // setAttribute(Qt::WA_TransparentForMouseEvents);

    QDesktopWidget* desktopWidget = QApplication::desktop();
    //得到客户区矩形
    QRect clientRect = desktopWidget->availableGeometry();

    //得到应用程序矩形
    QRect applicationRect = desktopWidget->screenGeometry();
    resize(applicationRect.width(),applicationRect.height());
    
    m_isClear = false;
    
    m_graphicsView = new QGraphicsView(this);

    //设置鼠标穿透效果
    m_graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_graphicsView->setStyleSheet("background:transparent;");
    m_graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_graphicsView->setGeometry(0,0,width(),height());
    m_graphicsView->show();

    //设置绘图场景
    m_scene = new QGraphicsScene();
    m_graphicsView->setScene(m_scene);
    m_graphicsView->setSceneRect(0,0,width(),height());
    m_pen.setStyle(Qt::SolidLine);
    m_pen.setColor(Qt::red);
    m_pen.setWidth(3);
    move(0,0);

    int toolYStart = applicationRect.height()-200;
    int btnCnt = 5;
    int btnSize = 72;
    int toolXStart = (applicationRect.width() - btnCnt*btnSize)/2;

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

TopPainterWidget::~TopPainterWidget()
{

   // delete ui;

    delete m_graphicsView;

}

void TopPainterWidget::mousePressEvent(QMouseEvent *event)

{
    if(m_isAllowDrawing == 0){
        /*qDebug(" %d  %d \n",event->pos().x(),event->pos().y());
        QPoint pt = lockBtn->mapFromGlobal(event->pos());
        qDebug(" %d  %d \n",pt.x(),pt.y());
        qDebug("%d %d \n",lockBtn->geometry().width(),lockBtn->geometry().height());
        if(redBtn->geometry().contains(redBtn->mapFromParent( event->pos()))){
           QApplication::sendEvent(lockBtn, event);
        }
        if(exitBtn->geometry().contains(exitBtn->mapFromParent(event->pos()))){
           QApplication::sendEvent(exitBtn, event);
        }*/
        return;
     }

    m_isPressed = true;

    m_pStart = event->pos();

}

void TopPainterWidget::mouseMoveEvent(QMouseEvent *event)

{
    if(m_isAllowDrawing == 0)
        return;
    QPainter painter(m_graphicsView);


    if(m_isPressed){

        QPoint pt = event->pos();

//        清除绘制内容（橡皮擦功能）

//        QTransform transform;

//        QGraphicsItem *item = m_scene->itemAt(pt,transform);

//        if(item){

//            m_scene->removeItem(item);

//        }

        //绘制图像
        if(m_isClear){
            //获得指定矩形区域内的元素的指针列表
            QRectF curRt = QRectF(m_pStart.x() ,m_pStart.y(), pt.x()-m_pStart.x(),pt.y() - m_pStart.y());
            QList<QGraphicsItem *> itemList= m_scene->items(curRt, Qt::IntersectsItemShape);

            //删除元素
            for(int i=0; i<itemList.size(); i++){
                m_scene->removeItem(itemList[i]);  //从scene移除
                delete itemList[i];  //释放内存
            }
        }else{
            m_scene->addLine(m_pStart.x(),m_pStart.y(),pt.x(),pt.y(),m_pen);
        }
        m_pStart = pt;

    }

}

void TopPainterWidget::mouseReleaseEvent(QMouseEvent *event)

{
    if(m_isAllowDrawing == 0){
        /*if(redBtn->geometry().contains(redBtn->mapFromParent( event->pos()))){
           QApplication::sendEvent(redBtn, event);
        }
        if(exitBtn->geometry().contains(exitBtn->mapFromParent(event->pos()))){
           QApplication::sendEvent(exitBtn, event);
        }
        */
        return;
    }

    m_isPressed = false;

}

void TopPainterWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
#ifdef WIN32
    painter.fillRect(this->rect(), QColor(255, 255, 255, 1));
#else
    //mac电脑上需要将透明度设置为15，小于15鼠标控制画不上，15是临界值，是根据测试得来的，没有具体研究
    painter.fillRect(this->rect(), QColor(255, 255, 255, 15));
#endif
}
void TopPainterWidget::onSlotLockMouseEvent()
{
    if(m_isAllowDrawing == 1){
        //m_graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents,false);
    #ifdef Q_OS_WIN
        SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) |WS_EX_TRANSPARENT | WS_EX_LAYERED);
        //SetWindowLong((HWND)exitBtn->winId(), GWL_EXSTYLE, GetWindowLong((HWND)exitBtn->winId(), GWL_EXSTYLE)&(~WS_EX_TRANSPARENT));
    #endif
        XGlobalHook::instance()->installMouseHook();
        //XGlobalHook::instance()->eventRevObj = this;
        XGlobalHook::instance()->eventRevLst.append(exitBtn);
        XGlobalHook::instance()->eventRevLst.append(lockBtn);
        m_isAllowDrawing =  0;
        lockBtn->SetPixmap(pixUnlock,pixUnlock,pixUnlock);
   }else if(m_isAllowDrawing == 0){
        XGlobalHook::instance()->uninstallMouseHook();
        XGlobalHook::instance()->eventRevLst.clear();
        //m_graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents,true);
        LONG lWindowLong = ::GetWindowLong((HWND)winId(), GWL_EXSTYLE);
        lWindowLong = lWindowLong &(~WS_EX_TRANSPARENT) &(~WS_EX_LAYERED );
        // lWindowLong = lWindowLong & (~ WS_EX_TRANSPARENT);
        //SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) |WS_EX_TRANSPARENT | WS_EX_LAYERED);
        SetWindowLong((HWND)winId(), GWL_EXSTYLE, lWindowLong);
        m_isAllowDrawing = 1;
        lockBtn->SetPixmap(pixLock,pixLock,pixLock);
        //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        //setFocus(Qt::MouseFocusReason);
        update();
    }

}
void  TopPainterWidget::onSlotSaveMouseEvent()
{
    QImage img(this->size(), QImage::Format_ARGB32_Premultiplied);
    //实测这一步如果是scrollArea的话，会保存出一张scrollArea界面包含滚动条的图片，不是期望的全部图片
    img.fill(Qt::transparent);

    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    this->render(&painter);
    painter.end();
    auto current_path = QCoreApplication::applicationDirPath();//获取当前所在路径

    QString strName = QDateTime::currentDateTime().toString ("yyyy_MM_dd_hh_mm_ss");

    QString file_path_name = current_path + strName + ".png";
    if (img.save(file_path_name, "PNG"))
    {
        qDebug() << "保存成功";
        QMessageBox::information(this, "Save", "OK");
        return ;//保存成功返回保存文件的路径，可以用于打印
    }
    else
    {
        qDebug() << "保存失败 " << file_path_name;
        QMessageBox::information(this, "Save", "Fail");
        return ;
    }
}
void  TopPainterWidget::onSlotPenBtn()
{
    //m_pen.setStyle(Qt::SolidLine);
    //m_pen.setColor(Qt::red);
    //m_pen.setWidth(3);
    
    m_isClear = false;
}
void  TopPainterWidget::onSlotClearBtn()
{
    //m_pen.setStyle(Qt::SolidLine);
    //m_pen.setColor(QColor(0,0,0,0));
    //m_pen.setColor(Qt::transparent);
    //m_pen.setWidth(10);
    
    m_isClear = true;
}

void  TopPainterWidget::onSlotClearAllBtn()
{
    m_scene->clear();
}
