#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
#include <string>
#pragma  comment (lib, "Ws2_32.lib")
#pragma warning (disable: 4996)
#define serv_port 80
#define  BUF_SIZE 1024

using namespace std;

int clean()
{
    if (WSACleanup() == SOCKET_ERROR) {
        cout << "WSACleanup error: " << WSAGetLastError() << endl; //Ошибка очищения
    }
    return 1;
}

int main()
{
    WSADATA ws; //WSADATA - структура, в которой сведения о конкретной реализации интерфейса Winsock

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

    hostent* hn; //получаем IP-адрес сервера по символьному имени
    if ((hn = gethostbyname("www.json.org")) == NULL) { //localhost //www.json.org
        cout << "gethostbyname error: " << WSAGetLastError() << '\n';
        closesocket(s);
        return clean();
    }

    //задание параметров сервера
    sockaddr_in clnt_sin;
    ZeroMemory(&clnt_sin, sizeof(clnt_sin));

    clnt_sin.sin_family = AF_INET;
    clnt_sin.sin_port = htons(serv_port); //htons - преобразование в сетевой формат порта из формата Intel
    ((unsigned long*)&clnt_sin.sin_addr)[0] = ((unsigned long**)hn->h_addr_list)[0][0];

    if (connect(s, (sockaddr*)&clnt_sin, sizeof(clnt_sin)) == SOCKET_ERROR)//устанавливаем соединение
    {
        cout << WSAGetLastError() << "Error SOCKET_ERROR" << endl;
        closesocket(s);
        return clean();
    }

    string request = "GET / HTTP/1.1\r\nHost: www.json.org\r\n\r\n"; // посылаем GET-запрос
    //GET / HTTP/1.1\r\nHost: localhost\r\n\r\n
    //GET / HTTP/1.1\r\nHost: www.json.org\r\n\r\n

    if (SOCKET_ERROR == send(s, (char*)request.c_str(), sizeof(request), 0)) //посылаем запрос серверу
    {
        cout << "send error: " << WSAGetLastError() << endl;
        closesocket(s);
        return clean();
    }



    char buf[BUF_SIZE] = { 0 };
    int len = 0;

    //получаем данные по частям, пока len не 0
    do
    {
        //получение сообщения от сервера
        if (SOCKET_ERROR == (len = recv(s, (char*)&buf, BUF_SIZE, 0)))
        {
            cout << "recv error: " << WSAGetLastError() << endl;
                closesocket(s);
                return clean();
        }

        cout << buf;
        

    } while (len != 0);

    if (SOCKET_ERROR == closesocket(s))
    {
        return 1;
        return clean();
    }

    clean();
    return 0;
}
