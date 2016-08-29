#include <ESP8266WiFi.h>
#include <SGNHI_ESP8266_WIFI.h>

dotori sensor("SENSOR CODE");//센서 객체를 선언합니다.

void setup() {
  dev.init("USER ID","DEVICE CODE");//장치의 사용을 설정합니다.
}

void loop() {
  sensor.set(0);//센서 객체의 값을 0으로 지정합니다.
  WiFi.waitForConnectResult();
  dev.send(sensor);//센서데이터를 전송합니다.
  delay(1000);
}
