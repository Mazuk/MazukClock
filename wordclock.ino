#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <FastLED.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "FS.h"
#include "RTClib.h"

#include "src/grid.h"
#include "src/wifi.h"
#include "src/types.h"
#include "src/color.h"
#include "src/config.h"
#include "src/dnd.h"
#include "src/led.h"
#include "src/time.h"
#include "src/httpServer.h"
#include "src/gui.h"
#include "src/controller.h"
#include "src/animation.h"

void setup() {
  Serial.begin(115200);
  Serial.println();
  SPIFFS.begin();

  Config::load();
  Led::setup();
  Wifi::setup();
  HttpServer::setup();
  Time::setup();
  animation::setup();

  //define new LED data pin as D3. D4 is used for blue board status
  pinMode(D4, OUTPUT); // Define LED pin as output
  digitalWrite(D4, HIGH); // Switch the blue board status LED OFF
}

void loop() {
  Time::loop();
  HttpServer::loop();
  Wifi::loop();
  animation::loop();
}
