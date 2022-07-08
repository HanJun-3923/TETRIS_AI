#pragma once
#include "Basic_Setting.h"

class Time_Date {
	// 현재 시각에 대한 정보를 얻기 위한 class이다.
private:
	time_t timer;
	struct tm* t;
public:
	Time_Date() {
		srand((unsigned int)time(NULL));
		timer = time(NULL); // 1970년 1월 1일 0시 0분 0초부터 시작하여 현재까지의 초
		t = localtime(&timer); // 포맷팅을 위해 구조체에 넣기
	}
	// 각종 정보를 얻는 메소드
	int getYear() { return t->tm_year + 1900; }
	int getMonth() { return t->tm_mon + 1; }
	int getDay() { return t->tm_mday; }
	int getHour() { return t->tm_hour; }
	int getMin() { return t->tm_min; }
	int getSec() { return t->tm_sec; }
};