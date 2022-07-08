#pragma once
#include "Basic_Setting.h"

class AI_Console {
	// �ְܼ� ������ ������ �� �Լ����� �ٷ��.
public:
	void DrawBasicFrame() {
		// Ʋ�� ���� �ٲ� ���� ����. ���� �ְܼ� �������� ���踦 ������ ���� ����Ѵ�.
		// �̵� ���� ���� ���� �Ǵ��� �� ���� ���縦 �˾ƾ� �ϹǷ�, tableData�� �����͸� �ִ´�.
		// ���� �����Ͽ� ���� 12, ���� 25 �̴�.
		for (int i = 0; i < LENGTH_X; i++) {
			Draw(i, 0, "��");
			Draw(i, LENGTH_Y - 1, "��");
			SetData(i, LENGTH_Y - 1, WALL);
		}
		for (int i = 1; i < LENGTH_Y - 1; i++) {
			Draw(0, i, "��");
			Draw(LENGTH_X - 1, i, "��");
			SetData(0, i, WALL);
			SetData(LENGTH_X - 1, i, WALL);
		}
		SetData(0, 0, WALL);
		SetData(LENGTH_X - 1, 0, WALL);
	}
	void PrintTableData() {
		// tableData�� ���� �����ش�.
		for (int r = 0; r < LENGTH_Y; r++) {
			for (int c = 0; c < LENGTH_X; c++) {
				gotoxy(2 * c + INITIAL_X, r + INITIAL_Y);
				cout << GetData(c, r) << " ";
			}
		}
	}
	void DrawTable() {
		// tableData�� ���� ���� ���̺��� �׸���.
		for (int r = 1; r < LENGTH_Y - 1; r++) {
			for (int c = 1; c < LENGTH_X - 1; c++) {
				int data = GetData(c, r);
				if (BLOCK1 <= data && data <= BLOCK7) { setColor(data);  Draw(c, r, "��"); }
				else if (SOLID_BLOCK1 <= data && data <= SOLID_BLOCK7) { setColor(data - 10); Draw(c, r, "��"); }
			}
		}
		setColor(0);
	}
	void ClearTable() {
		// �ָ��� ����� ������ �ٸ� ��ϵ��� ��� �����.
		for (int r = 1; r < LENGTH_Y - 1; r++) {
			for (int c = 1; c < LENGTH_X - 1; c++) {
				int data = GetData(c, r);
				if (SOLID_BLOCK1 <= data && data <= SOLID_BLOCK7) continue;
				else Draw(c, r, "  ");
			}
		}
	}
	void ClearNowBlockData() {
		// tableData���� ���� ����� �����͵��� ã�� �����ش�
		for (int r = 0; r < LENGTH_Y - 1; r++) {
			for (int c = 1; c < LENGTH_X - 1; c++) {
				int data = GetData(c, r);
				if (SOLID_BLOCK1 <= data && data <= SOLID_BLOCK7) continue;
				SetData(c, r, BLANK);
			}
		}
	}
	void SetData(int x1, int y1, int data) {
		// tableData���� x1, y1�� ���� data�� �����Ѵ�.
		tableData[y1][x1] = data;
	}
	int GetData(int x1, int y1) {
		// tableData���� x1, y1�� �����͸� ��´�.
		return tableData[y1][x1];
	}
private:
	int tableData[LENGTH_Y][LENGTH_X] = {}; // ����â�� ��� �����͸� ������ �ִ� �迭�̴�.
	// GetData, SetData�� ���� ���� ���� �Ǵ� ���� �����Ѵ�.
};