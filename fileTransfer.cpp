#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h> // ���ڻ�ȡ�㲥��ַ

#pragma comment(lib, "ws2_32.lib")

constexpr int PORT = 12345; // �㲥�˿�
constexpr int BUFFER_SIZE = 1024; // ��������С

// ����UDP�㲥��Ϣ
void send_broadcast_message(const char* broadcastAddress, const char* message) {
    // ��ʼ��Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return;
    }

    // ����UDP�׽���
    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // ���ù㲥ѡ��
    BOOL broadcastEnabled = TRUE;
    if (setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnabled, sizeof(broadcastEnabled)) == SOCKET_ERROR) {
        std::cerr << "setsockopt failed with error: " << WSAGetLastError() << std::endl;
        closesocket(udpSocket);
        WSACleanup();
        return;
    }

    // ����㲥��ַ
    sockaddr_in broadcastAddr;
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, broadcastAddress, &broadcastAddr.sin_addr.s_addr);

    // ���͹㲥��Ϣ
    if (sendto(udpSocket, message, strlen(message), 0, (sockaddr*)&broadcastAddr, sizeof(broadcastAddr)) == SOCKET_ERROR) {
        std::cerr << "sendto failed with error: " << WSAGetLastError() << std::endl;
        closesocket(udpSocket);
        WSACleanup();
        return;
    }

    std::cout << "Broadcast message sent: " << message << std::endl;

    // �ر��׽���
    closesocket(udpSocket);
    WSACleanup();
}

// ����UDP�㲥��Ϣ
void receive_broadcast_messages() {
    // ��ʼ��Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return;
    }

    // ����UDP�׽���
    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // �󶨵���������ӿ��ϵĹ㲥�˿�
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

    // ���չ㲥��Ϣ
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

        // �����յ�������ת��Ϊ�ַ���
        buffer[bytesReceived] = '\0';
        std::cout << "Received message from " << inet_ntoa(senderAddr.sin_addr) << ": " << buffer << std::endl;
    }

    // �ر��׽���
    closesocket(udpSocket);
    WSACleanup();
}

// TCP���Ӻ���
SOCKET establish_tcp_connection(const char* ipAddress, int port) {
    // ����TCP�׽���
    SOCKET tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcpSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        return INVALID_SOCKET;
    }

    // �����������ַ
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ipAddress);

    // ���ӵ�������
    if (connect(tcpSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect: " << WSAGetLastError() << std::endl;
        closesocket(tcpSocket);
        return INVALID_SOCKET;
    }

    return tcpSocket;
}

int main() {
    // ��ʼ��Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // ����UDP�׽���
    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // �󶨵�ָ���˿�
    sockaddr_in localAddress;
    localAddress.sin_family = AF_INET;
    localAddress.sin_port = htons(12345); // �滻Ϊ��ļ����˿ں�
    localAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(udpSocket, (sockaddr*)&localAddress, sizeof(localAddress)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(udpSocket);
        WSACleanup();
        return 1;
    }

    // ���չ㲥��Ϣ
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

    // �����������յ�����Ϣ
    std::cout << "Received message from " << inet_ntoa(senderAddr.sin_addr) << ": " << buffer << std::endl;

    // �ر�UDP�׽���
    closesocket(udpSocket);

    // ��ȡ�㲥�豸��IP��ַ
    std::string deviceIPAddress = inet_ntoa(senderAddr.sin_addr);

    // ����TCP����
    SOCKET tcpSocket = establish_tcp_connection(deviceIPAddress.c_str(), 12346); // �滻Ϊ���TCP�˿ں�
    if (tcpSocket == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    // �����ѽ����������ڴ˽����ļ�����Ȳ���

    // �ر�TCP�׽���
    closesocket(tcpSocket);

    WSACleanup();

    return 0;
}
