#include "tcpSocket.h"
#define MAXBYTES 300*1024
//#define CLOCKS_PER_SEC((clock_t)1000)
mutex m;

SOCKET Socket::GetClientSock() {
	return clientSock;
}

void Socket::SEND_FILE(string file) {
	int i = 0;
	char Temporary_file[MAXBYTE] = { 0 };
	for (i = 0; i < file.length(); i++) {
		wb_file[i] = file[i];
		Temporary_file[i] = file[i];
	}
	wb_file[i] = '\0';
	Temporary_file[i] = '\0';
	send(clientSock, Temporary_file, strlen(Temporary_file), 0);
}

DWORD WINAPI Socket::transmmit(const LPVOID arg) {
	m.lock();

	Socket* socket = (Socket*)arg;

	cout << "等待服务端发送消息..." << endl;

	char* file_name;
	char File_Alias[100] = { 0 };
	file_name = socket->wb_file;
	unsigned long long len_file = 0;
	FILE* fp = fopen(file_name, "rb");

	if (fp == NULL) {
		cout << "文件" << file_name << "出错或不存在" << endl;
	}
	else {
		unsigned long long g_fileSize;
		//fseek(fp, 0, SEEK_END);//将读取的文件指针放到文件末尾
		//g_fileSize = ftell(fp);
		//fseek(fp, 0, SEEK_SET);//指针移到文件开头
		struct _stat64 st;
		_stat64(file_name, &st);
		g_fileSize = st.st_size;
		string send_file_len;
		send_file_len = to_string(g_fileSize);
		send(socket->clientSock, send_file_len.c_str(), send_file_len.length(), 0);
		cout << "发送文件时间：";
		socket->LocalTime();
		double start_time = socket->StartTime();
		char Buffer[MAXBYTES] = { 0 };
		unsigned long long size = 0;
		unsigned long long Actual_file_len = 0;
		while ((size = fread(Buffer, sizeof(char), MAXBYTES, fp)) > 0) {
			if (send(socket->clientSock, Buffer, (unsigned long long)size, NULL) < 0)
			{
				cout << "传输错误，请检查网络配置。" << endl;
				break;
			}
			len_file += size;

			cout.width(3);
			if ((len_file * 100 / g_fileSize) % 5 > 0) {
				cout << (len_file * 100 / g_fileSize) << "%";
				cout << "\b\b\b\b";
			}
			size = 0;
			memset(&Buffer, 0, MAXBYTES);
		}
		const char* t = "end";
		send(socket->GetClientSock(), t, strlen(t), NULL);
		cout << socket->id << "线程已成功发送" << file_name << endl;

		cout << "发送文件大小：";
		socket->GetByteSize(len_file);
		cout << "文件发送结束时间：";
		socket->LocalTime();
		double end_time = socket->EndTime();
		double currentTime = 0;
		currentTime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
		cout << "发送文件耗时：" << currentTime << "s" << endl;
		fclose(fp);
	}
	m.unlock();
	return 0;
}

bool Socket::INPUT_IP(string ipt) {
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
		err("WSAStartup");
		return 0;
	}
	clientSock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSock == INVALID_SOCKET) {
		err("SOCKET");
		return 0;
	}
	sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(SOCKADDR));

	const char* ips = ipt.c_str();
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(3725);
	clientAddr.sin_addr.s_addr = inet_addr(ips);
	if (connect(clientSock, (SOCKADDR*)&clientAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		err("connect");
		return false;
	}
	return true;
}

bool Socket::TARGE_FILE(string ip) {
	bool flag = INPUT_IP(ip);
	if (flag == true) return 1;
	else {
		return 0;
	}
}

int Socket::MAIN_SOCKET() {
	return 0;
}

void Socket::Clear() {
	closesocket(clientSock);
	if (WSACleanup() != 0) {
		err("WSACleanup");
		return;
	}
	cout << "客户端连接已关闭。" << endl;
	system("pause");
}