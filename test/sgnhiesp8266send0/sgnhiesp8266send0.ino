#include <ESP8266WiFi.h>
#include <SGNHI_ESP8266_WIFI.h>

dotori PIRsensor("qvIic");

void setup() {
  dev.init("admin","1HM3CJsHd1MsiAyi176I");
}

void loop() {
  WiFi.waitForConnectResult();
  dev.send(PIRsensor);
  delay(1000);
}
