#pragma once
#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <process.h>
#include <mutex>
#include <string>
#define err(errMsg) printf("[error] %s failed,code %d\
line:%d\n", errMsg, WSAGetLastError(), __LINE__)
#pragma comment(lib, "ws2_32.lib")

using namespace std;
class Socket {
private:
	SOCKET clientSock;
	int id;
	string ip;
	char wb_file[MAXBYTE];
public:
	SOCKET GetCientSock();
	void OBTTON_TIME();
	double START_TIME();
	double END_TIME();
	void getByteSize(unsigned long long size);
	unsigned long long RETURN_MB(unsigned long long size);
	bool TARGE_FILE(string ip);
	void SEND_FILE(string file);
	static DWORD WINAPI transmmit(const LPVOID arg);
	bool INPUT_IP(string ipt);
	int MAIN_SOCKET();
	void CLEAR();
};
typedef struct my_file
{
	SOCKET clientSocket;
	sockaddr_in clientAddr;
	int id;
}F;
#endif