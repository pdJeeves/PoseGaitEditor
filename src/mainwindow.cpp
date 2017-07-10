#include "body/body.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "catalogue.h"
#include <QFileDialog>
#include <QToolButton>
#include <QHBoxLayout>
#include <QString>
#include <QCheckBox>
#include <QComboBox>
#include <cctype>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
current_pose(0L),
gait(0L),
ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->widget->window = this;
	m_skell.m_adapter = ui->widget;

	QStringList poseList;

	int N = catalogue_get_length("Pose List");
	for(int i = 0; i < N; ++i)
	{
		poseList << catalogue_get_string("Pose List", i);
	}

	ui->poseList->addItems(poseList);

	N = catalogue_get_length("Gait List");
	for(int i = 0; i < N; ++i)
	{
		ui->gaitList->insertItem(i, catalogue_get_string("Gait List", i));
	}

	connect(ui->actionNew,     &QAction::triggered, this, &MainWindow::documentNew);
	connect(ui->actionOpen,    &QAction::triggered, this, &MainWindow::documentOpen);
	connect(ui->actionSave,    &QAction::triggered, this, &MainWindow::documentSave);
	connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::documentSaveAs);
	connect(ui->actionClose,   &QAction::triggered, this, &MainWindow::documentClose);

	connect(ui->poseList, &QListWidget::currentRowChanged, this, &MainWindow::selectPose);
	connect(ui->gaitList, &QListWidget::currentRowChanged, this, &MainWindow::selectGait);

	ui->poseTable->setVisible(false);

	ui->poseTable->setRowCount(bp_poseLength);
	ui->poseTable->setColumnCount(sizeof(smPoseLetters)-1);
	ui->poseTable->setVerticalHeaderLabels({
		"Direction",
		"Head",
		"Body",
		"Left Thy",
		"Left Calf",
		"Left Foot",
		"Right Thy",
		"Right Calf",
		"Right Foot",
		"Left Humerus",
		"Left Radius",
		"Right Humerus",
		"Right Radius",
		"Tail Root",
		"Tail Tip",
	});


	for(int i = 0; i < bp_poseLength; ++i)
	{
		QButtonGroup * group = new QButtonGroup(ui->poseTable);

		for(int j = 0; j < sizeof(smPoseLetters)-1; ++j)
		{
			char op = smPoseLetters[j];
			char buf[2] = {op, 0};
			QToolButton * tool = 0L;

			if((i == 0 && !('4' <= op && op <= '9'))
			|| i == 1
			|| (i >  1 && op != '?' && op != '!'))
			{
				tool = new QToolButton(ui->poseTable);
				tool->setCheckable(true);
				tool->setText(buf);

				tool->connect(tool, &QToolButton::clicked, this, [=](bool value) { if(value && current_pose) current_pose[i] = op; } );
				group->addButton(tool);

				ui->poseTable->setCellWidget(i, j, tool);
			}

			m_tools[i][j] = tool;
		}
	}

	ui->poseTable->resizeColumnsToContents();

	m_poses[0] = ui->comboBox;
	m_poses[1] = ui->comboBox_2;
	m_poses[2] = ui->comboBox_3;
	m_poses[3] = ui->comboBox_4;
	m_poses[4] = ui->comboBox_5;
	m_poses[5] = ui->comboBox_6;
	m_poses[6] = ui->comboBox_7;
	m_poses[7] = ui->comboBox_8;

	for(int i = 0; i < POSES_PER_GAIT; ++i)
	{
		m_poses[i]->addItems(poseList);
		m_poses[i]->connect(m_poses[i], SIGNAL(currentIndexChanged(int)), this, SLOT(gaitListChanged(int)));
	}

	connect(ui->beat, &QSlider::valueChanged, this, [=](int value){ if(gait) gait->beat = value; });
	connect(ui->invert, &QCheckBox::stateChanged, this, [=](int value){ if(gait) gait->invert = value; });
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::documentNew()
{
}

