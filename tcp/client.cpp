#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
#include <string>
#pragma  comment (lib, "Ws2_32.lib")
using namespace std;

#define serv_port 5000
#define  BUF_SIZE 64
int clean()
{
    if (WSACleanup() == SOCKET_ERROR) {
        cout << "WSACleanup error: " << WSAGetLastError() << endl; //Ошибка очищения
    }
    return 1;
}

int main()
{
    WSADATA ws; //WSADATA - структура, в которой сведения о конкретной реализации интерфе1йса Winsock

    //0 - состоялась нициализация
    if (WSAStartup(MAKEWORD(2, 2), &ws))
    {
        cout << WSAGetLastError() << " Error WSAStartup" << endl; //Ошибка инициализации сокетов 
        return clean();
    }

    SOCKET s;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET)
    {
        cout << WSAGetLastError() << " Error INVALID_SOCKET" << endl; //ошибка создания сокета
        closesocket(s);
        return clean();
    }

    //задание параметров сокета сервера
    sockaddr_in clnt_sin;
    ZeroMemory(&clnt_sin, sizeof(clnt_sin));

    clnt_sin.sin_family = AF_INET;
    clnt_sin.sin_port = htons(serv_port); //htons - типа преобразование в сетевой формат порта из формата Intel
    clnt_sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(s, (sockaddr*)&clnt_sin, sizeof(clnt_sin)) == SOCKET_ERROR)
    {
        cout << WSAGetLastError() << "Error SOCKET_ERROR" << endl; //ошибка создания сокета
            closesocket(s);
            return clean();
    }
    



    char buf[BUF_SIZE] = {0};
    string m;

    //диалог с сервером, выход - EXIT
    do
    {

        //получение сообщения от сервера
        int from_len = recv(s, (char*)&buf, BUF_SIZE, 0);

        cout << "server say: ";
        for (int i = 0; i < from_len; i++) {
            cout << buf[i];
        }
        cout << endl;

        //отправка
        cout << "your message: ";
        getline(cin, m);
        send(s, (char*)&m[0], m.size(), 0);


    } while (m != "EXIT");

    cout << "You disconnected from the server :(" << endl;
}
