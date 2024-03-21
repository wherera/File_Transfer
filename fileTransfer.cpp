#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h> // 用于获取广播地址

#pragma comment(lib, "ws2_32.lib")

constexpr int PORT = 12345; // 广播端口
constexpr int BUFFER_SIZE = 1024; // 缓冲区大小

// 发送UDP广播消息
void send_broadcast_message(const char* broadcastAddress, const char* message) {
    // 初始化Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return;
    }

    // 创建UDP套接字
    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // 设置广播选项
    BOOL broadcastEnabled = TRUE;
    if (setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnabled, sizeof(broadcastEnabled)) == SOCKET_ERROR) {
        std::cerr << "setsockopt failed with error: " << WSAGetLastError() << std::endl;
        closesocket(udpSocket);
        WSACleanup();
        return;
    }

    // 构造广播地址
    sockaddr_in broadcastAddr;
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, broadcastAddress, &broadcastAddr.sin_addr.s_addr);

    // 发送广播消息
    if (sendto(udpSocket, message, strlen(message), 0, (sockaddr*)&broadcastAddr, sizeof(broadcastAddr)) == SOCKET_ERROR) {
        std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
        closesocket(udpSocket);
        WSACleanup();
        return;
    }

    std::cout << "Broadcast message sent: " << message << std::endl;

    // 关闭套接字
    closesocket(udpSocket);
    WSACleanup();
}

// 接收UDP广播消息
void receive_broadcast_messages() {
    // 初始化Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return;
    }

    // 创建UDP套接字
    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // 绑定到所有网络接口上的广播端口
    sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(PORT);
    localAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(udpSocket, (sockaddr*)&localAddr, sizeof(localAddr)) == SOCKET_ERROR) {
        std::cerr << "bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(udpSocket);
        WSACleanup();
        return;
    }

    // 接收广播消息
    char buffer[BUFFER_SIZE];
    sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);
    int bytesReceived;
    while (true) {
        bytesReceived = recvfrom(udpSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&senderAddr, &senderAddrSize);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "recvfrom failed with error: " << WSAGetLastError() << std::endl;
            break;
        }

        // 将接收到的数据转换为字符串
        buffer[bytesReceived] = '\0';
        std::cout << "Received message from " << inet_ntoa(senderAddr.sin_addr) << ": " << buffer << std::endl;
    }

    // 关闭套接字
    closesocket(udpSocket);
    WSACleanup();
}

// TCP连接函数
SOCKET establish_tcp_connection(const char* ipAddress, int port) {
    // 创建TCP套接字
    SOCKET tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcpSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        return INVALID_SOCKET;
    }

    // 构造服务器地址
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ipAddress);

    // 连接到服务器
    if (connect(tcpSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect: " << WSAGetLastError() << std::endl;
        closesocket(tcpSocket);
        return INVALID_SOCKET;
    }

    return tcpSocket;
}

int main() {
    // 初始化Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // 创建UDP套接字
    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 绑定到指定端口
    sockaddr_in localAddress;
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(12345); // 替换为你的监听端口号
    localAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(udpSocket, (sockaddr*)&localAddress, sizeof(localAddress)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(udpSocket);
        WSACleanup();
        return 1;
    }

    // 接收广播消息
    char buffer[1024];
    sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);
    int bytesReceived = recvfrom(udpSocket, buffer, sizeof(buffer), 0, (sockaddr*)&senderAddr, &senderAddrSize);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "recvfrom failed with error: " << WSAGetLastError() << std::endl;
        closesocket(udpSocket);
        WSACleanup();
        return 1;
    }

    // 解析并处理收到的消息
    std::cout << "Received message from " << inet_ntoa(senderAddr.sin_addr) << ": " << buffer << std::endl;

    // 关闭UDP套接字
    closesocket(udpSocket);

    // 获取广播设备的IP地址
    std::string deviceIPAddress = inet_ntoa(senderAddr.sin_addr);

    // 建立TCP连接
    SOCKET tcpSocket = establish_tcp_connection(deviceIPAddress.c_str(), 12346); // 替换为你的TCP端口号
    if (tcpSocket == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    // 连接已建立，可以在此进行文件传输等操作

    // 关闭TCP套接字
    closesocket(tcpSocket);

    WSACleanup();

    return 0;
}
