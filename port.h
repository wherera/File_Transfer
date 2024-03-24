#pragma once
#ifndef _PORT_H_
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <process.h>
#include <mutex>
#include <string>
#define err(errMsg) printf("[error] %s failed,code %d\
line:%d\n", errMsg, WSAGetLastError(), __LINE__)
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

using namespace std;
class port {
public:
	void LocalTime();
	double StartTime();
	double EndTime();
	void GetByteSize(unsigned long long size);
};


#endif