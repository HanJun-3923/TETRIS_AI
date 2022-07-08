#pragma once
// 필요한 헤더파일 및 열거형, 전역 변수, 전역 함수 등을 다룬다.
#include <iostream>
#include <algorithm>
#include <conio.h>
#include <cmath>
#include <vector>
#include <ctime>
#include <Windows.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

enum AI_CONSTANT {
	NONE = -1,
	THE_NUM_OF_BLOCKS = 7
};
enum TIME {
	TIME_START,
	TIME_RESTART,
	TIME_STOP,
	TIME_RESET,
	TIME_GET
};
enum COORDINATE {
	LENGTH_X = 12,
	LENGTH_Y = 25,
	INITIAL_X = 20,
	INITIAL_Y = 4,
	AI_INITIAL_X = 60,
	AI_INITIAL_Y = 4,

	LENGTH_NEXT_X = 12,
	//LEN_NEXT_Y = LENGTH_Y

	HOLD_INITIAL_X = 10,
	HOLD_INITIAL_Y = 5,

	INFO_INITIAL_X = 46,
	INFO_INITIAL_Y = 21,

	GAMEOVER_INITIAL_X = 70,
	GAMEOVER_INITIAL_Y = 10
};
enum TABLE_NUMBER {
	BLANK = 0,
	BLOCK1 = 1,
	BLOCK2 = 2,
	BLOCK3 = 3,
	BLOCK4 = 4,
	BLOCK5 = 5,
	BLOCK6 = 6,
	BLOCK7 = 7,
	SOLID_BLOCK1 = 11,
	SOLID_BLOCK2 = 12,
	SOLID_BLOCK3 = 13,
	SOLID_BLOCK4 = 14,
	SOLID_BLOCK5 = 15,
	SOLID_BLOCK6 = 16,
	SOLID_BLOCK7 = 17,

	WALL = 10,
	GHOST_BLOCK = -1,


};
enum DIRECTION {
	LEFT,
	RIGHT,
	UP,
	DOWN
};
enum GAMEMODE {
	PRACTICE = 1,
	SPRINT_40L,
	SET_PARAMETER = 4
};
struct cost_type {
	// cost 벡터의 타입
	double cost_Caculate;
	int cost_rot;
	int cost_x;
	int cost_y;
	bool cost_hold;

	double cost_height;
	double cost_makingBlank;
	double cost_onTheBlank;
	double cost_lineClear;
	double cost_heightDistance;
	double cost_position;

};

