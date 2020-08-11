#include<QDebug>
#include "XGlobalHook.h"
#include <QApplication>
#include <QWidget>
 /// 静态变量定义
 QMutex XGlobalHook::mutex;
 QSharedPointer<XGlobalHook> XGlobalHook::hook_instance;
 
 /// 全局变量定义
 HINSTANCE g_hInstance = Q_NULLPTR;
 HHOOK g_hMouseHook = Q_NULLPTR;  //鼠标钩子 句柄
 HHOOK g_hKeyHook = Q_NULLPTR;    //键盘钩子 句柄
 
 /// 全局函数定义
 // DllMain
 BOOL WINAPI DllMain(
     HINSTANCE hinstDLL,  // DLL模块的句柄
     DWORD fdwReason,     // 调用的情况
     LPVOID lpReserved    // reserved
     );
 
 // 鼠标钩子处理函数
 LRESULT CALLBACK MouseProc(int nCode,       // 钩子码
                            WPARAM wParam,   // 消息ID
                            LPARAM lParam    // 鼠标坐标
                            );
 
 // 键盘钩子处理函数
 LRESULT CALLBACK KeyboardProc(int nCode,            // 钩子码
                               WPARAM wParam,        // 虚拟键码
                               LPARAM lParam         // 键击信息
                               );
 
 
 XGlobalHook::~XGlobalHook()
 {
   uninstallMouseHook();
   uninstallKeyHook();
 }
 
 QSharedPointer<XGlobalHook>&XGlobalHook::instance()
 {
   if (hook_instance.isNull())
   {
     QMutexLocker mutexLocker(&mutex);
     if (hook_instance.isNull())
       hook_instance = QSharedPointer<XGlobalHook>(new XGlobalHook());
   }
   return hook_instance;
 }
 
 bool XGlobalHook::installMouseHook()
 {
   g_hMouseHook =SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseProc, g_hInstance, 0);
   //qDebug()<<"installMouseHook "<<Q_LIKELY(g_hMouseHook);
   return Q_LIKELY(g_hMouseHook);
 }
 
 bool XGlobalHook::uninstallMouseHook()
 {
   if (Q_UNLIKELY(!g_hMouseHook)) {
     return true;
   }
 
   bool success = false;
   success = UnhookWindowsHookEx(g_hMouseHook);
   if (success) {
     g_hMouseHook = Q_NULLPTR;
   }
    qDebug()<<"uninstallMouseHook";
   return success;
 }
 
 bool XGlobalHook::isMouseHookRunning() const
 {
   return Q_LIKELY(g_hMouseHook);
 }
 
 bool XGlobalHook::installKeyHook()
 {
   g_hKeyHook =SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, g_hInstance, 0);
   return Q_LIKELY(g_hKeyHook);
 }
 
 bool XGlobalHook::uninstallKeyHook()
 {
   if (Q_UNLIKELY(!g_hKeyHook)) {
     return true;
   }
 
   bool success = false;
   success = UnhookWindowsHookEx(g_hKeyHook);
   if (success) {
     g_hKeyHook = Q_NULLPTR;
   }
   return success;
 }
 
 bool XGlobalHook::isKeyHookRunning() const
 {
   return Q_LIKELY(g_hKeyHook);
 }
 
 /// DLL 钩子接口
 #if defined GLOBALHOOK_LIBRARY
 
 void XGlobalHook::onMouseEvent(PMOUSEHOOKSTRUCT pMouseHookStruct)
 {
   //qDebug()<<"onMouseEvent";
   emit mouseEvent(pMouseHookStruct);
 }
 
 void XGlobalHook::onKeyEvent(PKBDLLHOOKSTRUCT pKeyHookStruct)
 {
   emit keyEvent(pKeyHookStruct);
 }

