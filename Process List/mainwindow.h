#pragma once

#include <QtWidgets/QMainWindow>
#include <qtimer.h>
#include <qpushbutton.h>
#include "ui_mainwindow.h"
#include "ProcessModel.h"

class mainwindow : public QMainWindow
{
    Q_OBJECT
public:
    mainwindow(QWidget *parent = Q_NULLPTR);
    ProcessModel ProcessFunc;
private slots:
    void TimerRound();
    void KillProcess();

private:
    Ui::mainwindowClass ui;
    QTimer* timer;
    //QPushButton
};
