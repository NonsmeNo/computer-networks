//#define _WINSOCK_DEPRECATED_NO_WARNINGS

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

    SOCKET ls;
    ls = socket(AF_INET, SOCK_STREAM, 0);
    if (ls == INVALID_SOCKET)
    {
        cout << WSAGetLastError() << " Error INVALID_SOCKET" << endl; //ошибка создания сокета
        closesocket(ls);
        return clean();
    }

    //задание параметров сокета сервера
    sockaddr_in sin;
    ZeroMemory(&sin, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_port = htons(serv_port); //htons - типа преобразование в сетевой формат порта из формата Intel
    sin.sin_addr.s_addr = INADDR_ANY;

    //привязка адреса к сокету
    if (bind(ls, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        cout << WSAGetLastError() << " SOCKET_ERROR" << endl;
        closesocket(ls);
        return clean();
    }

    //режим ожидания запроса
    if (listen(ls, SOMAXCONN) == SOCKET_ERROR)
    {
        cout << "listen failed with error: " << WSAGetLastError() << endl;
        closesocket(ls);
        return clean();
    }

    SOCKET fs;
    sockaddr_in from_sin;
    char buf[BUF_SIZE] = {0};
    string m;

    while (1)
    {
        cout << "Waiting for client..." << endl;
        int from_len = sizeof(from_sin);
        fs = accept(ls, (sockaddr*)&from_sin, &from_len);//устанавливает соединение с клиентом из очереди
        cout << "WOW! New connected client :)" << endl;
        m = "Congratulate! You are connected to the server!";

        while (1) //общение с клиентом
        {
            //отправка сообщения
            send (fs, (char *)&m[0], m.size(), 0);

            //прием сообщения
            from_len = recv(fs, (char *)buf, BUF_SIZE, 0);

            m = "";
            cout << "client say: ";
            for (int i = 0; i < from_len; i++) 
                m += buf[i];

            cout << m << endl;
            if (m == "EXIT")
                break;

            cout << "Your message: ";
            getline(cin, m);

        }
        cout << "Client is lost" << endl;
        closesocket(fs);

        cout << endl;
    }

    closesocket(ls);
    return clean();

}