int XGlobalHook::onMouseEvent(int keyCode,PMOUSEHOOKSTRUCT pMouseHookStruct)
{
    int ret = 0;
    QMouseEvent* eventPtr = NULL;
    QPoint pos(pMouseHookStruct->pt.x,pMouseHookStruct->pt.y);

    for(int i = 0; i < XGlobalHook::instance()->eventRevLst.count(); i++){
        QPoint localPos = ((QWidget*)(XGlobalHook::instance()->eventRevLst[i]))->mapFromGlobal(pos);
        QRect widgetRect = ((QWidget*)(XGlobalHook::instance()->eventRevLst[i]))->rect();

        qDebug("global %d  %d \n",pos.x(),pos.y());
        qDebug("local %d  %d \n",localPos.x(),localPos.y());
        qDebug("%d %d %d %d %d %d\n",widgetRect.left(),widgetRect.top(),widgetRect.x(),widgetRect.y(),widgetRect.width(),widgetRect.height());

        if(!widgetRect.contains(localPos)){
           continue;
        }
        Qt::MouseButton mouseBtn = Qt::NoButton;
        switch(keyCode){
            /*
            case WM_MOUSEWHEEL://滑轮
            {
                 break;
            }
            case WM_RBUTTONDOWN:
            {
                 mouseBtn = Qt::RightButton;
                 eventPtr = new QMouseEvent(QEvent::MouseButtonPress, localPos,  Qt::RightButton,Qt::RightButton, Qt::NoModifier);
                 break;
            }
            case WM_RBUTTONUP:
            {
                  mouseBtn = Qt::NoButton;
                 eventPtr = new QMouseEvent(QEvent::MouseButtonRelease, localPos,  Qt::RightButton,Qt::RightButton, Qt::NoModifier);
                 break;
            }
            */
            case WM_LBUTTONDOWN:
            {
                 mouseBtn = Qt::LeftButton;
                 eventPtr = new QMouseEvent(QEvent::MouseButtonPress, localPos, Qt::LeftButton,Qt::LeftButton, Qt::NoModifier);
                 break;
            }

            case WM_LBUTTONUP:
            {
                 mouseBtn = Qt::NoButton;
                 eventPtr = new QMouseEvent(QEvent::MouseButtonRelease, localPos,  Qt::LeftButton,Qt::LeftButton, Qt::NoModifier);
                 break;
            }
            /*case WM_MOUSEMOVE:
            {
                 eventPtr = new QMouseEvent(QEvent::MouseMove, localPos, mouseBtn,mouseBtn,Qt::NoModifier);
                 break;
            }*/
        }
        if(NULL != eventPtr){
            QApplication::sendEvent(XGlobalHook::instance()->eventRevLst[i], eventPtr);
            delete eventPtr;
            eventPtr = NULL;
            ret = 1;
            return ret;
        }
    }
    return ret;

}
 #endif
 
 LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
 {
 
   PMOUSEHOOKSTRUCT pmshs = reinterpret_cast<PMOUSEHOOKSTRUCT>(lParam);
   int ret = xHook->onMouseEvent(wParam,pmshs);
   if(ret){
       return true;
    }
#if 0
   QMouseEvent* eventPtr = NULL;
   QPoint pos(pmshs->pt.x,pmshs->pt.y);
   QPoint localPos = ((QWidget*)(XGlobalHook::instance()->eventRevObj))->mapFromGlobal(pos);

   static  Qt::MouseButton mouseBtn = Qt::NoButton;
   switch(wParam){
       case WM_MOUSEWHEEL://滑轮
       {
            break;
       }
       case WM_RBUTTONDOWN:
       {
            mouseBtn = Qt::RightButton;
            eventPtr = new QMouseEvent(QEvent::MouseButtonPress, localPos,  Qt::RightButton,Qt::RightButton, Qt::NoModifier);
            break;
       }
       case WM_LBUTTONDOWN:
       {
            mouseBtn = Qt::LeftButton;
            eventPtr = new QMouseEvent(QEvent::MouseButtonPress, localPos, Qt::LeftButton,Qt::LeftButton, Qt::NoModifier);
            break;
       }
       case WM_RBUTTONUP:
       {
             mouseBtn = Qt::NoButton;
            eventPtr = new QMouseEvent(QEvent::MouseButtonRelease, localPos,  Qt::RightButton,Qt::RightButton, Qt::NoModifier);
            break;
       }
       case WM_LBUTTONUP:
       {
            mouseBtn = Qt::NoButton;
            eventPtr = new QMouseEvent(QEvent::MouseButtonRelease, localPos,  Qt::LeftButton,Qt::LeftButton, Qt::NoModifier);
            break;
       }
       case WM_MOUSEMOVE:
       {
            eventPtr = new QMouseEvent(QEvent::MouseMove, localPos, mouseBtn,mouseBtn,Qt::NoModifier);
            break;
       }
   }
   xHook->onMouseEvent(eventPtr);
   delete eventPtr;
   //return true;
#endif
   return CallNextHookEx(g_hMouseHook,nCode,wParam,lParam);
 }
 
 LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
 {
   PKBDLLHOOKSTRUCT pkbhs = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam);
   xHook->onKeyEvent(pkbhs);
   return CallNextHookEx(g_hKeyHook, nCode, wParam, lParam);
 }
 
 BOOL WINAPI DllMain(
     HINSTANCE hinstDLL,  // DLL模块的句柄
     DWORD fdwReason,     // 调用的情况
     LPVOID lpReserved )  // reserved
 {
   Q_UNUSED(lpReserved);
   // 在不同的情况下都会调用DllMain函数，分别处理
   switch( fdwReason )
   {
     // 加载Dll
     case DLL_PROCESS_ATTACH:
     {
       g_hInstance = hinstDLL;
       break;
     }
       // 新建线程
     case DLL_THREAD_ATTACH:
     break;
       // 线程退出
     case DLL_THREAD_DETACH:
     break;
       // 释放Dll
     case DLL_PROCESS_DETACH:
       g_hInstance = Q_NULLPTR;
     break;
   }
   return TRUE;
 }
