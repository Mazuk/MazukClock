#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>

#include "wifi.h"
#include "config.h"
#include "ticker.h"

/* Set these to your desired credentials. */
const byte DNSPort = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;

const char* ssid = "WordClockDir";
const char* password = "clock123";
static bool AP_TRUE = false;


void Wifi::setup() {
	WiFi.hostname("WordClock");
	WiFi.mode(WIFI_STA);
	WiFiManager wifiManager;
	wifiManager.setTimeout(60);
	//wifiManager.autoConnect("WordClock");

	if (!wifiManager.autoConnect("WordClock")) {
		Serial.println("Wifi failed to connect to WordClock. Trying now WordClockDir");
		String error = "SSID: WordClockDir  PASS: clock123";
		ticker::DisplayTicker(error, 150, CRGB::Red);
		wifiManager.resetSettings();
		WiFi.hostname(ssid);
		WiFi.mode(WIFI_AP);
		WiFi.softAP(ssid,password);

		WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
		dnsServer.start(DNSPort, "*", apIP);

		IPAddress myIP = WiFi.softAPIP();
		Serial.print("AP IP address: ");
		Serial.println(myIP);
		AP_TRUE = true;
		
	}
	
	if (Config::DNS_name != "") {
		if (MDNS.begin(Config::DNS_name)) { // Start the mDNS responder for  http://wordclock
			Serial.print("mDNS responder started: ");
			Serial.println(Config::DNS_name);
			Wifi::mDNSsetup();
		}
		else
			Serial.println("Error setting up MDNS responder!");
	}
	
}

void Wifi::loop() {
	
	unsigned long  now = millis();
	unsigned long static lastTime;
	if (now - lastTime < 1000) // use millis to have better access to httpserver, not blocking loop!
		return;
	lastTime = now;
	
	if (Config::DNS_name != "") 
		MDNS.update();
		
	if(AP_TRUE)
		dnsServer.processNextRequest();
}

void Wifi::mDNSsetup() {
	MDNS.addService("http", "tcp", 80);
	MDNS.addService("http", "tcp", 81);
}

void Wifi::reset() {
	WiFi.disconnect(true);
}