#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <stdio.h>
#include <ctime>
#include <chrono>

#define PATH "D:\\KNU\\2kurs\\AOC\\Lab1\\Server\\serverlog.txt"

using namespace std;
using namespace chrono;

int Counter = 0;
const char AUTHOR[] = "Chobotok Vladyslav, 4th variant";

ofstream serverlog(PATH);

SOCKET Connections[3];

void ClientHandler(int index) {
    char msg[256];
    while (true) {
        recv(Connections[index], msg, sizeof(msg), NULL);
        string message = msg;
        if (message == "exit") {
            time_t t = system_clock::to_time_t(system_clock::now());
            serverlog << "Client " << index << ": " << message << ". Command. Time: " << ctime(&t) << endl;
            Sleep(10);
            break;
        }

        for (int i = 0; i < Counter; i++) {
            if (i == index) {
                continue;
            }
            if (message == "Who") {
                send(Connections[index], AUTHOR, sizeof(AUTHOR), NULL);
                time_t t = system_clock::to_time_t(system_clock::now());
                serverlog << "Client " << index << ": " << message << ". Command. Time: " << ctime(&t) << endl;
                Sleep(10);
            }else {
                send(Connections[i], msg, sizeof(msg), NULL);
                time_t t = system_clock::to_time_t(system_clock::now());
                if(message.length() <= 20) {
                    serverlog << "Client " << index << ": " << message << ". Message. Time: " << ctime(&t) << endl;
                    Sleep(10);
                }
                else {
                    string currMsg;
                    while(message.length() > 20)
                    {
                        currMsg = message.substr(0, 20);
                        message = message.substr(20, message.length() - 20);
                        serverlog << "Client " << index << ": " << currMsg << ". Message. Time: " << ctime(&t) << endl;
                        Sleep(10);
                    }
                    serverlog << "Client " << index << ": " << message << ". Message. Time: " << ctime(&t) << endl;
                    Sleep(10);
                }
            }
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

    Addr init;
    SOCKADDR_IN addr = init.getAddr();
    int sizeOfAddr = sizeof(addr);

    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 2);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        cout << "Error" << endl;
        exit(1);
    }

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR *) &addr, sizeOfAddr);
    listen(sListen, SOMAXCONN);

    SOCKET newConnection;
    for (int i = 0; i < 3; ++i) {
        newConnection = accept(sListen, (SOCKADDR *) &addr, &sizeOfAddr);

        if (newConnection == 0) {
            cout << "Error connecting to server" << endl;
        } else {
            cout << "Client " << i << " connected" << endl;
            time_t t = system_clock::to_time_t(system_clock::now());
            serverlog << "Client " << i << " connected. Time: " << ctime(&t) << endl;
            Sleep(10);
            Connections[i] = newConnection;
            Counter++;
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) ClientHandler, (LPVOID) (i), NULL, NULL);
        }
    }

    return 0;
}
