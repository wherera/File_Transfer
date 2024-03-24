#include "port.h"

SOCKET port::GetClientSock() {
	return clientSock;
}

void port::LocalTime() {
	SYSTEMTIME time;
	GetLocalTime(&time);
	cout << time.wYear << "/" << time.wMonth << "/" << time.wDay
		<< " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << endl;
}

double port::StartTime() {
	DWORD startTime;
	startTime = GetTickCount64();
	return double(startTime);
}

double port::EndTime() {
	DWORD endTime;
	endTime = GetTickCount64();
	return double(endTime);
}

void port::GetByteSize(unsigned long long size) {
	unsigned long long rest = 0;
	if (size < 1024) {
		cout << size << "B" << endl;
		return;
	}
	else {
		size /= 1024;
	}

	if (size < 1024) {
		cout << size << "KB" << endl;
		return;
	}
	else {
		rest = size % 1024;
		size /= 1024;
	}

	if (size < 1024) {
		size *= 100;
		cout << (size / 100) << "." << (rest * 100 / 1024 % 100) << "MB" << endl;
		return;
	}
	else {
		size = size * 100 / 1024;
		cout << (size / 100) << "." << (size % 100) << "GB" << endl;
		return;
	}
}
