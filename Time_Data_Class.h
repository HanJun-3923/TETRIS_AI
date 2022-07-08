#pragma once
#include "Basic_Setting.h"

class Time_Date {
	// ���� �ð��� ���� ������ ��� ���� class�̴�.
private:
	time_t timer;
	struct tm* t;
public:
	Time_Date() {
		srand((unsigned int)time(NULL));
		timer = time(NULL); // 1970�� 1�� 1�� 0�� 0�� 0�ʺ��� �����Ͽ� ��������� ��
		t = localtime(&timer); // �������� ���� ����ü�� �ֱ�
	}
	// ���� ������ ��� �޼ҵ�
	int getYear() { return t->tm_year + 1900; }
	int getMonth() { return t->tm_mon + 1; }
	int getDay() { return t->tm_mday; }
	int getHour() { return t->tm_hour; }
	int getMin() { return t->tm_min; }
	int getSec() { return t->tm_sec; }
};