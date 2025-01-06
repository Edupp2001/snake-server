#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <chrono>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <WinSock2.h>
#include <conio.h>
#include <time.h>
#include <ctime>
#include <thread>
#include "snakelogic.h"
#pragma comment(lib, "ws2_32.lib")
using namespace std;
WSADATA wsd;
vector <int> free_id(90);
int id = 0, kos = 0;


string info(int a) {
	string s = "   " + ITS(a);
	reverse(s.begin(), s.end());
	s.resize(4);
	reverse(s.begin(), s.end());
	return s;
}


//inet_ntoa(@sockaddr_in.sin_addr@) - ip(string)

int main() {
	generate();
	WSAStartup(MAKEWORD(2, 2), &wsd);
	srand((unsigned int)time(NULL));
	for (int i = 1, k = 0; i <= 10; ++i)
		for (int j = 0; j < 9; ++j)
			free_id[k++] = -(i + j * 10);
	thread mainthr(gamethread);
	mainthr.detach();
	while (1) {
		SOCKET listener = socket(AF_INET, SOCK_STREAM, 0); //Создаем слушающий сокет
		if (listener == INVALID_SOCKET)
			cout << "Error with creating socket" << endl; //Ошибка создания сокета
		fd_set list;
		list.fd_array[50] = listener;
		sockaddr_in addr; //Создаем и заполняем переменную для хранения адреса
		addr.sin_family = AF_INET; //Семейство адресов, которые будет обрабатывать наш сервер, у нас это TCP/IP адреса
		addr.sin_port = htons(3128); //Используем функцию htons для перевода номера порта в TCP/IP представление
		addr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY означает, что сервер будет принимать запросы с любых IP
		if (SOCKET_ERROR == ::bind(listener, (struct sockaddr*)&addr, sizeof(addr))) //Связываем сокет с адресом
			cout << "Error with binding socket";

		char buf[2000];
		listen(listener, 1);
		int len = sizeof(addr);
		SOCKET curr_sock;
		if (FAILED(curr_sock = accept(listener, (struct sockaddr*)&addr, &len)))
			cout << "fail" << endl;
		memset(buf, 0, 2000);
		int n = recv(curr_sock, buf, 2000, 0);
		string data = buf;
		if (players[inet_ntoa(addr.sin_addr)].size != 0) {
			if (to_lower(data) == "w" && players[inet_ntoa(addr.sin_addr)].way != 's') {
				players[inet_ntoa(addr.sin_addr)].way = 'w';
				send(curr_sock, data.c_str(), data.size(), 0);
			}
			else if (to_lower(data) == "left" && players[inet_ntoa(addr.sin_addr)].way != 'd') {
				players[inet_ntoa(addr.sin_addr)].way = 'a';
				send(curr_sock, data.c_str(), data.size(), 0);
			}
			else if (to_lower(data) == "down" && players[inet_ntoa(addr.sin_addr)].way != 'w') {
				players[inet_ntoa(addr.sin_addr)].way = 's';
				send(curr_sock, data.c_str(), data.size(), 0);
			}
			else if (to_lower(data) == "right" && players[inet_ntoa(addr.sin_addr)].way != 'a') {
				players[inet_ntoa(addr.sin_addr)].way = 'd';
				send(curr_sock, data.c_str(), data.size(), 0);
			}
			else if (to_lower(data) == "stay") {
				players[inet_ntoa(addr.sin_addr)].way = 'g';
				send(curr_sock, data.c_str(), data.size(), 0);
			}
			else if (data == "7") {
				string answer = "";
				for (int i = 0; i < field.size(); ++i) {
					for (int j = 0; j < field[i].size(); ++j) {
						answer += info(field[i][j]);
					}
					answer += '\n';
				}
				answer += "'WASD' to move, 'G' to stay\n";
				send(curr_sock, answer.c_str(), answer.size(), 0);

			}
			else {
				data += "error";
				send(curr_sock, data.c_str(), data.size(), 0);
			}
		}
		else {
			players[inet_ntoa(addr.sin_addr)].id = free_id[id];//rand int from -99 to -1 without copies
			++id;
			pair <int, int> place = spawn();
			if (place.first != 0) {
				players[inet_ntoa(addr.sin_addr)].place = place;
				field[place.first][place.second] = players[inet_ntoa(addr.sin_addr)].id;
			}
			else {
				string sorry = "there is no place";
				send(curr_sock, sorry.c_str(), sorry.size(), 0);

			}

			players[inet_ntoa(addr.sin_addr)].size = 4;
			players[inet_ntoa(addr.sin_addr)].way = 'g';
		}

		closesocket(curr_sock);
		closesocket(listener);
		kos = 0;
	}
}
