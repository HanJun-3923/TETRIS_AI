#pragma once
#include "Basic_Setting.h"
#include "AI_Block.h"
#include "Time_Data_Class.h"
#include "AI_Console.h"

class AI_Mode : public AI_Block {
	// AI�� ����� ���� �˰����� �ٷ��.
public:
	int secondLowestHeight = 0;
	bool gameEnd = false;
	void Put_Block() { // AI�� ����� ����Ͽ� ����� �ϳ� ���´�.
		SetNewBlock(); // ����� �ҷ��´�.
		if (isGameOver()) {
			gameEnd = true;
			return;
		}
		SetNowBlockData(); // �ҷ��� ����� �����͸� tableData�� �ִ´�.
		vector<cost_type> cost; // ���� ����� �� ����
		/*
		* ����
		��� �� �������� ������
		��� �� �Ʒ��� ������
		��� �����ϱ�
		���� ��ǥ �����ϱ�
		�ٽ� ������ �̵��� ��, ���������� �� ĭ�� �̵��ϸ鼭 �ݺ�
		*/
		secondLowestHeight = getSecondLowestHeight();
		rotCycle = SetRotCycle(); // ��Ͽ� ���� rotCycle �� ����
		CalNowBlockCost(cost); // ���� ��� ��� ����
		CalHoldBlockCost(cost); // Ȧ�� ��� ��� ����
		sort(cost.begin(), cost.end(), SortCost); // ��� �������� ����
		double selectedCost = cost[0].cost_Caculate; // ���� ��� ����
		int selectedRot = cost[0].cost_rot; // ������ ��ġ�� rot ��
		int selectedX = cost[0].cost_x; // ������ ��ġ�� X��
		int selectedY = cost[0].cost_y; // ������ ��ġ�� X��
		bool selectedHold = cost[0].cost_hold; // Ȧ�� ����
		gotoxy(0, 0); cout << "cost: " << selectedCost << " rot: " << selectedRot << " x: " << selectedX << " y: " << selectedY << "                    ";
		// test - ( ��� ��ǥ ȸ������ ) ���

		// ��ǥ �� ȸ�� ���¸� ������ ������ ����
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

		BlockSolid(); // ��� ���� (Solid)
		LineClear(); // ����Ŭ���� �˻� �� ����Ŭ����

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
			BlockMoveLeftMax(); // ��� �� �������� ������
			for (;;) { // break: ���������� ������ �� ���� ��
				// ���������� �����鼭 �� ��ġ�� ����� �����Ѵ�.
				// �̶� ���� cost�� ���� ������ ������ { ���, rot, x, y } �̴�.
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

				y = 0; // ��ǥ �� ���� -> �ٷ� �����ʿ��� ����� ���� �� �ֱ� ����
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
			BlockMoveLeftMax(); // ��� �� �������� ������
			for (;;) { // break: ���������� ������ �� ���� ��
				// ���������� �����鼭 �� ��ġ�� ����� �����Ѵ�.
				// �̶� ���� cost�� ���� ������ ������ { ���, rot, x, y } �̴�.
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

				y = 0; // ��ǥ �� ���� -> �ٷ� �����ʿ��� ����� ���� �� �ֱ� ����
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
			1. ��� ������ ��.
			2. �ϼ��� ���� ����
			3. ��� ���̿� �ִ� �� ������ ����.
			4. �� ������ ���� �ִ� ����� ����.
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
		// �ش� ��ġ�� ����� ������ ���� ���
		double sum = 0;
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				int data = nowBlock[rot][r][c];
				if (BLOCK1 <= data && data <= BLOCK7) {
					sum += W_height * pow(double(23) - double(r) - double(y), 1.5);
					// 23 -> �� �Ʒ� ��ġ�� �� ��� 0
				}
			}
		}
		return sum;
	}
	double getMakingBlankCost() {
		// �ش� ��ġ�� ����� ������ �� ���ۿ� ���� ���
		// ��� �Ʒ��� ������ �� �� �� �ִ��� �˻��Ѵ�.
		int count = 0; // ������ ����
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				int data = nowBlock[rot][r][c];
				if (BLOCK1 <= data && data <= BLOCK7) {
					for (int i = 0; i < 4; i++) { // ������ �� ������ Ȯ��
						int data1 = GetData(x + c, y + r + 1 + i); // ���� ��� (i + 1)��° �Ʒ� ������
						if (data1 == BLANK) count++;
						else break;
					}
				}
			}
		}

		return double(count) * W_makingBlank;
	}
	double getOnTheBlankCost() {
		// �ش� ����� ���� ���� ���� �ִٸ� ��� �߰�
		// ����: Ÿ��ó�� ���̴� ������ ���� ���� 3��� �Ʒ� ������ �˻���
		int count = 0;
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				int data = nowBlock[rot][r][c];
				if (BLOCK1 <= data && data <= BLOCK7) {
					for (int i = 1; i < 5; i++) {
						if (y + r + 1 + i >= 24) {
							break; // ���� �����ߴٸ�
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
		//�� ���� ������ ���������� Ȯ���� �� �׿� ���� ����� ���� �Ǵ� �߰��Ѵ�.
		// 1���� Ŭ���� -> 30
		// 2���� Ŭ���� -> 20
		// 3���� Ŭ���� -> 10
		// 4���� Ŭ���� -> -100
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
		// �ι�°�� ���� ��ġ�� ����� ���� ���̿� ���� ��� ���
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
		// �ʵ忡�� �ι�°�� ���� ���� �κ��� �ǹ��Ѵ�.
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
		// �׽�Ʈ �� ������ ���� �ӽ������� ���� �����ִ� �Լ�
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