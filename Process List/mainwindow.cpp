#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    this->timer = new QTimer();
    this->timer->setInterval(2000);
    ui.ProcessTable->setEditTriggers(QTableWidget::NoEditTriggers); 

    connect(this->timer, SIGNAL(timeout()), this , SLOT(TimerRound()));
    connect(ui.pushButton, SIGNAL(released()), this, SLOT(KillProcess()));
    
    TimerRound();
    this->timer->start();
}

void mainwindow::KillProcess()
{
    DWORD ProcessID;
    ProcessID = ui.TextEditPID->text().toInt();
    ProcessFunc.KillProcessByID(ProcessID);
}

void mainwindow::TimerRound()
{
    QList<ListRow> ProcessList = ProcessFunc.GetProcessListInfo();
    ui.ProcessTable->setSortingEnabled(false);
    ui.ProcessTable->clearContents();
    ui.ProcessTable->setRowCount(ProcessList.size());
    int Row = 0;

    for (auto process : ProcessList)
    {
        ui.ProcessTable->setItem(Row, 0, std::get<0>(process));
        ui.ProcessTable->setItem(Row, 1, std::get<1>(process));
        ui.ProcessTable->setItem(Row, 2, std::get<2>(process));
        ui.ProcessTable->setItem(Row, 3, std::get<3>(process));
        ui.ProcessTable->setItem(Row, 4, std::get<4>(process));
        Row++;
    }
    ui.ProcessTable->setSortingEnabled(true);
}
