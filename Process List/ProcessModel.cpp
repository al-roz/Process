#include "ProcessModel.h"



ProcessModel::ProcessModel()
{
	Processes = GetProcess();
}

QList<ListRow> ProcessModel::GetProcessListInfo()
{
	Processes = GetProcess();
	return Processes;
}

void ProcessModel::KillProcessByID(DWORD ProcessID)
{
		HANDLE ProcessHandle = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessID);
		DWORD exitcode;
		TerminateProcess(ProcessHandle,GetExitCodeProcess(ProcessHandle, &exitcode));
}

QList<ListRow> ProcessModel::GetProcess()
{
	HANDLE ProcessHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe32;
	QList<PROCESSENTRY32> ProcessList;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(ProcessHandle, &pe32))
	{
		CloseHandle(ProcessHandle);
		return QList<ListRow>();
	}

	do
	{
		ProcessList.push_back(pe32);
	} while (Process32Next(ProcessHandle, &pe32));

	return ParseProcessList(ProcessList);

}

long ProcessModel::GetProcessMemory(unsigned long ProcessID)
{
	auto ProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcessID);
	PROCESS_MEMORY_COUNTERS Memory;
	GetProcessMemoryInfo(ProcessHandle, &Memory, sizeof(Memory));
	return Memory.WorkingSetSize/1024;
}

QString ProcessModel::GetProcessPriority(unsigned long ProcessID)
{

	auto ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
	auto Priority = GetPriorityClass(ProcessHandle);
	QString PriorityString;
	switch (Priority)
	{
	case ABOVE_NORMAL_PRIORITY_CLASS:
		PriorityString = QString::fromWCharArray(L"Выше среднего");
		break;

	case BELOW_NORMAL_PRIORITY_CLASS:
		PriorityString = QString::fromWCharArray(L"Ниже среднего");
		break;

	case NORMAL_PRIORITY_CLASS:
		PriorityString = QString::fromWCharArray(L"Средний");
		break;

	case HIGH_PRIORITY_CLASS:
		PriorityString = QString::fromWCharArray(L"Высокий");
		break;

	case IDLE_PRIORITY_CLASS:
		PriorityString = QString::fromWCharArray(L"Простаивает");
		break;

	case REALTIME_PRIORITY_CLASS:
		PriorityString = QString::fromWCharArray(L"Реального времени");
		break;

	default:
		PriorityString = QString::fromWCharArray(L"Неизвестно");
		break;
	}
	return PriorityString;
}

QList<ListRow> ProcessModel::ParseProcessList(QList<PROCESSENTRY32>& ProcessList)
{
	QList<ListRow> result;
	for (auto Process : ProcessList)
	{
		ListRow tmp;
		QTableWidgetItem* ProcessID = new QTableWidgetItem(QString::number(Process.th32ProcessID));
		QTableWidgetItem* CntThreads = new QTableWidgetItem(QString::number(Process.cntThreads));
		QTableWidgetItem* szExeFile = new QTableWidgetItem(QString::fromWCharArray(Process.szExeFile));
		QTableWidgetItem* ProcessMemory = new QTableWidgetItem(QString::number(GetProcessMemory(Process.th32ProcessID)));
		QTableWidgetItem* ProcessPriority = new QTableWidgetItem(GetProcessPriority(Process.th32ProcessID));
		tmp = std::make_tuple(szExeFile, ProcessID, ProcessMemory, ProcessPriority,CntThreads);
		result.push_back(tmp);
	}

	return result;
}
