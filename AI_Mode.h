#pragma once
#include "Basic_Setting.h"
#include "AI_Block.h"
#include "Time_Data_Class.h"
#include "AI_Console.h"

class AI_Mode : public AI_Block {
	// AI가 블록을 놓는 알고리즘을 다룬다.
public:
	int secondLowestHeight = 0;
	bool gameEnd = false;
	void Put_Block() { // AI가 비용을 고려하여 블록을 하나 놓는다.
		SetNewBlock(); // 블록을 불러온다.
		if (isGameOver()) {
			gameEnd = true;
			return;
		}
		SetNowBlockData(); // 불러온 블록의 데이터를 tableData에 넣는다.
		vector<cost_type> cost; // 계산된 비용이 들어갈 벡터
		/*
		* 순서
		블록 맨 왼쪽으로 보내기
		블록 맨 아래로 보내기
		비용 측정하기
		비용과 좌표 저장하기
		다시 맨위로 이동한 뒤, 오른쪽으로 한 칸씩 이동하면서 반복
		*/
		secondLowestHeight = getSecondLowestHeight();
		rotCycle = SetRotCycle(); // 블록에 따른 rotCycle 값 조정
		CalNowBlockCost(cost); // 현재 블록 비용 저장
		CalHoldBlockCost(cost); // 홀드 블록 비용 저장
		sort(cost.begin(), cost.end(), SortCost); // 비용 오름차순 정렬
		double selectedCost = cost[0].cost_Caculate; // 최저 비용 설정
		int selectedRot = cost[0].cost_rot; // 선정된 위치의 rot 값
		int selectedX = cost[0].cost_x; // 선정된 위치의 X값
		int selectedY = cost[0].cost_y; // 선정된 위치의 X값
		bool selectedHold = cost[0].cost_hold; // 홀드 여부
		gotoxy(0, 0); cout << "cost: " << selectedCost << " rot: " << selectedRot << " x: " << selectedX << " y: " << selectedY << "                    ";
		// test - ( 비용 좌표 회전상태 ) 출력

		// 좌표 및 회전 상태를 선정된 값으로 설정
		if (selectedHold) Hold();
		x = selectedX;
		y = selectedY;
		rot = selectedRot;

		gotoxy(0, 1); cout << "hold: " << selectedHold << "  " << holdBlockNumber << " " << nowBlockNumber << "   ";
		gotoxy(0, 2); for (int i = 0; i < 14; i++) cout << nextOrder[i] << " ";
		gotoxy(0, 3); cout << "heightCost: " << cost[0].cost_height << "     ";
		gotoxy(0, 4); cout << "makingBlankCost: " << cost[0].cost_makingBlank << "     ";
		gotoxy(0, 5); cout << "onTheBlankCost: " << cost[0].cost_onTheBlank << "     ";
		gotoxy(0, 6); cout << "lineClearCost: " << cost[0].cost_lineClear << "     ";
		gotoxy(0, 7); cout << "heightDistance: " << cost[0].cost_heightDistance << "     ";
		gotoxy(0, 8); cout << "position: " << cost[0].cost_position << "     ";

		BlockSolid(); // 블록 놓기 (Solid)
		LineClear(); // 라인클리어 검사 및 라인클리어

	}
	void SaveResult(int mode, string modeName, double time) {
		Time_Date td;
		ofstream file("AI_Test_Result.txt", ios::out | ios::app);
		if (!file.is_open()) return;
		file << "-----------------FINAL RESULT----------------\n";
		file << td.getYear() << "-" << td.getMonth() << "-" << td.getDay() << " " << td.getHour() << ":" << td.getMin() << "\n";
		file << "Solo-Cleared Line: " << ClearedLine1 << "\n";
		file << "Double-Cleared Line: " << ClearedLine2 << "\n";
		file << "Triple-Cleared Line: " << ClearedLine3 << "\n";
		file << "Tetris-Cleared Line : " << ClearedLine4 << "\n";
		file << "TOTAL Cleared Lines: " << ClearedLinesSum << "\n";
		file << "GAME MODE: " << modeName << "\n";
		if (mode == SPRINT_40L) {
			file << "40L Sprint Record: " << time << "\n";
		}
		file << "---------------------------------------------\n";
		file.close();
	}
	void CalNowBlockCost(vector<cost_type>& cost) {
		for (rot = 0; rot < rotCycle; rot++) {
			BlockMoveLeftMax(); // 블록 맨 좌측으로 보내기
			for (;;) { // break: 오른쪽으로 움직일 수 없을 때
				// 오른쪽으로 보내면서 각 위치의 비용을 측정한다.
				// 이때 벡터 cost에 들어가는 데이터 구조는 { 비용, rot, x, y } 이다.
				BlockMoveDownMax();
				SetNowBlockData();

				cost_type tempCost;
				if (CalculateCost(tempCost)) {
					tempCost.cost_hold = false;
					tempCost.cost_rot = rot;
					tempCost.cost_x = x;
					tempCost.cost_y = y;
					cost.push_back(tempCost);
				}

				y = 0; // 좌표 맨 위로 -> 바로 오른쪽에는 블록이 있을 수 있기 때문
				ClearNowBlockData();
				if (!CanMove(RIGHT)) break;
				else BlockMove(RIGHT);
			}
		}
	}
	void CalHoldBlockCost(vector<cost_type>& cost) {
		int temp = nowBlockNumber;
		if (hold) SetBlock(holdBlockNumber);
		else SetBlock(nextOrder[reNextCount]);
		SetPosInitial();
		for (rot = 0; rot < rotCycle; rot++) {
			BlockMoveLeftMax(); // 블록 맨 좌측으로 보내기
			for (;;) { // break: 오른쪽으로 움직일 수 없을 때
				// 오른쪽으로 보내면서 각 위치의 비용을 측정한다.
				// 이때 벡터 cost에 들어가는 데이터 구조는 { 비용, rot, x, y } 이다.
				BlockMoveDownMax();
				SetNowBlockData();

				cost_type tempCost;
				if (CalculateCost(tempCost)) {
					tempCost.cost_hold = true;
					tempCost.cost_rot = rot;
					tempCost.cost_x = x;
					tempCost.cost_y = y;
					cost.push_back(tempCost);
				}

				y = 0; // 좌표 맨 위로 -> 바로 오른쪽에는 블록이 있을 수 있기 때문
				ClearNowBlockData();
				if (!CanMove(RIGHT)) break;
				else BlockMove(RIGHT);

			}
		}
		SetBlock(temp);
	}
public: //private
	int rotCycle = 4;
	string parameterMode;
	double W_height = 0, W_makingBlank = 0, W_onTheBlank = 0, W_lineClear[4] = {}, W_heightDistance = 0;
	double W_Position[10] = { -100, 0, 0, 0, 0, 0, 0, 0, -100 };
	
