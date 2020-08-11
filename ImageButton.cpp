#include "imagebutton.h"
#include <QtGui>
CImageButton::CImageButton(QWidget* pParent):QPushButton(pParent),
	m_bMouseIN(false),
	m_bLeftDown(false),
	m_bLighted(false)
{
}
CImageButton::~CImageButton()
{
 
}
void CImageButton::SetPixmap(QPixmap* pNormal,QPixmap* pHover,QPixmap* pLighted)
{
	m_pNormal=pNormal;
	m_pHover=pHover;
	m_pLighted=pLighted;
}
 
void CImageButton::SetLighted(bool value)
{
	if(value != m_bLighted)
	{
		m_bLighted=value;
		repaint();
	}
}
 
void CImageButton::paintEvent(QPaintEvent* pEvent)
{
	QPixmap* pPixmap;
	if(m_bLighted || m_bLeftDown)
		pPixmap=m_pLighted;
	else
	{
		if(m_bMouseIN)
			pPixmap=m_pHover;
		else
			pPixmap=m_pNormal;
	}
	QPainter painter;
	painter.begin(this);
	if(pPixmap)
		painter.drawPixmap(rect(),*pPixmap);
	if(text().count() > 0)
		painter.drawText(rect(),Qt::AlignCenter,text());
	painter.end();
}
 
void CImageButton::mousePressEvent(QMouseEvent *e)
{
	if(e->button()==Qt::LeftButton)
	{
		m_bLeftDown=true;
		repaint();
	}
	QPushButton::mousePressEvent(e);
}
 
void CImageButton::mouseReleaseEvent(QMouseEvent *e)
{
	if(e->button()==Qt::LeftButton)
	{
		m_bLeftDown=false;
		repaint();
	}
	QPushButton::mouseReleaseEvent(e);
}
 
void CImageButton::enterEvent(QEvent *e)
{
	m_bMouseIN=true;
	repaint();
}
 
void CImageButton::leaveEvent(QEvent *e)
{
	m_bMouseIN=false;
	repaint();
}
