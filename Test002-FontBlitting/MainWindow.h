#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
private:
    Ui_MainWindow *m_Ui = nullptr;

public:
    MainWindow();
    ~MainWindow();
};

#endif // MAINWINDOW_H
