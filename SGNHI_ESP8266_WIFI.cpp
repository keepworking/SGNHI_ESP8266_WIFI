#include "SGNHI_ESP8266_WIFI.h"

dotori::dotori(char *sencode){
	senCode = sencode;
}

void dotori::printcode(){
	DEBUG_PRINT(senCode);
}

//입력 변수형별 전송 데이터 타입 정리.
void dotori::set(int val){
	void * vo = &val;
	value = *(uint32_t*)vo;
	argType = atInt;
}

void dotori::set(float val){
	void * vo = &val;
	value = *(uint32_t*)vo;
	argType = atFloat;
}

void dotori::set(long val){
	void * vo = &val;
	value = *(uint32_t*)vo;
	argType = atLong;
}

void dotori::set(double val){
	void * vo = &val;
	value = *(uint32_t*)vo;
	argType = atDouble;
}

void sgnDev::init(char *id,char *devcode/*,IPAddress local_ip*/){
	//addr = local_ip;
	//init();
	devCode = devcode;
	ID = id;
	DEBUG_PRINT(devCode);
}


void sgnDev::setRest(unsigned long rest){
	restTime = rest < REST? REST:rest;
}


int sgnDev::send(dotori mdotori, ...){//iot_up 소스코드 수정해야함 -> 수정완료.
  	WiFi.waitForConnectResult();
	//return 1;
	//send value code 아래쪽 부터.
	unsigned long now = millis();

	if(state != 0){
		if(now <= sTime){
			unsigned long lastTime = 0xffffffff - sTime;
			if((lastTime + now < restTime)){
				return sgnhi_WAIT;
			}
		}else if(now - sTime < restTime){
			return sgnhi_WAIT;
		}
	}

	if(!WiFi.isConnected()){//와이파이 연결 상태 확인.
		WiFi.reconnect();//연결이 끊겼을 시 재시도.
		return sgnhi_EWIFI;
	}

	WiFiClient client;//클라이언트 객체 생성.

	if (client.connect(SERVER, 80)) {
		//DEBUG_PRINT("connected");
		//client.flush();
		client.print("GET /iot/iot_up.php?");
		client.print("uid=");client.print(ID);
		client.print("&dc=");client.print(devCode);
		int cnt = 0;
		va_list vl;
		va_start(vl,mdotori);
		for(dotori m = mdotori;m.chk == 42;m= va_arg(vl,dotori)){
			uint32_t value = m.value;
			void * vo = &m.value;
			client.print("&sc");client.print(cnt);client.print("=");
			client.print(m.senCode);
			client.print("&sv");client.print(cnt);client.print("=");
			client.print(MACHTYPE(vo,m.argType));
			//
			/*#ifdef DEBUG
			Serial.print("&sc");Serial.print(cnt);Serial.print("=");
			Serial.print(m.senCode);
			Serial.print("&sv");Serial.print(cnt);Serial.print("=");
			Serial.println(MACHTYPE(vo,m.argType));
			#endif*/
			//
			cnt++;
		}
		va_end(vl);

		client.print(" HTTP/1.0\r\n");
		client.print("Host:veyrobotics.cafe24.com \r\n");
		client.print("User-Agent: sgnhi\r\n");
		client.print("Connection: close\r\n");
		client.println();
		//Serial.println();
		state = client.status() == 0?0:1;
		//state = client.status() == 0?0:1;
		//while(client.connected());
		client.stop();
		sTime = now;

	}
	else {
		Serial.println(client.status());
		client.stop();
  		DEBUG_PRINT("connection failed");
  		DEBUG_PRINT("try to begin");
  		//esp에서는 dhcp요청을, begin에서함!
  		//wifi_station_dhcpc_start(); 함수가. 있음.
  		// WiFi.begin();// <-- wifi begin 은 아닌듯
  		state = 0;
  		return sgnhi_ERROR;
  	}
  	return sgnhi_OK;
}
/*
---- send함수 return type ----
#define sgnhi_OK 0 // 서버에 데이터 전송. 성공. <-- 아직 홈페이지의 리퀘스트를 확인하지 않음.
#define sgnhi_WAIT 1 // 서버에 전송한지 대기시간이 지나지 않음, 좀더 뒤에 전송 요망.
#define sgnhi_ERROR 2 // 웹사이트에 접속 실패.
--네트워크 연결 오류나, DHCP서버에서 IP를 할당 받지 못할경우, 해당에러 발생.
-- esp8266에서는 WiFi.begin(); 으로 dhcp요청을 함.
#define sgnhi_EWIFI 3 //와이파이 연결에 문제 있음. <-- 함수내에서 reconnect를 시도.
*/

sgnDev dev;//dev객체 미리 생성.
