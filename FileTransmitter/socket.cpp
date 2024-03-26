#include "tcpSocket.h"
int main() {
	Socket* socket = new Socket();
	char Tempoary[1024] = { 0 };
	cout << "Socket:" << endl;
	while (true) {
		cout << ">";
		string op;
		cin >> op;
		if (op == "connect") {
			string ip;
			cin >> ip;
			bool flag = socket->IsConnect(ip);
			if (flag == true) continue;
			else {
				cout << "IP地址错误或者目标主机不存在。" << endl;
			}
		}
		else if (op == "list") {
			socket->PrintIp();
		}
		else if (op == "send") {
			string filePath;
			cin >> filePath;
			socket->SendFilePath(filePath);
			int ret = recv(socket->GetClientSock(), Tempoary, 10, 0);
			if (ret < 0) continue;
			socket->Transmmit(socket);
		}
		else if (op == "listen") {
			socket->Listen();
		}
		else if (op == "save") {
			cout << "输入保存文件路径：";
			string folderPath;
			cin >> folderPath;
			string st = "ESC";
			string fileName = socket->GetFileName();
			send(socket->GetClientSock(), st.c_str(), st.length(), 0);
			socket->GetFilePath(folderPath, fileName);
			socket->RecvFile();
		}
		else if (op == "exit") {
			socket->Clear();
			break;
		}
	}
	return 0;
}