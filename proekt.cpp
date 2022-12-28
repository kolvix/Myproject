#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>

HANDLE hSerial;
LPCTSTR PortName = L"\\\\.\\COM3";
bool bContinue = true;

const DWORD dwBUFF_SIZE = 9;

static float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
	static unsigned long long _previousTotalTicks = 0;
	static unsigned long long _previousIdleTicks = 0;

	unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
	unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;

	float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);

	_previousTotalTicks = totalTicks;
	_previousIdleTicks = idleTicks;
	return ret;
}

static unsigned long long FileTimeToInt64(const FILETIME& ft) { return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime); }

// Returns 1.0f for "CPU fully pinned", 0.0f for "CPU idle", or somewhere in between
// You'll need to call this at regular intervals, since it measures the load between
// the previous call and the current one.  Returns -1.0 on error.
void GetLoad(char* msg)
{
	//CPU Load
	FILETIME idleTime, kernelTime, userTime;

	int cpu = GetSystemTimes(&idleTime, &kernelTime, &userTime) ? (int)(CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) * 100) : -1;

	//RAM Load
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);

	int ram = memInfo.dwMemoryLoad;


	_snprintf_s(msg, dwBUFF_SIZE, 9, "!%d-%d;", cpu, ram);
}

DWORD WINAPI SendToPortThread(LPVOID t)
{
	hSerial = CreateFile(PortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		_tprintf_s(_T("Coudn't find %s port(%d)\n"), PortName, GetLastError());
		ExitThread(-1);
	}

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if (!GetCommState(hSerial, &dcbSerialParams))
	{
		_tprintf_s(_T("Coudn't get %s port state(%d)\n"), PortName, GetLastError());
		CloseHandle(hSerial);
		ExitThread(-1);
	}

	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	if (!SetCommState(hSerial, &dcbSerialParams))
	{
		_tprintf_s(_T("Coudn't set %s port state(%d)\n"), PortName, GetLastError());
		CloseHandle(hSerial);
		ExitThread(-1);
	}

	DWORD dwBytesWritten;

	while (bContinue)
	{
		char message[dwBUFF_SIZE];
		GetLoad(message);
		Sleep(2000);
		WriteFile(hSerial, message, dwBUFF_SIZE, &dwBytesWritten, NULL);
	}
	CloseHandle(hSerial);
	ExitThread(0);
}

int main()
{
	HANDLE Thread = CreateThread(NULL, 0, SendToPortThread, NULL, 0, NULL);
	if (Thread == INVALID_HANDLE_VALUE || Thread == NULL)
	{
		_tprintf_s(_T("Coundn't create thread(%d)\n"), GetLastError());
		return -1;
	}
	system("pause");
	bContinue = false;
	CloseHandle(Thread);
}