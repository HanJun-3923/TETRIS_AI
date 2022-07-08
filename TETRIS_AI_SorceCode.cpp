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
		// ���� ���� -> ���� �ð����� AI�� ����� ������ �Ѵ�.
		DrawBasicFrame(); // ���� Ʋ �׸���
		SetNextOrder(); // �ؽ�Ʈ ���� ���ϱ� -> ���� �ڵ����� �ؽ�Ʈ ����
		Game_Timer(TIME_START);
		AI_Timer(TIME_START); // AI�� ��� ���� ���� �ð� Ÿ�̸� - ����
		while (true) {
			if (AI_TimeChecking()) { // - Ÿ�̸� ���� -> �ð� ��� -> �ٽ� ���� or ��� ���� Ÿ�̸� �ʱ�ȭ
				Put_Block(); // AI�� ����� ���´�.
				if (gameEnd) break;
				//PrintTableData(); // test
				ClearTable(); // table �����
				DrawTable(); // table �׸���
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
		// ���ӿ� ���� ������ �����ִ� �Լ�
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
		// AI�� ��� ���� �ð��� ���� Ÿ�̸�
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
		// AI�� ��� ���� �ð��� ���� Ÿ�̸�
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
		// ����� ���� ��ŭ �ð��� �������� �˻�
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
AI�� ����� ���� ��, ���� ������ ��ġ�� ã�� ���� ���� ��ҵ��� ����Ͽ� ����� ���´�.
��� {
	����� ��ġ
	�Ʒ� ��ĭ�� ����
}
�� ��ҵ鿡 ���� ���� ��븦 ���� �� ����� ���� ���� ��ġ�� ����� ���´�.
����� ���ϴ� ������ �Ʒ��� ����.

1. ����� �� �������� ���´�.
2. ��ġ�� ���� ����� ����Ѵ�.
3. 1 ~ 2 ������ ����� �� �����ʿ� �� �� ���� �ݺ�.
4. 1 ~ 3 ������ ����� ȸ���� ���� �� 3�� �� �ݺ�

5. ���� ���� ����� ������ ���� ���Ѵ�.
6. �ش� ��ġ�� ����� ���´�.

�� ����
�⺻ �Լ�:
AI_setNextOrder() -> �ؽ�Ʈ ������ ���ؼ� �����Ѵ�.
SetBlockShape() -> �ؽ�Ʈ�� ���� ���� 2���� �迭�� �����´�.
SetNowBlockData() -> ���� table_data�� �ִ´�. (��ǥ�� ���� �ִ´�.)
CanMove(int Direction) -> �ش� ������ �������� �������� �˾ƺ���.

0. table_data�� ��� ����
��� ��������.
re_next_count++;

1. ���� �� ���ʿ� ����
����: ����� �ؽ�Ʈ ������ ���� table_data �� 
�ֿ� �Լ�: SetBlockLeftMax()




��Ÿ ����: 
AI�� ��� ���� �ӵ��� 1�ʴ� ��� �ϳ� �̴�. ���� �����ϴ�.



*/