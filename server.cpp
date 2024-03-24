#include "tcpServer.h"
int main() {

	Server* ser = new Server();
	ser->TARGE_FILE();
	string wb_file;
	while (true) {

		cout << "首先请服务端输入想要写入的文件(不用输入文件名): ";
		cin >> wb_file;
		string st = "ESC";
		string end_file = ser->TYPE_file();
		send(ser->GetclientSock(), st.c_str(), st.length(), 0);
		ser->REVER_file(wb_file, end_file);
		ser->MAIN_Server();
	}
	ser->CLEAR();
	return 0;
}
