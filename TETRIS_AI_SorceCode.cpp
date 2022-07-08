#define _CRT_SECURE_NO_WARNINGS
#include "Time_Data_Class.h"
#include "AI_Console.h"
#include "Basic_Setting.h"
#include "AI_Mode.h"
#include "MainMenu.h"
using namespace std;

class Game : public AI_Mode {
public:
	float AI_SPEED = 0.0f;
	void start(int mode) {
		GetGameModeName(mode);
		// 메인 루프 -> 일정 시간마다 AI가 블록을 놓도록 한다.
		DrawBasicFrame(); // 게임 틀 그리기
		SetNextOrder(); // 넥스트 순서 정하기 -> 이후 자동으로 넥스트 갱신
		Game_Timer(TIME_START);
		AI_Timer(TIME_START); // AI의 블록 놓기 위한 시간 타이머 - 시작
		while (true) {
			if (AI_TimeChecking()) { // - 타이머 정지 -> 시간 계산 -> 다시 시작 or 블록 놓고 타이머 초기화
				Put_Block(); // AI가 블록을 놓는다.
				if (gameEnd) break;
				//PrintTableData(); // test
				ClearTable(); // table 지우기
				DrawTable(); // table 그리기
				ShowTestInfo();
				ShowInfo();

				if (mode == SPRINT_40L) {
					if (ClearedLinesSum >= 40) break;
				}
			}
		}
		SaveResult(mode, gameMode, Game_Time);
		if (mode == SET_PARAMETER) {
			parameterModeCount++;
			parameterModeSum += ClearedLinesSum;
			if (parameterModeCount >= 3) {
				parameterModeSum /= 3.0;
				if (ClearedLinesSum > maxAverage) {
					maxAverage = ClearedLinesSum;
					SaveParameterSetting();
					SetParameterRand();
				}
				
			}
		}
	}
private:
	string gameMode;
	float AI_Time = 0, Game_Time = 0;
	int maxAverage = 0, parameterModeCount = 0;
	double parameterModeSum = 0;
	void ShowInfo() {
		// 게임에 대한 정보를 보여주는 함수
		const int INFO_X = AI_INITIAL_X + 34;
		const int INFO_Y = AI_INITIAL_Y + 5;
		int i = 0;
		gotoxy(INFO_X, INFO_Y + i++);
		cout << "GAME MODE: " << gameMode << "          ";
		gotoxy(INFO_X, INFO_Y + i++);
		Game_Timer(TIME_STOP);
		cout << "TIME: " << Game_Time << " sec         ";
		Game_Timer(TIME_RESTART);
		gotoxy(INFO_X, INFO_Y + i++);
		cout << "Solo-Cleared Line: " << ClearedLine1 << "         ";
		gotoxy(INFO_X, INFO_Y + i++);
		cout << "Double-Cleared Line: " << ClearedLine2 << "          ";
		gotoxy(INFO_X, INFO_Y + i++);
		cout << "Triple-Cleared Line: " << ClearedLine3 << "          ";
		gotoxy(INFO_X, INFO_Y + i++);
		cout << "Tetris-Cleared Line : " << ClearedLine4 << "          ";
		gotoxy(INFO_X, INFO_Y + i++);
		cout << "TOTAL Cleared Lines: " << ClearedLinesSum << "          ";
	}
	void AI_Timer(int CONDITION) {
		clock_t static AI_Start = 0, AI_End = 0;
		float static Temp_AI_Time;
		// AI의 블록 놓는 시간에 대한 타이머
		if (CONDITION == TIME_START) {
			AI_Start = clock();
		}
		else if (CONDITION == TIME_RESTART) {
			AI_Start = clock();
		}
		else if (CONDITION == TIME_STOP) {
			AI_End = clock();
			AI_Time = ((float)(AI_End - AI_Start) / CLOCKS_PER_SEC) + Temp_AI_Time;
			Temp_AI_Time = AI_Time;
		}
		else if (CONDITION == TIME_RESET) {
			AI_Time = 0;
			Temp_AI_Time = 0;
			AI_Start = clock();
		}
	}
	void Game_Timer(int CONDITION) {
		clock_t static Game_Start = 0, Game_End = 0;
		float static Temp_Game_Time;
		// AI의 블록 놓는 시간에 대한 타이머
		if (CONDITION == TIME_START) {
			Game_Start = clock();
		}
		else if (CONDITION == TIME_RESTART) {
			Game_Start = clock();
		}
		else if (CONDITION == TIME_STOP) {
			Game_End = clock();
			Game_Time = ((float)(Game_End - Game_Start) / CLOCKS_PER_SEC) + Temp_Game_Time;
			Temp_Game_Time = Game_Time;
		}
		else if (CONDITION == TIME_RESET) {
			Game_Time = 0;
			Temp_Game_Time = 0;
			Game_Start = clock();
		}
	}
	bool AI_TimeChecking() {
		// 블록을 놓을 만큼 시간이 지났는지 검사
		AI_Timer(TIME_STOP);
		if (AI_Time >= AI_SPEED) {
			AI_Timer(TIME_RESET);
			AI_Timer(TIME_START);
			return true;
		}
		else {
			AI_Timer(TIME_RESTART);
			return false;
		}
	}
	void GetGameModeName(int mode) {
		if (mode == PRACTICE) gameMode = "PRACTICE";
		else if (mode == SPRINT_40L) gameMode = "40L Sprint";
		else if (mode == SET_PARAMETER) gameMode = "Set Parameter";
	}
	void SaveParameterSetting() {
		Time_Date td;
		ofstream file("AI_ParameterSetting.txt", ios::out | ios::app);
		if (!file.is_open()) return;
		file << "-----------------FINAL RESULT----------------\n";
		file << td.getYear() << "-" << td.getMonth() << "-" << td.getDay() << " " << td.getHour() << ":" << td.getMin() << "\n";
		file << "S_height: " << S_height << "\n";
		file << "S_makingBlank: " << S_makingBlank << "\n";
		file << "S_onTheBlank: " << S_onTheBlank << "\n";
		file << "S_lineClear: " << S_lineClear[0] << " " << S_lineClear[1] << " " << S_lineClear[2] << " " << S_lineClear[3] << "\n";
		file << "S_heightDistance: " << S_heightDistance << "\n";
		file << "---\n";
		file << "D_height: " << D_height << "\n";
		file << "D_makingBlank: " << D_makingBlank << "\n";
		file << "D_onTheBlank: " << D_onTheBlank << "\n";
		file << "D_lineClear: " << D_lineClear[0] << " " << D_lineClear[1] << " " << D_lineClear[2] << " " << D_lineClear[3] << "\n";
		file << "D_heightDistance: " << D_heightDistance << "\n";
		file << "---\n";
		file << "clearedLinesAverage: " << parameterModeSum << "\n";
		file << "maxClearedLineAverage: " << maxAverage << "\n";
		file.close();
	}
	void ClearALL() {
		for (int r = 0; r < LENGTH_Y; r++) {
			for (int c = 0; c < LENGTH_X; c++) {
				SetData(r, c, BLANK);
			}
		}
	}
	void SetParameterRand() {
		const double percent = 0.2;
		int num = rand() % 2;
		if (num) {
			S_height *= (1 + percent);
			D_height *= (1 + percent);
		}
		else {
			S_height *= (1 - percent);
			D_height *= (1 - percent);
		}
		num = rand() % 2;
		if (num) {
			S_makingBlank *= (1 + percent);
			D_makingBlank *= (1 + percent);
		}
		else {
			S_makingBlank *= (1 - percent);
			D_makingBlank *= (1 - percent);
		}
		num = rand() % 2;
		if (num) {
			S_onTheBlank *= (1 + percent);
			D_onTheBlank *= (1 + percent);
		}
		else {
			S_onTheBlank *= (1 - percent);
			D_onTheBlank *= (1 - percent);
		}
		num = rand() % 2;
		for (int i = 0; i < 4; i++) {
			if (num) {
				S_lineClear[i] *= (1 + percent);
				D_lineClear[i] *= (1 + percent);
			}
			else {
				S_lineClear[i] *= (1 - percent);
				D_lineClear[i] *= (1 - percent);
			}
		}
		num = rand() % 2;
		if (num) {
			S_heightDistance *= (1 + percent);
			D_heightDistance *= (1 + percent);
		}
		else {
			S_heightDistance *= (1 - percent);
			D_heightDistance *= (1 - percent);
		}
	}
};

