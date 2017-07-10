#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "qt_skell.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class QToolButton;
class QComboBox;
class QCheckBox;
class QSlider;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	static constexpr char smPoseLetters[] = "IX0123456789?!";

	QToolButton * m_tools[bp_poseLength][sizeof(smPoseLetters)-1];
	QComboBox * m_poses[POSES_PER_GAIT];

	int  getPoseLetter(int i) const;

public:

	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	Skell  m_skell;
	char * current_pose;
	GaitData * gait;

	QString m_file;

	void documentNew();
	void documentOpen();
	void documentSave();
	void documentSaveAs();
	void documentClose();

	void selectPose(int i);
	void selectGait(int i);

public slots:
	void gaitListChanged(int);

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
