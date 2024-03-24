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

    //绑定和监听
    void TARGE_FILE();
    //返回文件的类型
    string TYPE_file();
    //系统的实现以上函数
    int MAIN_Server();
    //接收文件
    void REVER_file(string file, string fileanme);
    void Clear();
};
//定义结构体用来设置
typedef struct my_file {
    SOCKET clientSocket; //文件内部包含了一个SOCKET 用于和客户端进行通信
    sockaddr_in clientAddr; //用于保存客户端的socket地址
    int id; //文件块的序号
}F;

#endif
