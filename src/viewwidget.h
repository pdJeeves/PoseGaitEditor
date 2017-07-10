#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QWidget>
#include <QTimer>
#include <chrono>

class MainWindow;

class ViewWidget : public QWidget
{
	Q_OBJECT
	QTimer timer;
	std::chrono::time_point<std::chrono::system_clock> m_last;

public:
	explicit ViewWidget(QWidget *parent = nullptr);

	float getHeadRotation(int part, char c);
	int getBodyDirection();

	void doRepaint() { repaint(); }

	MainWindow * window;

	void paintEvent				(QPaintEvent * event)	Q_DECL_OVERRIDE;
};

#endif // VIEWWIDGET_H
