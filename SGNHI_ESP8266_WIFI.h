#ifndef SGNHI_ESP8266
#define SGNHI_ESP8266

#include <ESP8266WiFi.h>
#if ARDUINO < 100
  #include "WProgram.h"
#else
    #include "Arduino.h"
#endif

#define SERVER "veyrobotics.cafe24.com"//서버 도메인.

#define atInt 1
#define atFloat 2
#define atDouble 3
#define atLong 4

#define sgnhi_OK 0 // 서버에 데이터 전송.
#define sgnhi_WAIT 1 // 서버에 전송한지 대기시간이 지나지 않음, 좀더 뒤에 전송 요망.
#define sgnhi_ERROR 2 // 웹사이트에 접속 실패.
#define sgnhi_EWIFI 3 //와이파이 연결에 문제 있음.

#define MACHTYPE(v,t) (t == atInt?*(int*)v:(t == atFloat?*(float*)v:(t == atDouble?*(double*)v:(t == atLong?*(long*)v:0))))
//값을 업로드할때, 다양한 형식의 변수를 지원하기 위함.


#define REST 1000 * 10 * 60// 10분 기본, 최소 업데이트 주기.

#define DEBUG

#ifdef DEBUG
 #define DEBUG_PRINT(x) Serial.println(x)
#else
 #define DEBUG_PRINT(x)
#endif



class dotori {
public:
	dotori(char *sencode);
	void printcode();
	void set(int val);
	void set(float val);
	void set(long val);
	void set(double val);
	int argType;
	char* senCode;
	int chk = 42;
	uint32_t value;
	//int request(void *val,int type);
	//
	//vvoid setvalue(void *val);
private:

};




//장치 메인 객체 사용자가 추가할 필요 x
class sgnDev {
public:
	void init(char *id,char *devCode);
	int send(dotori mdotori,...);
  int mail(char *,char *);
	void setRest(unsigned long rest);
private:
	char* ID;
	char* devCode;
	unsigned long sTime;
	int state = 0;
	unsigned long restTime = REST;
};

extern sgnDev dev;


#endif
