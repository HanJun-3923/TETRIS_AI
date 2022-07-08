#pragma once
#include "Basic_Setting.h"
#include "AI_Console.h"

class AI_Block : public AI_Console {
	// AI가 블록을 놓는 과정에서 필요한 함수들을 다룬다.
public:
	int x = 0, y = 0, rot = 0; // 현재 좌표, 회전상태
	int nextOrder[2 * THE_NUM_OF_BLOCKS] = {}; // 2가방의 넥스트(7개 x 2)
	int reNextCount = 0; // 넥스트 갱신 변수 -> 7이 되면 넥스트 Refresh
	int nowBlock[4][4][4] = {}; // 현재 블록의 데이터를 가지고 있다. 
	int nowBlockNumber = -1;
	int holdBlock[4][4] = {}; // 홀드 블록의 데이터를 가지고 있다.
	bool hold = false; // 현재 홀드 중인지 여부
	int holdBlockNumber = -1;
	int ClearedLinesSum = 0; // 지워진 라인의 개수이다.
	int ClearedLine1 = 0;
	int ClearedLine2 = 0;
	int ClearedLine3 = 0;
	int ClearedLine4 = 0;


	bool CanMove(int DIRECTION) {
		// 해당 방향으로 이동이 가능한지 여부를 판단한다.
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
	// 해당 방향으로 블록을 이동시킨다. 
	void BlockMove(int DIRECTION) {
		// 블록을 이동시킨다.
		// 현재 블록의 데이터를 지운다. -> 좌표를 이동한다. -> 좌표에 따라 블록 데이터를 다시 넣는다.
		ClearNowBlockData();
		if (DIRECTION == LEFT) x--;
		else if (DIRECTION == DOWN) y++;
		else if (DIRECTION == RIGHT) x++;
		//SetNowBlockData();
	}
	void BlockMoveLeftMax() {
		// 블록을 맨 왼쪽으로 이동시킨다.
		for (; CanMove(LEFT);) { // 맨 왼쪽으로 보내기
			BlockMove(LEFT);
		}
	}
	void BlockMoveRightMax() {
		// 블록을 맨 오른쪽으로 이동시킨다.
		for (; CanMove(RIGHT);) { // 맨 오른으로 보내기
			BlockMove(RIGHT);
		}
	}
	void BlockMoveDownMax() {
		// 블록을 맨 아래쪽으로 이동시킨다.
		for (; CanMove(DOWN);) {
			BlockMove(DOWN);
		}
	}
	void SetPosInitial() {
		// 블록의 좌표를 초기 좌표로 이동시킨다.
			// 위치를 초기 위치로 설정.
			// 4, 0 을 초기로 정한다.
		x = 4; y = 0;
	}
	void SetNextOrder() {
		// 넥스트 블록들의 순서를 결정한다.
			// 초기 넥스트 2가방 (14개)를 7-bag에 맞춰 만들어준다.
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
		// 2번째 가방의 넥스트를 1번째 가방으로 옮긴 뒤, 2번째 가방을 생성한다.
			// 앞 가방 7가지 블록을 모두 사용했을 때, 2번째 가방의 넥스트를 1번째 가방으로 옮겨준 뒤, 2번째 가방의 넥스트를 생성
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
		// 넥스트에 따른 새로운 블록을 가져온다.
			// 넥스트에 따른 블록을 가져와 nowBlock 에 넣는다.
			// 좌표 초기화, reNextCount 증가 및 Refresh 작업을 같이 한다.
		SetBlock(nextOrder[reNextCount]);
		SetPosInitial();
		reNextCount++;
		if (reNextCount >= 7) {
			reNextCount = 0;
			NextRefresh();
		}
	}
	void SetNowBlockData() {
		// 현재 블록의 데이터들 tableData에 넣는다.
			// NowBlock 데이터 -> tableData
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
		// 현재 블록을 Solid 한다.
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				int data = nowBlock[rot][r][c];
				if (BLOCK1 <= data && data <= BLOCK7) {
					SetData(c + x, r + y, data + 10); // 블록에서 10이 더해진 값이 솔리드 블록이다. (하드코딩)
				}
			}
		}
	}
	bool CanLineClear(int r) {
		// 해당 라인이 라인 클리어가 가능한지 검사한다.
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
		// 모든 행의 라인을 검사하여 라인클리어가 가능하다면 한다.
		// 라인클리어가 되면 지워진 라인을 증가시킨다.
		for (int r = 1; r < LENGTH_Y - 1; r++) {
			if (CanLineClear(r)) { // 라인클리어 여부 검사
				cl++;  // 지워진 라인 카운트

				for (int c = 1; c < LENGTH_X - 1; c++) SetData(c, r, BLANK); // 해당 열 지우기
				for (int i = r; i > 0; i--) { // 해당 열부터 맨 위까지 바로 윗 행의 열 복사하여 가져오기.
					for (int x = 1; x < LENGTH_X - 1; x++) {
						SetData(x, i, GetData(x, i - 1));
					}
				}
				for (int c = 1; c < LENGTH_X - 1; c++) SetData(c, 0, BLANK); // 맨 윗 열 생성
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
			// 현재 블록을 홀드 블록으로 옮긴 뒤, 다음 넥스트 블록을 불러와 현재 블록으로 넣는다.
			hold = true;
			holdBlockNumber = nowBlockNumber;
			SetNewBlock();
		}
	}
	// SetBlock 1 ~ 7: 블록을 nowBlock으로 불러와 현재 블록에 저장한다.
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