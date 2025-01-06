#pragma once
#include <iostream>
#include <vector>
using namespace std;
vector<vector <int>> field(16 + 2);
struct player {//fix
	int size = 0, id = 0;
	char way;
	bool is_blocked;
	bool nothread = true;
	pair <int, int> place;
};
map <string, player> players;
void to_lower(char& s) {
	if (s >= 'A' && s <= 'Z')
		s += 'a' - 'A';
}
string to_lower(string S) {
	for (int i = 0; i < S.size(); ++i) {
		to_lower(S[i]);
	}
	return S;
}
string ITS(int a) {
	string tmp = "", ans = "";
	if (a < 0)
		ans += '-';
	if (a == 0)
		return "0";
	while (a) {
		tmp += abs(a % 10) + '0';
		a /= 10;
	}
	for (int i = 0; i < tmp.size(); ++i) {
		ans += tmp[tmp.size() - i - 1];
	}
	return ans;
}
int STI(string a) {
	long long b = 0;
	for (int i = 0; i < a.size(); ++i) {
		if (a[i] >= '0' && a[i] <= '9') {
			b = (a[i] - '0') + b * 10;
		}
	}
	if (a[0] == '-')
		return -b;
	else
		return b;
}
void draw() {
	//system("mode 200");//big picture mode
	HDC hDC = GetDC(GetConsoleWindow());//??????? ?????? ? ???????
	HPEN wht = CreatePen(PS_SOLID, 20, RGB(255, 255, 255));//?????? ????? "???" ???????? 5 ? ??????? ??? 255 10 10(???????)
	HPEN grn = CreatePen(PS_SOLID, 20, RGB(0, 255, 0));//(??????)
	HPEN red = CreatePen(PS_SOLID, 20, RGB(255, 0, 0));
	HPEN blu = CreatePen(PS_SOLID, 20, RGB(0, 0, 255));
	HPEN blk = CreatePen(PS_SOLID, 20, RGB(0, 0, 0));
	//to draw other object create new HPEN and new if for that object
	HBRUSH Brush = CreateSolidBrush(RGB(255, 10, 10));//?????? ????? ??????? ??? 255 10 10(???????)
	HBRUSH O = CreateSolidBrush(RGB(0, 0, 0));
	for (int i = 0; i < field.size(); ++i)
		for (int j = 0; j < field[i].size(); ++j) {
			//draw walls
			if (field[i][j] >= 1)
				SelectObject(hDC, wht);
			//draw apple
			else if (field[i][j] == -100)
				SelectObject(hDC, red);
			//draw snake's head
			else if (field[i][j] < 0)
				SelectObject(hDC, grn);
			//zakrasit' pustoe
			else
				SelectObject(hDC, blk);
			MoveToEx(hDC, 20 + 20 * i, 20 + 20 * j, NULL);
			LineTo(hDC, 20 + 20 * i, 20 + 20 * j);
		}
}
void generate() {
	for (int i = 0; i < 16 + 2; ++i)
		field[i].resize(16 + 2);
	for (int i = 0; i < field.size(); ++i)
		for (int j = 0; j < field[i].size(); ++j)
			if (i == 0 || j == 0 || i == 17 || j == 17)
				field[i][j] = 1;
			else
				field[i][j] = 0;
}
pair <int, int> spawn() {
	srand(time(NULL));
	vector <pair<int, int>> places;
	for (int i = 2; i < field.size() - 2; ++i)
		for (int j = 2; j < field[i].size() - 2; ++j)
			if (field[i + 1][j + 1] + field[i + 1][j] + field[i][j + 1] + field[i - 1][j + 1] + field[i + 1][j - 1] + field[i - 1][j - 1] + field[i - 1][j] + field[i][j - 1] + field[i][j] == 0)
				places.push_back(make_pair(i, j));
	if (places.size() != 0)
		return places[rand() % places.size()];
	else
		for (int i = 1; i < field.size() - 1; ++i)
			for (int j = 0; j < field[i].size() - 1; ++j)
				if (field[i][j] == 0)
					places.push_back(make_pair(i, j));
	if (places.size() != 0)
		return places[rand() % places.size()];
	else return make_pair(0, 0);
}
void step() {
	for (int i = 1; i < field.size() - 1; ++i)
		for (int j = 1; j < field[i].size() - 1; ++j)
			if (field[i][j] >= 1)
				--field[i][j];
			else if (field[i][j] < 0 && field[i][j] > -100) {
				map <string, player>::iterator it;
				for (it = players.begin(); it != players.end() && (*it).second.id != field[i][j]; ++it) { ; }
				if ((*it).second.way == 'w' && (*it).second.is_blocked == false) {
					if (field[i][j - 1] == 0) {
						field[i][j - 1] = (*it).second.id;
						field[i][j] = (*it).second.size;
						(*it).second.is_blocked = true;
					}
					else if (field[i][j - 1] == -100) {
						field[i][j - 1] = (*it).second.id;
						field[i][j] = (*it).second.size + 1;
						(*it).second.size++;
						(*it).second.is_blocked = true;
					}
					else if (field[i][j - 1] > 0) {
						field[i][j] = (*it).second.size;
						(*it).second.size = 0;
					}
				}
				else if ((*it).second.way == 'a' && (*it).second.is_blocked == false) {
					if (field[i - 1][j] == 0) {
						field[i - 1][j] = (*it).second.id;
						field[i][j] = (*it).second.size;
						(*it).second.is_blocked = true;
					}
					else if (field[i - 1][j] == -100) {
						field[i - 1][j] = (*it).second.id;
						field[i][j] = (*it).second.size + 1;
						(*it).second.size++;
						(*it).second.is_blocked = true;
					}
					else if (field[i - 1][j] > 0) {
						field[i][j] = (*it).second.size;
						(*it).second.size = 0;
					}
				}
				else if ((*it).second.way == 's' && (*it).second.is_blocked == false) {
					if (field[i][j + 1] == 0) {
						field[i][j + 1] = (*it).second.id;
						field[i][j] = (*it).second.size;
						(*it).second.is_blocked = true;
					}
					else if (field[i][j + 1] == -100) {
						field[i][j + 1] = (*it).second.id;
						field[i][j] = (*it).second.size + 1;
						(*it).second.size++;
						(*it).second.is_blocked = true;
					}
					else if (field[i][j + 1] > 0) {
						field[i][j] = (*it).second.size;
						(*it).second.size = 0;
					}
				}
				else if ((*it).second.way == 'd' && (*it).second.is_blocked == false) {
					if (field[i + 1][j] == 0) {
						field[i + 1][j] = (*it).second.id;
						field[i][j] = (*it).second.size;
						(*it).second.is_blocked = true;
					}
					else if (field[i + 1][j] == -100) {
						field[i + 1][j] = (*it).second.id;
						field[i][j] = (*it).second.size + 1;
						(*it).second.size++;
						(*it).second.is_blocked = true;
					}
					else if (field[i + 1][j] > 0) {
						field[i][j] = (*it).second.size;
						(*it).second.size = 0;
					}
				}
			}
	map <string, player>::iterator it;
	for (it = players.begin(); it != players.end(); ++it)
		(*it).second.is_blocked = false;
}
void keep_apples() {
	int apples = 0;
	for (int i = 1; i < field.size() - 1; ++i)
		for (int j = 1; j < field[i].size() - 1; ++j)
			if (field[i][j] == -100)
				apples++;
	while (apples < 2) {
		pair <int, int> apple = spawn();
		if (apple.first != 0) {
			field[apple.first][apple.second] = -100;
			apples++;
		}
		else {
			;//€ задолбалс€, давайте мы не будем играть, до тех пор, когда место дл€ €блок не кончитс€
			;//чисто в теории можно найти самую длинную змею и объ€вить ее победителем
		}
	}
}
void gamethread() {
	while (1) {
		keep_apples();
		draw();
		step();
		Sleep(300);//should be fixed
	}
}