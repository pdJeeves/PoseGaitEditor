#include "mainwindow.h"
#include "catalogue.h"
#include <QApplication>

#ifndef _WIN32
#include <unistd.h>
#endif

#include <iostream>


int main(int argc, char *argv[])
{
	QCoreApplication::addLibraryPath(".");

#ifdef _WIN32
#else
	auto home = getenv("HOME");

	if(0 == home)
	{
		throw std::runtime_error("environment variable $HOME not defined.");
	}

	char buffer[1024];
	sprintf(buffer, "%s/.config/%s", home, "Kreatures");
	if(-1 == chdir(buffer))
	{
		std::cerr << buffer << std::endl;
		throw std::runtime_error(strerror(errno));
	}
#endif

	initialize_catalogue("en");

	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
