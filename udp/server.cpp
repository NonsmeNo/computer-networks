#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <iostream>
#include <winsock2.h>
#include <string>
#pragma  comment (lib, "Ws2_32.lib")
using namespace std;

#define serv_port 5000
#define  BUF_SIZE 64

void clean()
{
    if (WSACleanup() == SOCKET_ERROR) {
        cout << "WSACleanup error: " << WSAGetLastError() << endl; //Ошибка очищения
    }
}

int main()
{
    WSADATA ws; //WSADATA - структура, в которой сведения о конкретной реализации интерфе1йса Winsock

    //0 - состоялась нициализация
    if (WSAStartup(MAKEWORD(2, 2), &ws))
    {
        cout << WSAGetLastError() << " Error WSAStartup" << endl; //Ошибка инициализации сокетов 
        clean();
        return 1;
    }

    SOCKET ls;
    ls = socket(AF_INET, SOCK_DGRAM, 0);
    if (ls == INVALID_SOCKET)
    {
        cout << WSAGetLastError() << " Error INVALID_SOCKET" << endl; //ошибка создания сокета
        closesocket(ls);
        clean();
        return 1;
    }

    //задание параметров сокета сервера в адресной структуре
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
        clean();
        return 1;
    }

    sockaddr_in from_sin;
    char buf[BUF_SIZE];
    string m;

    cout << "waiting.." << endl;
    while (1)
    {
        int from_size = sizeof(from_sin);
        int buf_size = recvfrom(ls, &buf[0], sizeof(buf) - 1, 0, (sockaddr*)&from_sin, &from_size);
        if(buf_size == SOCKET_ERROR)
            cout << "recvfrom error: " << WSAGetLastError() << endl;

       //определение параметров клиента
        hostent* hst;
        hst = gethostbyaddr((char*)&from_sin.sin_addr, 4, AF_INET);
        cout << "NEW DATAGRAM! " << endl;
        cout << "NAME: " << ((hst) ? hst->h_name : "Unknown host") << " IP: " << inet_ntoa(from_sin.sin_addr) << " PORT: " << ntohs(from_sin.sin_port) << endl;
        buf[buf_size] = '\0'; //добавление завершающего нуля
        cout << "client say: " << buf << endl;
        cout << endl;

        sendto(ls, &buf[0], buf_size, 0, (sockaddr*)&from_sin, sizeof(from_sin));
    }
    return 0;
}
