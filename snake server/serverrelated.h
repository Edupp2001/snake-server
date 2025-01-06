#pragma once
#include <iostream>

using namespace std;
WSADATA wsd;

pair <SOCKET, sockaddr_in> createlistener() {
	WSAStartup(MAKEWORD(2, 2), &wsd);
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
	return make_pair(listener, addr);
}

string info(int a) {//????
	string s = "   " + ITS(a);
	reverse(s.begin(), s.end());
	s.resize(4);
	reverse(s.begin(), s.end());
	return s;
}

void TalkToClient(string ip, SOCKET curr_sock) {
	int n = 1;
	//while n != -1
	while (1) {
		char buf[2000];
		memset(buf, 0, 2000);
		n = recv(curr_sock, buf, 2000, 0);
		string data = buf;
		if (players[ip].size != 0) {
			if (to_lower(data) == "w" && players[ip].way != 's') {
				players[ip].way = 'w';
				send(curr_sock, data.c_str(), data.size(), 0);
			}
			else if (to_lower(data) == "left" && players[ip].way != 'd') {
				players[ip].way = 'a';
				send(curr_sock, data.c_str(), data.size(), 0);
			}
			else if (to_lower(data) == "down" && players[ip].way != 'w') {
				players[ip].way = 's';
				send(curr_sock, data.c_str(), data.size(), 0);
			}
			else if (to_lower(data) == "right" && players[ip].way != 'a') {
				players[ip].way = 'd';
				send(curr_sock, data.c_str(), data.size(), 0);
			}
			else if (to_lower(data) == "stay") {
				players[ip].way = 'g';
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
			pair <int, int> place = spawn();
			if (place.first != 0) {
				players[ip].place = place;
				field[place.first][place.second] = players[ip].id;
			}
			else {
				string sorry = "there is no place";
				send(curr_sock, sorry.c_str(), sorry.size(), 0);

			}

			players[ip].size = 4;
			players[ip].way = 'g';
		}
	}
	closesocket(curr_sock);
}
void waitforcon(SOCKET listener, sockaddr_in addr, int len) {
	SOCKET curr_sock = accept(listener, (struct sockaddr*)&addr, &len);
	string ip = inet_ntoa(addr.sin_addr);
	//0.0.0.0 is listener socket ip
	if (ip != "0.0.0.0" && players[ip].nothread) {
		players[ip].nothread = false;
		thread thr(TalkToClient, ip, curr_sock);
		thr.detach();
	}
}