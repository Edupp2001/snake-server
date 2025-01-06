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
#include "serverrelated.h"
#pragma comment(lib, "ws2_32.lib")
using namespace std;

vector <int> free_id(90);
int id = 0, kos = 0;



//inet_ntoa(@sockaddr_in.sin_addr@) - ip(string)

int main() {
	generate();
	srand((unsigned int)time(NULL));
	/*
	for (int i = 1, k = 0; i <= 10; ++i)
		for (int j = 0; j < 9; ++j)
			free_id[k++] = -(i + j * 10);
		*/
	thread mainthr(gamethread);
	mainthr.detach();
	pair <SOCKET, sockaddr_in> listener = createlistener();
	listen(listener.first, 1);
	
	
	int len = sizeof(listener.second);

	while (1) {
		
		waitforcon(listener.first, listener.second, len);
		
		
		kos = 0;//??
	}
	closesocket(listener.first);
}
