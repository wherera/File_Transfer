#pragma once
#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_
#include "port.h"

using namespace std;
class Server:public port {
private:
    SOCKET clientSock;
	SOCKET serverSock;
	int id;
	char wb_file[MAXBYTE];
	char filename[MAXBYTE];
public:

    //�󶨺ͼ���
    void TARGE_FILE();
    //�����ļ�������
    string TYPE_file();
    //ϵͳ��ʵ�����Ϻ���
    int MAIN_Server();
    //�����ļ�
    void REVER_file(string file, string fileanme);
    void Clear();
};
//����ṹ����������
typedef struct my_file {
    SOCKET clientSocket; //�ļ��ڲ�������һ��SOCKET ���ںͿͻ��˽���ͨ��
    sockaddr_in clientAddr; //���ڱ���ͻ��˵�socket��ַ
    int id; //�ļ�������
}F;

#endif
