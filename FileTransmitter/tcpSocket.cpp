#include "tcpSocket.h"
#define MAXBYTES 300*1024
//#define CLOCKS_PER_SEC((clock_t)1000)
mutex m;

void Socket::LocalTime() {
	SYSTEMTIME time;
	GetLocalTime(&time);
	cout << time.wYear << "/" << time.wMonth << "/" << time.wDay
		<< " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << endl;
}

double Socket::StartTime() {
	DWORD startTime;
	startTime = GetTickCount64();
	return double(startTime);
}

double Socket::EndTime() {
	DWORD endTime;
	endTime = GetTickCount64();
	return double(endTime);
}

void Socket::GetByteSize(unsigned long long size) {
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

SOCKET Socket::GetClientSock() {
	return clientSock;
}

void Socket::SendFilePath(string filePath) {
	int i = 0;
	char Temporary_file[MAXBYTE] = { 0 };
	for (i = 0; i < filePath.length(); i++) {
		wb_file[i] = filePath[i];
		Temporary_file[i] = filePath[i];
	}
	wb_file[i] = '\0';
	Temporary_file[i] = '\0';
	send(clientSock, Temporary_file, strlen(Temporary_file), 0);
}

DWORD WINAPI Socket::Transmmit(const LPVOID arg) {
	m.lock();

	Socket* socket = (Socket*)arg;

	cout << "�ȴ�����˷�����Ϣ..." << endl;

	char* file_name;
	char File_Alias[100] = { 0 };
	file_name = socket->wb_file;
	unsigned long long len_file = 0;
	FILE* fp = fopen(file_name, "rb");

	if (fp == NULL) {
		cout << "�ļ�" << file_name << "����򲻴���" << endl;
	}
	else {
		unsigned long long g_fileSize;
		struct _stat64 st;
		_stat64(file_name, &st);
		g_fileSize = st.st_size;
		string send_file_len;
		send_file_len = to_string(g_fileSize);
		send(socket->clientSock, send_file_len.c_str(), send_file_len.length(), 0);
		cout << "�����ļ�ʱ�䣺";
		socket->LocalTime();
		double start_time = socket->StartTime();
		char Buffer[MAXBYTES] = { 0 };
		unsigned long long size = 0;
		unsigned long long Actual_file_len = 0;
		while ((size = fread(Buffer, sizeof(char), MAXBYTES, fp)) > 0) {
			if (send(socket->clientSock, Buffer, (unsigned long long)size, NULL) < 0)
			{
				cout << "������������������á�" << endl;
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
		cout << socket->id << "�߳��ѳɹ�����" << file_name << endl;

		cout << "�����ļ���С��";
		socket->GetByteSize(len_file);
		cout << "�ļ����ͽ���ʱ�䣺";
		socket->LocalTime();
		double end_time = socket->EndTime();
		double currentTime = 0;
		currentTime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
		cout << "�����ļ���ʱ��" << currentTime << "s" << endl;
		fclose(fp);
	}
	m.unlock();
	return 0;
}

bool Socket::Connect(string ipt) {
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
	else {
		ip = ipt;
		return true;
	}
}

bool Socket::SendIsConnect() {
	
	if (ip.empty()) {
		return false;
	}
	const char* testMessage = "Test";
	int result = send(clientSock, testMessage, strlen(testMessage), 0);

	if (result == SOCKET_ERROR) {
		int errorCode = WSAGetLastError();
		if (errorCode == WSAECONNRESET || errorCode == WSAECONNABORTED || errorCode == WSAENETRESET) {
			return false;
		}
		else {
			return true;
		}
	}
	else {
		return true;
	}
}

bool Socket::SaveIsConnect() {
	if (ip.empty()) {
		return false;
	}
	char Tempoary[1024] = { 0 };
	int result = recv(clientSock, Tempoary, 10, 0);
	if (result < 0) return false;
	else {
		return true;
	}
}

void Socket::Listen() {
	//���������
	WSADATA wsaData;
	//��һ��������winsocket load�İ汾�ţ�2.2��
	if (WSAStartup(MAKEWORD(2, 3), &wsaData) != 0) {
		err("WSAStartup");
		return;
	}
	//�����������˵�socket��Э���壬 sokcet���ͣ�
	serverSock = socket(AF_INET, SOCK_STREAM, 0);//����ĳ�SOCK_DGRAM��ʹ��UDP
	if (serverSock == INVALID_SOCKET) {
		err("SOCKET");
		return;
	}
	sockaddr_in servAddr; //��������socket��ַ������sin_addr��ʾIP��ַ��sin_port���ֶ˿ںź�sin_zero����ֽ�
	memset(&servAddr, 0, sizeof(SOCKADDR)); //��ʼ��socket��ַ
	servAddr.sin_family = AF_INET; //����ʹ�õ�Э����
	servAddr.sin_port = htons(3725); //����ʹ�õĶ˿�
	servAddr.sin_addr.s_addr = INADDR_ANY; //define s_addr = S_un.S_addr
	//��֮ǰ������servSocket�Ͷ˿ڣ�IP��ַ��
	if (bind(serverSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		err("bind");
		return;
	}
	listen(serverSock, 1); //�����������˿�
	sockaddr_in clntAddr;
	int nSize = sizeof(clntAddr);
	cout << "�ȴ�����..." << endl;
	clientSock = accept(serverSock, (SOCKADDR*)&clntAddr, &nSize);
	if (clientSock == INVALID_SOCKET) {
		err("accept");
	}
	cout << "���ӳɹ�" << endl;

	ip = inet_ntoa(clntAddr.sin_addr);
}

string Socket::GetFileName() {
	string fileName = "";
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
			fileName += file[i];
		}
	}
	return fileName;
}

void Socket::GetFilePath(string folderPath, string fileName) {
	int i = 0;
	int len_file = folderPath.length();
	int len_filename = fileName.length();
	for (i = 0; i < len_file; i++) {
		wb_file[i] = folderPath[i];
	}
	int j = 0;
	for (i = len_file; i < (len_file + len_filename) && j < len_filename; j++, i++) {
		wb_file[i] = fileName[j];
	}
	wb_file[i] = '\0';
}

int Socket::RecvFile() {
	char Buffer[MAXBYTES] = { 0 }; // �ļ�������
	char wb_files[MAXBYTE] = { 0 };
	FILE* fp = fopen(wb_file, "wb");
	//���¼���ļ������ڵĻ��ʹ���һ���µ��ļ�
	if (fp == NULL) {
		fp = fopen(wb_file, "w");
	}
	unsigned long long len_file = 0;
	if (fp == NULL) {
		cout << "�����ļ�ʱ����" << endl;
		system("pause");
	}
	else {
		cout << "�����ļ�ʱ��: ";
		LocalTime();
		unsigned long long g_fileSize = 0;
		char rev_buffer[MAXBYTES] = { 0 };//�����ļ��ĳ���
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
		//���ɹ������ļ���size > 0��ʱ���ж�д���ʱ���ļ������Ƿ���ڽ��յĳ���
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
					cout << "д����������ļ�ȱʧ��" << endl;
					break;
				}
				cout.width(3);//i�����Ϊ3λ��
				if ((len_file * 100 / g_fileSize) % 5 > 0) {
					cout << (len_file * 100 / g_fileSize) << "%";
					cout << "\b\b\b\b";//��ɾ�����ַ���ʹ������ԭ�ر仯
				}
				break;
			}
			else {
				if (fwrite(Buffer, sizeof(char), size, fp) < size) {
					cout << "д����������ļ�ȱʧ��" << endl;
				}
				len_file += size;
			}
			cout.width(3);//i�����Ϊ3λ��
			if ((len_file * 100 / g_fileSize) % 5 > 0) {
				cout << (len_file * 100 / g_fileSize) << "%";
				cout << "\b\b\b\b";//��ɾ�����ַ���ʹ������ԭ�ر仯
			}
			//��ջ������Ա���һ�ν���
			memset(&Buffer, 0, MAXBYTE);
		}
		cout << "�������" << endl;
		cout << "�����ļ���С: ";
		len_file = (unsigned long long)len_file;
		GetByteSize(len_file);
		cout << "�ļ���������ʱ��: ";
		LocalTime();
		double end_time = EndTime();
		double currentTime = 0;
		currentTime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
		cout << "�����ļ���ʱ: " << currentTime << "s" << endl;
		fclose(fp);
	}
	return 0;
}

void Socket::PrintIp() {
	if (ip == "") {
		cout << "NULL" << endl;
	}
	else {
		cout << "IP:" << " " << ip << endl;
	}
}

void Socket::ShowHelp() {
	std::cout << "Available commands:\n"
		<< "connect [ip]   - Connect to the specified IP address.\n"
		<< "list           - Print list of connected IP addresses.\n"
		<< "send [file]    - Send file to connected socket.\n"
		<< "listen         - Listen for incoming connections.\n"
		<< "save           - Save received file.\n"
		<< "exit           - Close socket and exit.\n"
		<< "help           - Display this help message.\n";
}

void Socket::Clear() {
	closesocket(clientSock);
	if (WSACleanup() != 0) {
		err("WSACleanup");
		return;
	}
	cout << "�����ѹرա�" << endl;
	system("pause");
}