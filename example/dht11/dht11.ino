#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

#include <ESP8266WiFi.h>
#include <SGNHI_ESP8266_WIFI.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
dotori humi("sensorcode");
dotori temp("sensorcode");

void setup() {
  wifi_set_sleep_type(NONE_SLEEP_T);
  dev.init("userID","devcode");
  dht.begin();
}

float t,h;

void loop() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  humi.set(h);
  temp.set(t);
  dev.send(humi,temp);
  delay(1000);
}
