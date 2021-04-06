#include "httpServer.h"
#include "gui.h"
#include "controller.h"
#include "wifi.h"




    
void HttpServer::setup() {
	
    HttpServer::ip = WiFi.localIP().toString();
   
	HttpServer::web.on("/", HTTP_GET, Controller::index);
	HttpServer::web.on("/api/color", HTTP_PUT, Controller::saveColor);
	HttpServer::web.on("/api/time", HTTP_PUT, Controller::saveTime);
	HttpServer::web.on("/api/dnd", HTTP_PUT, Controller::saveDnd);
	HttpServer::web.on("/api/weather", HTTP_PUT, Controller::saveWeather);
	HttpServer::web.on("/api/wifi", HTTP_DELETE, Controller::deleteWiFi);

	HttpServer::web.on("/manifest.json", Controller::getManifest);
	//HttpServer::web.on("/logo.svg", Controller::getLogoSvg);

	//HttpServer::web.onNotFound(Controller::notFound);
	
	HttpServer::web.onNotFound([]() {
		Controller::index();
		});
	
    HttpServer::web.begin();
	Serial.println("TCP server started");

}
  
void HttpServer::loop() {
	/*
	unsigned long  now = millis();
	unsigned long static lastTime;
	if (now - lastTime < 2000) // use millis to have better access to httpserver, not blocking loop!
		return;
	lastTime = now;
	*/
	HttpServer::web.handleClient();
}

ESP8266WebServer HttpServer::web(80);
String HttpServer::ip;
