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
    //获取clientSock
    SOCKET GetclientSock();
    //获取当前日期
    void OBTION_TIME();
    //获取开始时间
    double START_TIME();
    //获取结束时间
    double END_TIME();
    //获取文件大小
    void  getByteSize(unsigned long long size);
    //返回以MB为单位的文件大小
    unsigned long long RETURN_MB(unsigned long long size);
    //绑定和监听
    void TARGE_FILE();
    //返回文件的类型
    string TYPE_file();
    //LPVOID是一个没有类型的指针，也就是说你可以将任意类型的指针赋值给LPVOID类型的变量（一般作为参数传递）
    static DWORD WINAPI transmmit(const LPVOID arg);
    //系统的实现以上函数
    int MAIN_Server();
    //接收文件
    void REVER_file(string file, string fileanme);
    //清理网络库和关闭
    void CLEAR();
};
//定义结构体用来设置
typedef struct my_file {
    SOCKET clientSocket; //文件内部包含了一个SOCKET 用于和客户端进行通信
    sockaddr_in clientAddr; //用于保存客户端的socket地址
    int id; //文件块的序号
}F;

#endif
