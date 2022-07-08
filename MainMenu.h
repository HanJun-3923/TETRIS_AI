#pragma once
#include <iostream>
#include "Basic_Setting.h"
using namespace std;

enum KEYS {
	ARROW = 224,
	ARROW_UP = 72,
	ARROW_DOWN = 80,
	ESC = 27,
	ENTER = 13,
};

class MainMenu {
	// 테트리스 시작 전, 메뉴를 보여준다.
public:
	MainMenu() {
		// 기본 세팅
		setColor(15); // 밝인 흰색
		CursorView(false); // 커서 숨기기
	}
	int start() {
		showMenu();
		while (true) {
			if (_kbhit()) {
				int key = _getch();
				if (key == ARROW) key = _getch();

				if (key == ARROW_DOWN) SetPointer(DOWN);
				else if (key == ARROW_UP) SetPointer(UP);
				else if (key == ENTER) { return OpenMenu(); }

				showMenu();	
			}
		}
	}
	int SelectGameMode() {
		// 게임 모드를 선택하는 메뉴
		system("cls");
		showModeMenu();
		while (true) {
			if (_kbhit()) {
				int key = _getch();
				if (key == ARROW) key = _getch();

				if (key == ARROW_DOWN) SetModePointer(DOWN);
				else if (key == ARROW_UP) SetModePointer(UP);
				else if (key == ENTER) { return OpenModeMenu(); }

				showModeMenu();
			}
		}
	}
private:
	const int maxPointer = 1;
	int pointer = 1;
	const int maxModePointer = 1;
	int modePointer = 1;

	void showMenu() {
		system("cls");
		/*
			게임 시작
			게임 설명
			게임 조작법
			개발자
		*/
		gotoxy(0, 0);
		if (pointer == 1) cout << ">  게임 시작  <";
		else cout << "   게임 시작   ";

		/*gotoxy(0, 1);
		if (pointer == 2) cout << ">  게임 설명  <";
		else cout << "   게임 설명   ";

		gotoxy(0, 2);
		if (pointer == 3) cout << "> 게임 조작법 <";
		else cout << "  게임 조작법  ";

		gotoxy(0, 3);
		if (pointer == 4) cout << "> 게임 개발자 <";
		else cout << "  게임 개발자  ";

		gotoxy(0, 4);
		if (pointer == 5) cout << ">  게임 종료  <";
		else cout << "   게임 종료   ";*/
	}
	void SetPointer(int DIRECTION) {
		if (DIRECTION == UP) {
			if (pointer > 1) pointer--;
			else pointer = maxPointer;
		}
		else if (DIRECTION == DOWN) {
			if (pointer < maxPointer) pointer++;
			else pointer = 1; // minPointer = 1
		}
	}
	int OpenMenu() {
		// 해당 포인터에 해당하는 메뉴 열기
		if (pointer == 1) {
			// 게임 시작
			return SelectGameMode();
		}
		return SelectGameMode();
	}
	void showModeMenu() {
		system("cls");
		/*
			practice
			40L Sprint
			CheeseRace
		*/
		gotoxy(0, 0);
		if (modePointer == 1) cout << "-> Practice";
		else cout << "   Practice";

		/*gotoxy(0, 1);
		if (modePointer == 2) cout << "-> 40L Sprint";
		else cout << "   40L Sprint";

		gotoxy(0, 2);
		if (modePointer == 3) cout << "-> CheeseRace";
		else cout << "   CheeseRace";

		gotoxy(0, 3);
		if (modePointer == 4) cout << "-> Set Parameter Mode (For Developing)";
		else cout << "   Set Parameter Mode (For Developing)";*/
	}
	void SetModePointer(int DIRECTION) {
		if (DIRECTION == UP) {
			if (modePointer > 1) modePointer--;
			else modePointer = maxModePointer;
		}
		else if (DIRECTION == DOWN) {
			if (modePointer < maxModePointer) modePointer++;
			else modePointer = 1; // minPointer = 1
		}
	}
	int OpenModeMenu() {
		// 해당 포인터에 해당하는 메뉴 열기
		if (modePointer == 1) {
			// Practice
			return PRACTICE;
		}
		else if (modePointer == 2) {
			// 40L Sprint
			return SPRINT_40L;
		}
		else if (modePointer == 4) {
			return SET_PARAMETER;
		}
		return PRACTICE;
	}
};