#pragma once
#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <process.h>
#include <mutex>
#include <string>
#define err(errMsg) printf("[error] %s failed,code %d\
line:%d\n", errMsg, WSAGetLastError(), __LINE__)
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

using namespace std;
class Server {
private:
	SOCKET clientSock;
	SOCKET serverSock;
	int id;
	char wb_file[MAXBYTE];
	char filename[MAXBYTE];
public:
    //��ȡclientSock
    SOCKET GetclientSock();
    //��ȡ��ǰ����
    void OBTION_TIME();
    //��ȡ��ʼʱ��
    double START_TIME();
    //��ȡ����ʱ��
    double END_TIME();
    //��ȡ�ļ���С
    void  getByteSize(unsigned long long size);
    //������MBΪ��λ���ļ���С
    unsigned long long RETURN_MB(unsigned long long size);
    //�󶨺ͼ���
    void TARGE_FILE();
    //�����ļ�������
    string TYPE_file();
    //LPVOID��һ��û�����͵�ָ�룬Ҳ����˵����Խ��������͵�ָ�븳ֵ��LPVOID���͵ı�����һ����Ϊ�������ݣ�
    static DWORD WINAPI transmmit(const LPVOID arg);
    //ϵͳ��ʵ�����Ϻ���
    int MAIN_Server();
    //�����ļ�
    void REVER_file(string file, string fileanme);
    //���������͹ر�
    void CLEAR();
};
//����ṹ����������
typedef struct my_file {
    SOCKET clientSocket; //�ļ��ڲ�������һ��SOCKET ���ںͿͻ��˽���ͨ��
    sockaddr_in clientAddr; //���ڱ���ͻ��˵�socket��ַ
    int id; //�ļ�������
}F;

#endif