void MainWindow::documentSave()
{
	if(m_file.isNull())
	{
		documentSaveAs();
		return;
	}

	FILE * fp = fopen(m_file.toStdString().c_str(), "w");
	if(!fp)
	{
		QMessageBox::information(this, QGuiApplication::applicationDisplayName(), tr("unable to open file %1 for writing").arg(m_file));
		documentSaveAs();
		return;
	}

	fprintf(fp,
	"gene 'poses' : genus {\n"
	"\tmoniker = '0000-0000-0000-0000',\n"
	"\tmother  = '0000-0000-0000-0EVE',\n"
	"\tfather  = '0000-0000-0000-ADAM'\n"
	 "}\n\nchromosome : 'P' {\n");

	for(int i = 0; i < 255; ++i)
	{
		char * pose = skellGetPoseString(m_skell.m_skell, i);

		if(!pose)
			continue;

		fprintf(fp, "\tgene '%s' : pose { '%.3s_%.3s_%.3s_%.2s_%.2s_%.2s' }\n",
			catalogue_get_string("Pose List", i),
			pose,
			pose+3,
			pose+6,
			pose+9,
			pose+11,
			pose+13
		);
	}

	fprintf(fp, "}\n\nchromosome : 'G' {\n");

	for(int i = 0; i < 255; ++i)
	{
		GaitData * gait = skellGetGaitData(m_skell.m_skell, i);

		if(!gait)
			continue;

		fprintf(fp, "\tgene '%s' : gait {\n\t\tpose = {\n", catalogue_get_string("Gait List", i));
		for(int j = 0; j < POSES_PER_GAIT; ++j)
		{
			if(!gait->pose[j])
				break;

			if(j > 0)
				fprintf(fp, ",\n");

			fprintf(fp, "\t\t\t'%s'", catalogue_get_string("Pose List", gait->pose[j]));
		}
		fprintf(fp, "\n\t\t}");

		if(gait->beat != 128)
		{
			fprintf(fp, ",\n\t\tbeat = %i", gait->beat);
		}

		if(gait->invert)
		{
			fprintf(fp, ",\n\t\tinvert");
		}

		fprintf(fp, "\n\t}\n");
	}

	fprintf(fp, "}\n");

	fprintf(fp,
	"\nchromosome : 'A' {\n"
	"\tgene 'ears' : appearance { 'Pixie' }\n"
	"\tgene 'head' : appearance { 'Pixie' }\n"
	"\tgene 'hair' : appearance { 'Pixie' }\n"
	"\tgene 'body' : appearance { 'Pixie' }\n"
	"\tgene 'arms' : appearance { 'Pixie' }\n"
	"\tgene 'legs' : appearance { 'Pixie' }\n"
	"\tgene 'tail' : appearance { 'Pixie' }\n"
	"}\n");




	fclose(fp);
}


void MainWindow::documentSaveAs()
{
	FILE * fp = 0L;

	while(fp == 0L)
	{
		QString filename = QFileDialog::getSaveFileName(this,
		tr("Open Egg"), 0L, tr("Egg File (*.egg)"));

		if(filename.isNull())
			return;

		fp = fopen(filename.toStdString().c_str(), "w");
		if(fp)
		{
			fclose(fp);
			m_file = filename;
			documentSave();
			return;
		}

		QMessageBox::information(this, QGuiApplication::applicationDisplayName(), tr("unable to open file %1 for writing").arg(filename));
	}
}

void MainWindow::documentClose()
{

}

void MainWindow::documentOpen()
{
	QString filename = QFileDialog::getOpenFileName(this,
	tr("Open Egg"), 0L, tr("Egg File (*.egg)"));

	if(filename.isNull())
		return;

	m_skell.loadGenome(filename.toStdString());
	ui->widget->repaint();
}

int  MainWindow::getPoseLetter(int i) const
{
	const char op = toupper(current_pose[i]);
	for(int j = 0; j < sizeof(smPoseLetters)-1; ++j)
	{
		if(op == smPoseLetters[j])
		{
			return j;
		}
	}

	return 0;
}

void MainWindow::selectPose(int i)
{
	if(ui->poseTable->isVisible() == false)
	{
		ui->poseTable->setVisible(true);
	}

	if(!m_skell.m_skell)
		return;

	skellSetPose(m_skell.m_skell, i);
	current_pose = skellGetPoseString(m_skell.m_skell, i);

	if(!current_pose)
		return;

	for(int i = 0; i < bp_poseLength; ++i)
	{
		int j = getPoseLetter(i);
		const char op = toupper(current_pose[i]);
		m_tools[i][j]->setChecked(true);
	}
}

void MainWindow::selectGait(int i)
{
	if(ui->poseTable->isVisible())
	{
		ui->poseTable->setVisible(false);
	}

	if(!m_skell.m_skell)
		return;

	skellSetGait(m_skell.m_skell, i);
	GaitData * m_gait = skellGetCurrentGaitData(m_skell.m_skell);
	gait = 0L;

	if(!m_gait)
		return;

	ui->beat->setValue(m_gait->beat);
	ui->invert->setChecked(m_gait->invert);

	for(int i = 0; i < POSES_PER_GAIT; ++i)
	{
		m_poses[i]->setCurrentIndex(m_gait->pose[i]);
	}

	gait = m_gait;
}

void MainWindow::gaitListChanged(int)
{
	if(!gait) return;

	for(int i = 0; i < POSES_PER_GAIT; ++i)
	{
		gait->pose[i] = m_poses[i]->currentIndex();
	}
}
