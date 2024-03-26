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
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

using namespace std;
class Socket {
private:
	SOCKET clientSock;
	SOCKET serverSock;
	int id;
	string ip;
	char wb_file[MAXBYTE];
public:
	void LocalTime();
	double StartTime();
	double EndTime();
	void GetByteSize(unsigned long long size);
	SOCKET GetClientSock();
	bool SendIsConnect();
	bool SaveIsConnect();
	void PrintIp();
	void ShowHelp();
	void SendFilePath(string filePath);
	static DWORD WINAPI Transmmit(const LPVOID arg);
	bool Connect(string ipt);
	void Listen();
	string GetFileName();
	void GetFilePath(string folderPath, string fileName);
	int RecvFile();
	void Clear();
};
typedef struct my_file
{
	SOCKET clientSocket;
	sockaddr_in clientAddr;
	int id;
}F;
#endif