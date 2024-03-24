#pragma once
#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_
#include "port.h"

using namespace std;
class Socket:public port {
private:
	SOCKET clientSock;
	int id;
	string ip;
	char wb_file[MAXBYTE];
public:
	bool TARGE_FILE(string ip);
	void SEND_FILE(string file);
	static DWORD WINAPI transmmit(const LPVOID arg);
	bool INPUT_IP(string ipt);
	int MAIN_SOCKET();
	void Clear();
};
typedef struct my_file
{
	SOCKET clientSocket;
	sockaddr_in clientAddr;
	int id;
}F;
#endif