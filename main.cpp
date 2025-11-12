/**
 * @file main.cpp
 * @brief Entry point for the ObjectDrawer application using Qt's event loop. 
 * @author Nikol Grigoryan
 */
#include "mainwindow.h"

#include <QApplication>

/**
 * @brief Creates the Qt application and launches the main window.
 * @param argc Argument count.
 * @param argv Argument values.
 * @return Qt event loop exit code.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
