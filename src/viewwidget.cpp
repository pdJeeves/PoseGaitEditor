#include "viewwidget.h"
#include "mainwindow.h"
#include <QPainter>
#include <QPaintEvent>
#include <QCursor>
#include <cmath>

extern "C"
{
float getHeadRotation(void * adapter, int part, char c)
{
	return ((ViewWidget*)adapter)->getHeadRotation(part,c);
}

int getBodyDirection(void * adapter)
{
	return ((ViewWidget*)adapter)->getBodyDirection();
}

}

ViewWidget::ViewWidget(QWidget *parent) :
	QWidget(parent),
	timer(this),
	m_last(std::chrono::system_clock::now()),
	window(0L)
{
	timer.start(16);
	connect(&timer, &QTimer::timeout, this, &ViewWidget::doRepaint);
}

float ViewWidget::getHeadRotation(int part, char c)
{
	return 2;
}

int ViewWidget::getBodyDirection()
{
	QPoint p = mapFromGlobal(QCursor::pos());
	QPointF mouse = QPointF(p.x()/(double)width() - .5, p.y()/(double)height() - .5)*2;
/*
	if(std::fabs(mouse.x()) < .5)
	{
		if(mouse.y() < -.25)
			return 0;
		else if(mouse.y() > .25)
			return 1;
	}*/

	if(mouse.x() < -.25)
			return 2;
	else if(mouse.x() > .25)
			return 3;
	return -1;
}

void ViewWidget::paintEvent	(QPaintEvent * event)
{
	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = now-m_last;
	m_last = now;

	QPainter painter;
	painter.begin(this);

	painter.fillRect(event->rect(), QBrush(Qt::black));
	painter.translate(event->rect().width()/2, event->rect().height()/2);

	if(window)
	{
		window->m_skell.update(elapsed_seconds.count());
		window->m_skell.render(painter);
	}

	painter.end();
}