	// Staking Weights
	double S_height = 100, S_makingBlank = 5000, S_onTheBlank = 100, S_lineClear[4] = {2000, 1500, -1000, -10000}, S_heightDistance = 20;
	
	// DownStaking Weights
	double D_height = 300, D_makingBlank = 4000, D_onTheBlank = 0, D_lineClear[4] = { 1000, -2000, -3000, -10000 }, D_heightDistance = 3000;
	
	bool CalculateCost(cost_type& temp) {
		/*
			1. 블록 높이의 합.
			2. 완성된 줄의 개수
			3. 블록 사이에 있는 빈 공간의 개수.
			4. 빈 공간을 막고 있는 블록의 개수.
		*/
		if (isExeption()) return false;
		SetParameter();
		double costSum = 0;
		double heightCost = getHeightCost();
		double makingBlankCost = getMakingBlankCost();
		double onTheBlankCost = getOnTheBlankCost();
		double lineClearCost = getLineClearCost();
		double heightDistanceCost = getHeightDistanceCost(secondLowestHeight);
		double positionCost = getPositionCost();
		temp.cost_height = heightCost;
		temp.cost_makingBlank = makingBlankCost;
		temp.cost_onTheBlank = onTheBlankCost;
		temp.cost_lineClear = lineClearCost;
		temp.cost_heightDistance = heightDistanceCost;
		temp.cost_position = positionCost;
		temp.cost_Caculate = heightCost + makingBlankCost + onTheBlankCost + lineClearCost + heightDistanceCost + positionCost;
		return true;
	}
	void SetParameter() {
		if (y < 11) {
			// DownStaking Mode
			parameterMode = "DownStacking";
			W_height = D_height;
			W_heightDistance = D_heightDistance;
			W_makingBlank = D_makingBlank;
			W_onTheBlank = D_onTheBlank;
			W_lineClear[0] = D_lineClear[0];
			W_lineClear[1] = D_lineClear[1];
			W_lineClear[2] = D_lineClear[2];
			W_lineClear[3] = D_lineClear[3];
		}
		else if (y > 19) {
			// Staking Mode
			parameterMode = "Stacking";
			W_height = S_height;
			W_heightDistance = S_heightDistance;
			W_makingBlank = S_makingBlank;
			W_onTheBlank = S_onTheBlank;
			W_lineClear[0] = S_lineClear[0];
			W_lineClear[1] = S_lineClear[1];
			W_lineClear[2] = S_lineClear[2];
			W_lineClear[3] = S_lineClear[3];
		}
	}
	double getHeightCost() {
		// 해당 위치의 블록이 가지는 높이 비용
		double sum = 0;
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				int data = nowBlock[rot][r][c];
				if (BLOCK1 <= data && data <= BLOCK7) {
					sum += W_height * pow(double(23) - double(r) - double(y), 1.5);
					// 23 -> 맨 아래 위치할 때 비용 0
				}
			}
		}
		return sum;
	}
	double getMakingBlankCost() {
		// 해당 위치의 블록이 가지는 빈 구멍에 대한 비용
		// 블록 아래에 공백이 총 몇 개 있는지 검사한다.
		int count = 0; // 공백의 개수
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				int data = nowBlock[rot][r][c];
				if (BLOCK1 <= data && data <= BLOCK7) {
					for (int i = 0; i < 4; i++) { // 공백이 몇 개인지 확인
						int data1 = GetData(x + c, y + r + 1 + i); // 현재 블록 (i + 1)번째 아래 데이터
						if (data1 == BLANK) count++;
						else break;
					}
				}
			}
		}

		return double(count) * W_makingBlank;
	}
	double getOnTheBlankCost() {
		// 해당 블록이 막힌 구멍 위에 있다면 비용 추가
		// 수정: 타워처럼 쌓이는 현상을 막기 위해 3블록 아래 까지만 검사함
		int count = 0;
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				int data = nowBlock[rot][r][c];
				if (BLOCK1 <= data && data <= BLOCK7) {
					for (int i = 1; i < 5; i++) {
						if (y + r + 1 + i >= 24) {
							break; // 벽에 도달했다면
						}
						int data1 = GetData(x + c, y + r + 1 + i);
						if (data1 == BLANK) {
							count += i + 1;
							break;
						}
						else continue;
					}
				}
			}
		}
		return double(count) * W_onTheBlank;
	}
	double getLineClearCost() {
		//몇 개의 라인이 지워지는지 확인한 후 그에 따라 비용을 차감 또는 추가한다.
		// 1라인 클리어 -> 30
		// 2라인 클리어 -> 20
		// 3라인 클리어 -> 10
		// 4라인 클리어 -> -100
		int count = 0;
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				int data = nowBlock[rot][r][c];
				if (BLOCK1 <= data && data <= BLOCK7) {
					if (CanLineClear(r + y)) {
						count++; break;
					}
				}
			}
		}
		if (count > 0) return W_lineClear[count - 1];
		else return 0;
	}
	double getHeightDistanceCost(int low) {
		// 두번째로 낮은 위치와 블록의 높이 차이에 대한 비용 계산
		bool done = false;
		int distance;
		for (int r = 3; r >= 0; r--) {
			for (int c = 0; c < 4; c++) {
				int data = nowBlock[rot][r][c];
				if (BLOCK1 <= data && data <= BLOCK7) {
					distance = low - (r + y);
					done = true;
					break;
				}
			}
			if (done) break;
		}
		return distance * W_heightDistance;
	}
	int getSecondLowestHeight() {
		// 필드에서 두번째로 가장 낮은 부분을 의미한다.
		int firstHeight = -2;
		int secondHeight = -1;
		for (int c = 1; c <= 10; c++) {
			int height = 0;
			for (int r = 0; r < 24; r++) {
				int data = GetData(c, r);
				if (BLOCK1 <= data && data <= BLOCK7) {
					height = 24 - r;
					break;
				}
			}
			if (height <= firstHeight && height <= secondHeight) { secondHeight = firstHeight; firstHeight = height; }
			else if (height >= firstHeight && height <= secondHeight) { secondHeight = height; }
		}
		return secondHeight;
	}
	double getPositionCost() {
		double cost = 0;
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				int data = nowBlock[rot][r][c];
				if (BLOCK1 <= data && data <= BLOCK7) {
					cost += W_Position[c + x - 1];
				}
			}
		}
		return cost;
	}
	bool isExeption() {
		if (nowBlockNumber == 7 && (rot == 1 || rot == 3)) return false;
		else return false;
	}
	int SetRotCycle() {
		if (nowBlockNumber == 1) return 1;
		if (nowBlockNumber == 2 && nowBlockNumber == 3 && nowBlockNumber == 4) return 2;
		else return 4;
	}

	// test
	void ShowTestInfo() {
		// 테스트 및 개발을 위해 임시적으로 값을 보여주는 함수
		const int INFO_X = 0;
		const int INFO_Y = 17;
		int i = 0;
		gotoxy(INFO_X, INFO_Y + i++);
		cout << "AI-MODE: " << parameterMode << "         ";
		gotoxy(INFO_X, INFO_Y + i++);
		cout << "W_height: " << W_height << "         ";
		gotoxy(INFO_X, INFO_Y + i++);
		cout << "W_makingBlank: " << W_makingBlank << "         ";
		gotoxy(INFO_X, INFO_Y + i++);
		cout << "W_onTheBlank: " << W_onTheBlank << "         ";
		gotoxy(INFO_X, INFO_Y + i++);
		cout << "W_lineClear: " << W_lineClear[0] << " " << W_lineClear[1] << " " << W_lineClear[2] << " " << W_lineClear[3] << "         ";
		gotoxy(INFO_X, INFO_Y + i++);
		cout << "W_heightDistance: " << W_heightDistance << "         ";

	}
};