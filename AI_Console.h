#pragma once
#include "Basic_Setting.h"

class AI_Console {
	// 콘솔과 연관된 데이터 및 함수들을 다룬다.
public:
	void DrawBasicFrame() {
		// 틀은 이후 바뀔 일이 없다. 따라서 콘솔과 직접적인 관계를 가지고 직접 출력한다.
		// 이동 가능 여부 등을 판단할 때 벽의 존재를 알아야 하므로, tableData에 데이터를 넣는다.
		// 벽을 포함하여 가로 12, 세로 25 이다.
		for (int i = 0; i < LENGTH_X; i++) {
			Draw(i, 0, "▩");
			Draw(i, LENGTH_Y - 1, "▩");
			SetData(i, LENGTH_Y - 1, WALL);
		}
		for (int i = 1; i < LENGTH_Y - 1; i++) {
			Draw(0, i, "▩");
			Draw(LENGTH_X - 1, i, "▩");
			SetData(0, i, WALL);
			SetData(LENGTH_X - 1, i, WALL);
		}
		SetData(0, 0, WALL);
		SetData(LENGTH_X - 1, 0, WALL);
	}
	void PrintTableData() {
		// tableData의 값을 보여준다.
		for (int r = 0; r < LENGTH_Y; r++) {
			for (int c = 0; c < LENGTH_X; c++) {
				gotoxy(2 * c + INITIAL_X, r + INITIAL_Y);
				cout << GetData(c, r) << " ";
			}
		}
	}
	void DrawTable() {
		// tableData에 따라 게임 테이블을 그린다.
		for (int r = 1; r < LENGTH_Y - 1; r++) {
			for (int c = 1; c < LENGTH_X - 1; c++) {
				int data = GetData(c, r);
				if (BLOCK1 <= data && data <= BLOCK7) { setColor(data);  Draw(c, r, "■"); }
				else if (SOLID_BLOCK1 <= data && data <= SOLID_BLOCK7) { setColor(data - 10); Draw(c, r, "▣"); }
			}
		}
		setColor(0);
	}
	void ClearTable() {
		// 솔리드 블록을 제외한 다른 블록들을 모두 지운다.
		for (int r = 1; r < LENGTH_Y - 1; r++) {
			for (int c = 1; c < LENGTH_X - 1; c++) {
				int data = GetData(c, r);
				if (SOLID_BLOCK1 <= data && data <= SOLID_BLOCK7) continue;
				else Draw(c, r, "  ");
			}
		}
	}
	void ClearNowBlockData() {
		// tableData에서 현재 블록의 데이터들을 찾아 지워준다
		for (int r = 0; r < LENGTH_Y - 1; r++) {
			for (int c = 1; c < LENGTH_X - 1; c++) {
				int data = GetData(c, r);
				if (SOLID_BLOCK1 <= data && data <= SOLID_BLOCK7) continue;
				SetData(c, r, BLANK);
			}
		}
	}
	void SetData(int x1, int y1, int data) {
		// tableData에서 x1, y1의 값을 data로 변경한다.
		tableData[y1][x1] = data;
	}
	int GetData(int x1, int y1) {
		// tableData에서 x1, y1의 데이터를 얻는다.
		return tableData[y1][x1];
	}
private:
	int tableData[LENGTH_Y][LENGTH_X] = {}; // 게임창의 모든 데이터를 가지고 있는 배열이다.
	// GetData, SetData를 통해 값을 수정 또는 값을 추출한다.
};