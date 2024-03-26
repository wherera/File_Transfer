//#include "tcpSocket.h"
//int main() {
//	Socket* socket = new Socket();
//	char Tempoary[1024] = { 0 };
//	while (true) {
//		string ip;
//		cout << "请输入目标机器的IP：";
//		cin >> ip;
//		bool flag = socket->TARGE_FILE(ip);
//		if (flag == true) break;
//		else {
//			cout << "IP地址错误或者目标主机不存在。" << endl;
//			continue;
//		}
//	}
//
//	while (true) {
//		cout << "其次请客户端输入传输文件路径：";
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