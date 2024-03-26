
#include "tcpServer.h"
#define MAXBYTES 300*1024
mutex n;

SOCKET Server::GetClientSock() {
	return clientSock;
}

string Server::TYPE_file() {
	string end_file = "";
	char Temporary[1024] = { 0 };
	char file[1024] = { 0 };
	int index_last = 0;
	int ret = recv(GetClientSock(), file, 100, 0);
	for (int i = strlen(file) - 1; i >= 0; i--) {
		if (file[i] == '\\' || file[i] == '/') {
			index_last = i;
			break;
		}
	}
	index_last++;
	if (ret > 0) {
		file[ret] = '\0';
		for (int i = index_last; i < strlen(file); i++) {
			end_file += file[i];
		}
	}
	return end_file;
}
void Server::TARGE_FILE() {
	//加载网络库
	WSADATA wsaData;
	//第一个参数是winsocket load的版本号（2.2）
	if (WSAStartup(MAKEWORD(2, 3), &wsaData) != 0) {
		err("WSAStartup");
		return;
	}
	//创建服务器端的socket（协议族， sokcet类型）
	serverSock = socket(AF_INET, SOCK_STREAM, 0);//如果改成SOCK_DGRAM则使用UDP
	if (serverSock == INVALID_SOCKET) {
		err("SOCKET");
		return;
	}
	sockaddr_in servAddr; //服务器的socket地址，包含sin_addr表示IP地址，sin_port保持端口号和sin_zero填充字节
	memset(&servAddr, 0, sizeof(SOCKADDR)); //初始化socket地址
	servAddr.sin_family = AF_INET; //设置使用的协议族
	servAddr.sin_port = htons(3725); //设置使用的端口
	servAddr.sin_addr.s_addr = INADDR_ANY; //define s_addr = S_un.S_addr
	//将之前创建的servSocket和端口，IP地址绑定
	if (bind(serverSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		err("bind");
		return;
	}
	listen(serverSock, 1); //监听服务器端口
	sockaddr_in clntAddr;
	int nSize = sizeof(clntAddr);
	cout << "等待连接..." << endl;
	clientSock = accept(serverSock, (SOCKADDR*)&clntAddr, &nSize);
	if (clientSock == INVALID_SOCKET) {
		err("accept");
	}
	cout << "连接成功" << endl;
}
void Server::REVER_file(string file, string filename) {
	int i = 0;
	int len_file = file.length();
	int len_filename = filename.length();
	for (i = 0; i < len_file; i++) {
		wb_file[i] = file[i];
	}
	int j = 0;
	for (i = len_file; i < (len_file + len_filename) && j < len_filename; j++, i++) {
		wb_file[i] = filename[j];
	}
	wb_file[i] = '\0';
}

int Server::MAIN_Server() {
	char Buffer[MAXBYTES] = { 0 }; // 文件缓冲区
	char wb_files[MAXBYTE] = { 0 };
	FILE* fp = fopen(wb_file, "wb");
	//如果录入文件不存在的话就创建一个新的文件
	if (fp == NULL) {
		fp = fopen(wb_file, "w");
	}
	unsigned long long len_file = 0;
	if (fp == NULL) {
		cout << "操作文件时出错" << endl;
		system("pause");
	}
	else {
		cout << "接收文件时间: ";
		LocalTime();
		unsigned long long g_fileSize = 0;
		char rev_buffer[MAXBYTES] = { 0 };//接收文件的长度
		int rev_len = recv(clientSock, rev_buffer, MAXBYTE, 0);
		if (rev_len > 0) {
			rev_buffer[rev_len] = '\0';
			for (int i = 0; i < strlen(rev_buffer); i++) {
				g_fileSize = g_fileSize * 10 + ((unsigned long long)rev_buffer[i] - 48);
			}
		}
		double start_time = StartTime();
		memset(&Buffer, 0, MAXBYTES);
		unsigned long long  size = 0;
		//当成功接收文件（size > 0）时，判断写入的时候文件长度是否等于接收的长度
		while ((size = recv(clientSock, Buffer, MAXBYTES, 0)) > 0) {

			if (Buffer[size - 3] == 'e' && Buffer[size - 2] == 'n' && Buffer[size - 1] == 'd')
			{
				char buffer[MAXBYTES] = { 0 };
				for (int i = 0; i < strlen(Buffer) - 3; i++) {
					buffer[i] = Buffer[i];
				}
				len_file += size - 3;
				size -= 3;
				if (fwrite(buffer, sizeof(char), size, fp) < size) {
					cout << "写入出错，部分文件缺失。" << endl;
					break;
				}
				cout.width(3);//i的输出为3位宽
				if ((len_file * 100 / g_fileSize) % 5 > 0) {
					cout << (len_file * 100 / g_fileSize) << "%";
					cout << "\b\b\b\b";//回删三个字符，使数字在原地变化
				}
				break;
			}
			else {
				if (fwrite(Buffer, sizeof(char), size, fp) < size) {
					cout << "写入出错，部分文件缺失。" << endl;
				}
				len_file += size;
			}
			cout.width(3);//i的输出为3位宽
			if ((len_file * 100 / g_fileSize) % 5 > 0) {
				cout << (len_file * 100 / g_fileSize) << "%";
				cout << "\b\b\b\b";//回删三个字符，使数字在原地变化
			}
			//清空缓存区以便下一次接收
			memset(&Buffer, 0, MAXBYTE);
		}
		cout << "接收完成" << endl;
		cout << "接受文件大小: ";
		len_file = (unsigned long long)len_file;
		GetByteSize(len_file);
		cout << "文件结束接受时间: ";
		LocalTime();
		double end_time = EndTime();
		double currentTime = 0;
		currentTime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
		cout << "接收文件耗时: " << currentTime << "s" << endl;
		fclose(fp);
	}
	return 0;

}
void Server::Clear() {
	//关闭socket，释放winsock
	closesocket(serverSock);
	////关闭网络库 
	if (WSACleanup()) {
		err("WSACleanup");
		return;
	}
	cout << "服务器连接已关闭。" << endl;
	system("pause");
}