const int block1[4][4][4] = {
		{
						{0, 0, 0, 0},
						{0, 1, 1, 0},
						{0, 1, 1, 0},
						{0, 0, 0, 0}
		},
		{
						{0, 0, 0, 0},
						{0, 1, 1, 0},
						{0, 1, 1, 0},
						{0, 0, 0, 0}
		},
		{
						{0, 0, 0, 0},
						{0, 1, 1, 0},
						{0, 1, 1, 0},
						{0, 0, 0, 0}
		},
		{
						{0, 0, 0, 0},
						{0, 1, 1, 0},
						{0, 1, 1, 0},
						{0, 0, 0, 0}
		},

};
const int block2[4][4][4] = {
		{
						{0, 0, 0, 0},
						{2, 2, 2, 2},
						{0, 0, 0, 0},
						{0, 0, 0, 0}
		},
		{
						{0, 0, 2, 0},
						{0, 0, 2, 0},
						{0, 0, 2, 0},
						{0, 0, 2, 0}
		},
		{
						{0, 0, 0, 0},
						{0, 0, 0, 0},
						{2, 2, 2, 2},
						{0, 0, 0, 0}

		},
		{
						{0, 2, 0, 0},
						{0, 2, 0, 0},
						{0, 2, 0, 0},
						{0, 2, 0, 0}

		},

};
const int block3[4][4][4] = {
		{
						{0, 0, 0, 0},
						{0, 3, 3, 0},
						{3, 3, 0, 0},
						{0, 0, 0, 0}

		},
		{
						{0, 0, 0, 0},
						{0, 3, 0, 0},
						{0, 3, 3, 0},
						{0, 0, 3, 0}
		},
		{
						{0, 0, 0, 0},
						{0, 0, 0, 0},
						{0, 3, 3, 0},
						{3, 3, 0, 0}

		},
		{
						{0, 0, 0, 0},
						{3, 0, 0, 0},
						{3, 3, 0, 0},
						{0, 3, 0, 0}
		},

};
const int block4[4][4][4] = {
		{
						{0, 0, 0, 0},
						{4, 4, 0, 0},
						{0, 4, 4, 0},
						{0, 0, 0, 0}

		},
		{
						{0, 0, 0, 0},
						{0, 0, 4, 0},
						{0, 4, 4, 0},
						{0, 4, 0, 0}
		},
		{
						{0, 0, 0, 0},
						{0, 0, 0, 0},
						{4, 4, 0, 0},
						{0, 4, 4, 0}

		},
		{
						{0, 0, 0, 0},
						{0, 4, 0, 0},
						{4, 4, 0, 0},
						{4, 0, 0, 0}
		},

};
const int block5[4][4][4] = {
		{
						{0, 0, 0, 0},
						{0, 0, 5, 0},
						{5, 5, 5, 0},
						{0, 0, 0, 0}

		},
		{
						{0, 0, 0, 0},
						{0, 5, 0, 0},
						{0, 5, 0, 0},
						{0, 5, 5, 0}
		},
		{
						{0, 0, 0, 0},
						{0, 0, 0, 0},
						{5, 5, 5, 0},
						{5, 0, 0, 0}

		},
		{
						{0, 0, 0, 0},
						{5, 5, 0, 0},
						{0, 5, 0, 0},
						{0, 5, 0, 0}

		},

};
const int block6[4][4][4] = {
		{
						{0, 0, 0, 0},
						{6, 0, 0, 0},
						{6, 6, 6, 0},
						{0, 0, 0, 0}
		},
		{
						{0, 0, 0, 0},
						{0, 6, 6, 0},
						{0, 6, 0, 0},
						{0, 6, 0, 0}

		},
		{
						{0, 0, 0, 0},
						{0, 0, 0, 0},
						{6, 6, 6, 0},
						{0, 0, 6, 0}

		},
		{
						{0, 0, 0, 0},
						{0, 6, 0, 0},
						{0, 6, 0, 0},
						{6, 6, 0, 0}

		},

};
const int block7[4][4][4] = {
		{
						{0, 0, 0, 0},
						{8, 7, 8, 0},
						{7, 7, 7, 0},
						{9, 0, 9, 0}
		},
		{
						{0, 0, 0, 0},
						{9, 7, 8, 0},
						{0, 7, 7, 0},
						{9, 7, 8, 0}

		},
		{
						{0, 0, 0, 0},
						{9, 0, 9, 0},
						{7, 7, 7, 0},
						{8, 7, 8, 0}

		},
		{
						{0, 0, 0, 0},
						{8, 7, 9, 0},
						{7, 7, 0, 0},
						{8, 7, 9, 0}

		},

};

void Draw(short x, short y, string str) { // Windows.h
	COORD pos{ 2 * x + AI_INITIAL_X , y + AI_INITIAL_Y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	cout << str;
}
void gotoxy(short x, short y) { // Windows.h
	COORD pos{ x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void CursorView(char show) {
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
void setColor(int num)
{
	int color;
	if (num == 0) color = 15;
	else if (num == BLOCK1) color = 14;
	else if (num == BLOCK2) color = 11;
	else if (num == BLOCK3) color = 10;
	else if (num == BLOCK4) color = 4;
	else if (num == BLOCK5) color = 12;
	else if (num == BLOCK6) color = 1;
	else if (num == BLOCK7) color = 13;
	else color = num;
	/*
	14 -> O미노
	11 -> I미노
	10 -> S미노
	4 -> Z미노
	12 -> L미노
	1 -> J 미노
	13 -> T미노
	*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
bool SortCost(cost_type& a, cost_type& b) {
	return a.cost_Caculate < b.cost_Caculate;
}