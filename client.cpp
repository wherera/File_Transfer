//#include "tcpSocket.h"
//int main() {
//	Socket* socket = new Socket();
//	char Tempoary[1024] = { 0 };
//	while (true) {
//		string ip;
//		cout << "������Ŀ�������IP��";
//		cin >> ip;
//		bool flag = socket->TARGE_FILE(ip);
//		if (flag == true) break;
//		else {
//			cout << "IP��ַ�������Ŀ�����������ڡ�" << endl;
//			continue;
//		}
//	}
//
//	while (true) {
//		cout << "�����ͻ������봫���ļ�·����";
//		string file;
//		cin >> file;
//		socket->SEND_FILE(file);
//		int ret = recv(socket->GetClientSock(), Tempoary, 10, 0);
//		if (ret < 0) continue;
//		socket->transmmit(socket);
//	}
//	socket->Clear();
//	return 0;
//} 