int main() {
	cin.tie(NULL); cout.tie(NULL);
	ios_base::sync_with_stdio(false);
	int mode;
	MainMenu menu;
	mode = menu.start();
	system("cls");
	Game Tetris;
	Tetris.start(mode);
	gotoxy(0, 30);
	return 0;
}


/*
AI가 블록을 놓을 때, 가장 최적의 위치를 찾기 위해 여러 요소들을 고려하여 블록을 놓는다.
요소 {
	블록의 위치
	아래 빈칸의 개수
}
각 요소들에 따라 일정 비용를 정해 총 비용이 가장 낮은 위치에 블록을 놓는다.
비용을 구하는 과정은 아래와 같다.

1. 블록을 맨 왼쪽으로 놓는다.
2. 위치에 따라 비용을 계산한다.
3. 1 ~ 2 과정을 블록이 맨 오른쪽에 갈 때 까지 반복.
4. 1 ~ 3 과정을 블록의 회전에 따라 총 3번 더 반복

5. 가장 낮은 비용을 정렬을 통해 구한다.
6. 해당 위치에 블록을 놓는다.

상세 설명
기본 함수:
AI_setNextOrder() -> 넥스트 순서를 정해서 저장한다.
SetBlockShape() -> 넥스트에 따른 블럭을 2차원 배열로 가져온다.
SetNowBlockData() -> 블럭을 table_data에 넣는다. (좌표에 따라 넣는다.)
CanMove(int Direction) -> 해당 방향의 움직임이 가능한지 알아본다.

0. table_data에 블록 띄우기
블록 가져오기.
re_next_count++;

1. 블럭을 맨 왼쪽에 놓기
전제: 블록이 넥스트 순서에 따라 table_data 맨 
주요 함수: SetBlockLeftMax()




기타 사항: 
AI의 블록 놓는 속도는 1초당 블록 하나 이다. 조절 가능하다.



*/