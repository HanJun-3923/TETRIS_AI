#pragma once
#include "Basic_Setting.h"
#include "AI_Console.h"

class AI_Block : public AI_Console {
	// AI�� ����� ���� �������� �ʿ��� �Լ����� �ٷ��.
public:
	int x = 0, y = 0, rot = 0; // ���� ��ǥ, ȸ������
	int nextOrder[2 * THE_NUM_OF_BLOCKS] = {}; // 2������ �ؽ�Ʈ(7�� x 2)
	int reNextCount = 0; // �ؽ�Ʈ ���� ���� -> 7�� �Ǹ� �ؽ�Ʈ Refresh
	int nowBlock[4][4][4] = {}; // ���� ����� �����͸� ������ �ִ�. 
	int nowBlockNumber = -1;
	int holdBlock[4][4] = {}; // Ȧ�� ����� �����͸� ������ �ִ�.
	bool hold = false; // ���� Ȧ�� ������ ����
	int holdBlockNumber = -1;
	int ClearedLinesSum = 0; // ������ ������ �����̴�.
	int ClearedLine1 = 0;
	int ClearedLine2 = 0;
	int ClearedLine3 = 0;
	int ClearedLine4 = 0;


	bool CanMove(int DIRECTION) {
		// �ش� �������� �̵��� �������� ���θ� �Ǵ��Ѵ�.
		int movedData = 0;
		int data;
		if (DIRECTION == DOWN) {
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					data = nowBlock[rot][r][c];
					if (BLOCK1 <= data && data <= BLOCK7) {
						movedData = GetData(c + x, r + y + 1);
						if (movedData == WALL || (SOLID_BLOCK1 <= movedData && movedData <= SOLID_BLOCK7)) return false;
					}
				}
			}
		}
		else {
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					data = nowBlock[rot][r][c];
					if (BLOCK1 <= data && data <= BLOCK7) {
						if (DIRECTION == LEFT) movedData = GetData(c + x - 1, r + y);
						else if (DIRECTION == RIGHT) movedData = GetData(c + x + 1, r + y);

						if (movedData == WALL || (SOLID_BLOCK1 <= movedData && movedData <= SOLID_BLOCK7)) return false;
					}
				}
			}
		}
		return true;
	}
	// �ش� �������� ����� �̵���Ų��. 
	void BlockMove(int DIRECTION) {
		// ����� �̵���Ų��.
		// ���� ����� �����͸� �����. -> ��ǥ�� �̵��Ѵ�. -> ��ǥ�� ���� ��� �����͸� �ٽ� �ִ´�.
		ClearNowBlockData();
		if (DIRECTION == LEFT) x--;
		else if (DIRECTION == DOWN) y++;
		else if (DIRECTION == RIGHT) x++;
		//SetNowBlockData();
	}
	void BlockMoveLeftMax() {
		// ����� �� �������� �̵���Ų��.
		for (; CanMove(LEFT);) { // �� �������� ������
			BlockMove(LEFT);
		}
	}
	void BlockMoveRightMax() {
		// ����� �� ���������� �̵���Ų��.
		for (; CanMove(RIGHT);) { // �� �������� ������
			BlockMove(RIGHT);
		}
	}
	void BlockMoveDownMax() {
		// ����� �� �Ʒ������� �̵���Ų��.
		for (; CanMove(DOWN);) {
			BlockMove(DOWN);
		}
	}
	void SetPosInitial() {
		// ����� ��ǥ�� �ʱ� ��ǥ�� �̵���Ų��.
			// ��ġ�� �ʱ� ��ġ�� ����.
			// 4, 0 �� �ʱ�� ���Ѵ�.
		x = 4; y = 0;
	}
	void SetNextOrder() {
		// �ؽ�Ʈ ��ϵ��� ������ �����Ѵ�.
			// �ʱ� �ؽ�Ʈ 2���� (14��)�� 7-bag�� ���� ������ش�.
		bool same;
		for (int i = 0; i < THE_NUM_OF_BLOCKS; i++) {
			while (true) {
				nextOrder[i] = rand() % 7 + 1;
				same = false;
				for (int j = 0; j < THE_NUM_OF_BLOCKS; j++) {
					if (i != j && nextOrder[i] == nextOrder[j]) same = true;
				}
				if (!same) break;
			}
		}
		for (int i = 7; i < 7 + THE_NUM_OF_BLOCKS; i++) {
			while (true) {
				nextOrder[i] = rand() % 7 + 1;
				same = false;
				for (int j = 7; j < 7 + THE_NUM_OF_BLOCKS; j++) {
					if (i != j && nextOrder[i] == nextOrder[j]) same = true;
				}
				if (!same) break;
			}
		}
	}
	void NextRefresh() {
		// 2��° ������ �ؽ�Ʈ�� 1��° �������� �ű� ��, 2��° ������ �����Ѵ�.
			// �� ���� 7���� ����� ��� ������� ��, 2��° ������ �ؽ�Ʈ�� 1��° �������� �Ű��� ��, 2��° ������ �ؽ�Ʈ�� ����
		bool same;
		for (int i = 0; i < THE_NUM_OF_BLOCKS; i++) {
			nextOrder[i] = nextOrder[i + 7];
			nextOrder[i + 7] = 0;
		}
		for (int i = 7; i < 7 + THE_NUM_OF_BLOCKS; i++) {
			while (true) {
				nextOrder[i] = rand() % 7 + 1;
				same = false;
				for (int j = 7; j < 7 + THE_NUM_OF_BLOCKS; j++) {
					if (i != j && nextOrder[i] == nextOrder[j]) same = true;
				}
				if (!same) break;
			}
		}
	}
	void SetNewBlock() {
		// �ؽ�Ʈ�� ���� ���ο� ����� �����´�.
			// �ؽ�Ʈ�� ���� ����� ������ nowBlock �� �ִ´�.
			// ��ǥ �ʱ�ȭ, reNextCount ���� �� Refresh �۾��� ���� �Ѵ�.
		SetBlock(nextOrder[reNextCount]);
		SetPosInitial();
		reNextCount++;
		if (reNextCount >= 7) {
			reNextCount = 0;
			NextRefresh();
		}
	}
	void SetNowBlockData() {
		// ���� ����� �����͵� tableData�� �ִ´�.
			// NowBlock ������ -> tableData
		int data;
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				data = nowBlock[rot][r][c];
				if (BLOCK1 <= data && data <= BLOCK7) SetData(x + c, y + r, data);
			}
		}
	}
	bool isGameOver() {
		int data;
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				data = nowBlock[rot][r][c];
				if (BLOCK1 <= data && data <= BLOCK7) {
					int getData = GetData(x + c, y + r);
					if (SOLID_BLOCK1 <= getData && getData <= SOLID_BLOCK7) return true;
				}
			}
		}
		return false;
	}
	void BlockSolid() {
		// ���� ����� Solid �Ѵ�.
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				int data = nowBlock[rot][r][c];
				if (BLOCK1 <= data && data <= BLOCK7) {
					SetData(c + x, r + y, data + 10); // ��Ͽ��� 10�� ������ ���� �ָ��� ����̴�. (�ϵ��ڵ�)
				}
			}
		}
	}
	bool CanLineClear(int r) {
		// �ش� ������ ���� Ŭ��� �������� �˻��Ѵ�.
		int count = 0;
		for (int c = 1; c < LENGTH_X - 1; c++) {
			int data = GetData(c, r);
			if (BLOCK1 <= data && data <= BLOCK7) count++;
			else if (SOLID_BLOCK1 <= data && data <= SOLID_BLOCK7) count++;
		}
		if (count >= LENGTH_X - 2) return true; // LENGTH_X - 2 = 10
		else return false;
	}
	void LineClear() {
		int cl = 0;
		// ��� ���� ������ �˻��Ͽ� ����Ŭ��� �����ϴٸ� �Ѵ�.
		// ����Ŭ��� �Ǹ� ������ ������ ������Ų��.
		for (int r = 1; r < LENGTH_Y - 1; r++) {
			if (CanLineClear(r)) { // ����Ŭ���� ���� �˻�
				cl++;  // ������ ���� ī��Ʈ

				for (int c = 1; c < LENGTH_X - 1; c++) SetData(c, r, BLANK); // �ش� �� �����
				for (int i = r; i > 0; i--) { // �ش� ������ �� ������ �ٷ� �� ���� �� �����Ͽ� ��������.
					for (int x = 1; x < LENGTH_X - 1; x++) {
						SetData(x, i, GetData(x, i - 1));
					}
				}
				for (int c = 1; c < LENGTH_X - 1; c++) SetData(c, 0, BLANK); // �� �� �� ����
				r--;
			}
		}
		if (cl == 1) ClearedLine1++;
		else if (cl == 2) ClearedLine2++;
		else if (cl == 3) ClearedLine3++;
		else if (cl == 4) ClearedLine4++;

		ClearedLinesSum += cl;
	}
	void Hold() {
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				holdBlock[r][c] = nowBlock[0][r][c];
			}
		}
		if (hold) {
			int temp = nowBlockNumber;
			nowBlockNumber = holdBlockNumber;
			holdBlockNumber = temp;
			SetBlock(nowBlockNumber);
		}
		else {
			// ���� ����� Ȧ�� ������� �ű� ��, ���� �ؽ�Ʈ ����� �ҷ��� ���� ������� �ִ´�.
			hold = true;
			holdBlockNumber = nowBlockNumber;
			SetNewBlock();
		}
	}
	// SetBlock 1 ~ 7: ����� nowBlock���� �ҷ��� ���� ��Ͽ� �����Ѵ�.
	void SetBlock(int num) {
		if (num == 1) {
			for (int rot = 0; rot < 4; rot++) for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++)
				nowBlock[rot][r][c] = block1[rot][r][c];
		}
		if (num == 2) {
			for (int rot = 0; rot < 4; rot++) for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++)
				nowBlock[rot][r][c] = block2[rot][r][c];
		}
		if (num == 3) {
			for (int rot = 0; rot < 4; rot++) for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++)
				nowBlock[rot][r][c] = block3[rot][r][c];
		}
		if (num == 4) {
			for (int rot = 0; rot < 4; rot++) for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++)
				nowBlock[rot][r][c] = block4[rot][r][c];
		}
		if (num == 5) {
			for (int rot = 0; rot < 4; rot++) for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++)
				nowBlock[rot][r][c] = block5[rot][r][c];
		}
		if (num == 6) {
			for (int rot = 0; rot < 4; rot++) for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++)
				nowBlock[rot][r][c] = block6[rot][r][c];
		}
		if (num == 7) {
			for (int rot = 0; rot < 4; rot++) for (int r = 0; r < 4; r++) for (int c = 0; c < 4; c++)
				nowBlock[rot][r][c] = block7[rot][r][c];
		}
		nowBlockNumber = num;
	}
private:


};