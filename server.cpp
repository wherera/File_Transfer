#include "tcpServer.h"
int main() {

	Server* ser = new Server();
	ser->TARGE_FILE();
	string wb_file;
	while (true) {

		cout << "����������������Ҫд����ļ�(���������ļ���): ";
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
