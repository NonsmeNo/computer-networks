#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <winsock2.h>
#include <string>
#pragma  comment (lib, "Ws2_32.lib")
using namespace std;

#define serv_port 80
#define  BUF_SIZE 1024
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
        cerr << WSAGetLastError() << " Error WSAStartup" << endl; //Ошибка инициализации сокетов 
        return clean();
    }

    SOCKET ls;
    ls = socket(AF_INET, SOCK_STREAM, 0);
    if (ls == INVALID_SOCKET)
    {
        cerr << WSAGetLastError() << " Error INVALID_SOCKET" << endl; //ошибка создания сокета
        closesocket(ls);
        return clean();
    }

    //задание параметров сокета сервера
    sockaddr_in sin;
    ZeroMemory(&sin, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_port = htons(serv_port); //htons - преобразование в сетевой формат порта из формата Intel
    sin.sin_addr.s_addr = INADDR_ANY;

    //привязка адреса к сокету
    if (bind(ls, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        cerr << WSAGetLastError() << " SOCKET_ERROR" << endl;
        closesocket(ls);
        return clean();
    }

    //инициализируем слушающий сокет
    if (listen(ls, SOMAXCONN) == SOCKET_ERROR)
    {
        cerr << "listen failed with error: " << WSAGetLastError() << endl;
        closesocket(ls);
        return clean();
    }

    SOCKET fs = INVALID_SOCKET;
    char buf[BUF_SIZE];
    cout << "Wait.." << endl;

    for(;;)

    {
        fs = accept(ls, NULL, NULL);//устанавливает соединение с клиентом
        if (fs == INVALID_SOCKET)
        {
            cerr << "accept failed: " << WSAGetLastError() << endl;
            closesocket(ls);
            return clean();
        }
        
        stringstream response;//ответ полный с заголовками
        stringstream response_body;//тело ответа

        int result = recv(fs, buf, BUF_SIZE, 0);//получение данных
        if (result == SOCKET_ERROR)//ошибка получения данных
        {
            cerr << "recv failed: " << result << endl;
            closesocket(fs);
            return clean();
        }
        else if (result == 0) //Соединение с клиентом закрыто
        {
            cerr << "connection closed..." << endl;
        }
        else if (result > 0) // Данные запроса успешно получены
        {
            buf[result] = 0; //очищаем буфер

            //формируем ответ
            response_body <<
                "<title> http server </title>\n"
                << "<h1>HELLO</h1>\n"
                << "<p>i'm server</p>" << endl;

            response <<
                "HTTP/1.1 200 OK\r\n"
                << "Version: HTTP/1.1\r\n"
                << "Content-Type: text/html; charset=utf-8\r\n"
                << "Content-Length: " << response_body.str().length() << "\r\n"
                << "\r\n\r\n" << response_body.str();


            //отправка данных
            result = send(fs, response.str().c_str(), response.str().length(), 0);
            if (result == SOCKET_ERROR)//ошибка отправки данных
            { 
                cerr << "send failed: " << result << endl;
                closesocket(fs);
                return clean();
            }
        }

    }

    closesocket(ls);
    clean();
    return 0;

}
