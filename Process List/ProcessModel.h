#pragma once
#include <qobject.h>
#include <qtablewidget.h>

#include <Windows.h>
#include <TlHelp32.h>
#include <tuple>
#include <Psapi.h>

using ListRow = std::tuple<QTableWidgetItem*, QTableWidgetItem*, QTableWidgetItem*, QTableWidgetItem*, QTableWidgetItem*>;

class ProcessModel : public QObject
{
	Q_OBJECT
		QList<ListRow> Processes;
public:	
	ProcessModel();
	QList<ListRow> GetProcessListInfo();
	void KillProcessByID(DWORD);
private:
	QList<ListRow> GetProcess();
	long GetProcessMemory(unsigned long);
	QString GetProcessPriority(unsigned long);
	QList<ListRow> ParseProcessList(QList<PROCESSENTRY32>&);
};

