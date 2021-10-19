#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <fstream>
#include <ctime>
#include <chrono>

#define PATH "D:\\KNU\\2kurs\\AOC\\Lab1\\Client2\\clientlog.txt"

using namespace std;
using namespace chrono;

SOCKET Connection;
ofstream clientLog(PATH);

void ClientHandler() {
    char msg[256];
    while(true) {
        recv(Connection, msg, sizeof(msg), NULL);
        string message = msg;
        time_t t = system_clock::to_time_t(system_clock::now());
        if(message.length() <= 20) {
            clientLog << "Client received msg from server: " << msg <<". Time: " << ctime(&t) << endl;
            Sleep(10);
            cout << msg << endl;
        }
        else {
            string currMsg;
            while(message.length() > 20)
            {
                currMsg = message.substr(0, 20);
                message = message.substr(20, message.length() - 20);
                clientLog << "Client received msg from server: " << currMsg <<". Time: " << ctime(&t) << endl;
                Sleep(10);
                cout << currMsg << endl;
            }
            clientLog << "Client received msg from server: " << message <<". Time: " << ctime(&t) << endl;
            Sleep(10);
            cout << message << endl;
        }
    }
}

class Addr {
private:
    SOCKADDR_IN addr;
public:
    Addr(){
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        addr.sin_port = htons(1029);
        addr.sin_family = PF_INET;
    }

    SOCKADDR_IN getAddr(){
        return addr;
    }

};

int main() {

    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        cout << "Error" << endl;
        exit(1);
    }

    Addr init;
    SOCKADDR_IN addr = init.getAddr();
    int sizeOfAddr = sizeof(addr);

    Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(Connection, (SOCKADDR*)&addr, sizeOfAddr) != 0){
        cout << "Error connecting to the server" << endl;
        return 1;
    }else {
        cout << "Connected succesfully" << endl;
    }

    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

    char msg1[256];
    while (true){
        string message;
        cin.getline(msg1, sizeof(msg1));
        message = msg1;
        send(Connection, msg1, sizeof(msg1), NULL);
        time_t t = system_clock::to_time_t(system_clock::now());
        clientLog << "Client sent msg to server: " << msg1 <<" Time: " << ctime(&t) << endl;
        Sleep(10);
        if (message == "exit"){
            break;
        }
    }

    return 0;
}