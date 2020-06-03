#include<winsock2.h>
#include<iostream>
#include<string>

#pragma comment(lib,"libwsock32.a")

using namespace std;

void ShowErrorMessage(string message){
    cout << "error happens" << message << "\n";
    system("pause");
    exit(1);
}

int main() {
    //WSAStartUp()
    WSADATA wsaData; //데이터 구조체
    SOCKET serverSocket, clientSocket;
    SOCKADDR_IN serverAddress, clientAddress;

    int serverPort = 9876;
    char received[256];

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//winsock 초기화 0이 나오면 성공
        ShowErrorMessage("WSAStartup()");
    //socket()
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);//TCP 소켓 생성 SOCK_STREAM-->TCP

    if(serverSocket == INVALID_SOCKET)
        ShowErrorMessage("socket()");
    memset(&serverAddress, 0, sizeof(serverAddress));//serverAdress를 0으로 초기화
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);//4바이트 정수를 네트워크 바이트 형식으로
    serverAddress.sin_port = htons(serverPort);//2바이트 정수를 네트워크 바이트 형식으로 
    //bind()
    if(bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
        ShowErrorMessage("bind()");
    cout << "[current status]: bind()\n";

    //listen()
    if(listen(serverSocket, 5) == SOCKET_ERROR)
        ShowErrorMessage("listen()");
    cout << "[current status]: listen()\n";
    //accept()
    int sizeClientAddress = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
    cout << "[current status]: accept()\n";
    
    if(clientSocket == INVALID_SOCKET)
        ShowErrorMessage("accept()");
    
    //클라이언트의 메시지를 받아서 그대로 다시 전달합니다.
    while(1){//recv()
        int length = recv(clientSocket, received, sizeof(received), 0);
        received[length] = '\0';
        cout << "[client message]: " << received << '\n';
        cout << "[send message]: " << received << '\n';
        if(!strcmp(received, "[exit]")){ // send()
            send(clientSocket, received, sizeof(received)-1, 0);
            cout << "[terminate server]\n";
            break;
        }
        send(clientSocket, received, sizeof(received)-1, 0);
    }
    //closesocket
    closesocket(clientSocket);
    closesocket(serverSocket);
    //WSACleanUp()
    WSACleanup();
    system("pause");
    return 0;
}