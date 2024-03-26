
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
void Server::Clear() {
	//�ر�socket���ͷ�winsock
	closesocket(serverSock);
	////�ر������ 
	if (WSACleanup()) {
		err("WSACleanup");
		return;
	}
	cout << "�����������ѹرա�" << endl;
	system("pause");
}
