
#ifndef CIMAGEBUTTON_H
#define CIMAGEBUTTON_H
 
#include <QPushButton>
 
class CImageButton : public QPushButton
{
	Q_OBJECT
 
public:
	CImageButton(QWidget *parent);
	~CImageButton();
	void SetPixmap(QPixmap* pNormal,QPixmap* pHover,QPixmap* pLighted);
	void SetLighted(bool value);
public:
	void paintEvent(QPaintEvent* pEvent);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void enterEvent(QEvent *e);
	void leaveEvent(QEvent *e);
protected:
	bool m_bLighted;
	bool m_bMouseIN;
	bool m_bLeftDown;
	QPixmap* m_pHover;
	QPixmap* m_pNormal;
	QPixmap* m_pLighted;
	
};
 
#endif // CIMAGEBUTTON_H