#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
#include <string>
#pragma  comment (lib, "Ws2_32.lib")
using namespace std;

#define serv_port 5000
#define CLIENT_PORT 1232
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

	SOCKET s;
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET)
	{
		cout << WSAGetLastError() << " Error INVALID_SOCKET" << endl; //ошибка создания сокета
		closesocket(s);
		clean();
		return 1;
	}

	//задание параметров сокета сервера в адресной структуре
	sockaddr_in sin;
	ZeroMemory(&sin, sizeof(sin));

	sin.sin_family = AF_INET;
	sin.sin_port = htons(CLIENT_PORT);
	sin.sin_addr.s_addr = INADDR_ANY;

	if (bind(s, (sockaddr*)&sin, sizeof(sin)) == INVALID_SOCKET) {
		cout << "bind error: " << WSAGetLastError() << '\n';
		closesocket(s);
		clean();
		return 1;
	}

	
	sockaddr_in from_sin;
	ZeroMemory(&from_sin, sizeof(from_sin));

	from_sin.sin_family = AF_INET;
	from_sin.sin_port = htons(serv_port);
	from_sin.sin_addr.s_addr = inet_addr("127.0.0.1");


	string m = "echo message";

	int remote_len;
	char buf[BUF_SIZE] = { 0 };


	while (true) {
		cout << "your message: ";
		getline(cin, m);

		if (m == "EXIT")
			break;

		sendto(s, (char*)&m[0], m.size(), 0, (sockaddr*)&from_sin, sizeof(from_sin));

		int from_sin_size = sizeof(from_sin);
		int buf_size = recvfrom(s, buf, sizeof(buf), 0, (sockaddr*)&from_sin, &from_sin_size);

		if (buf_size == SOCKET_ERROR)
		{
			cout << "recvfrom error: " << WSAGetLastError() << endl;
			clean();
			return 1;
		}

	}

	closesocket(s);
	clean();
	return 0;
}
