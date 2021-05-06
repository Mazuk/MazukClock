#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <TimeLib.h>
#include "config.h"
#include "weather.h"
#include "led.h"

float weather::temperature = 0;
String weather::City = "";
String weather::Country = "";

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Get the weather from OpenWeatherMap
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
String weather::GetWeather() {
    static int lastupdate = 0;
    static int presentupdate = 0;
    String content;
    String lat = "48";
    String lon = "7";

    //String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
    String serverPath = "http://api.openweathermap.org/data/2.5/weather?lat=" + Config::lat + "&lon=" + Config::lon + "&units=metric&APPID=" + Config::apikey;

    String jsonBuffer;
    static String jsonBufferold = ""; // backup in case empty return use old

    StaticJsonDocument<1280> doc;
    jsonBuffer = httpGETRequest(serverPath.c_str());
#ifdef DEBUG
    Serial.println(serverPath);
#endif
    if (jsonBuffer == "{}" && jsonBufferold != "") { //no data from opwenweather server
        if (presentupdate - lastupdate < 600) { //do only if less than 10 minutes no update
            jsonBuffer = jsonBufferold;

        }
        else
            return "NA";
    }
    else {
        jsonBufferold = jsonBuffer;
        lastupdate = presentupdate;
    }
    deserializeJson(doc, jsonBuffer);
#ifdef DEBUG
    Serial.print("JSON object = ");
    serializeJsonPretty(doc, Serial);
    Serial.print("Temperatur: ");
#endif
    weather::temperature = doc["main"]["temp"];
    weather::City = doc["name"].as<String>();
    weather::Country = doc["sys"]["country"].as<String>();
    presentupdate = doc["dt"]; //UTC time from openweather
    char command[50];
    for (int i = 0; i < 50; i++) //initialize command buffer
        command[i] = 0;
    strcpy(command, Config::WXcommand.c_str());

    int i = 0;
    while (command[i] != 0) {
        if (command[i] == '$') {
            if (toupper(command[i + 1]) == 'T') { //Temperature
                char buff[6];
                float t = doc["main"]["temp"].as<float>();
                sprintf(buff, "%2.1f", t);
                content += buff;
                i++;
            }
            else if (toupper(command[i + 1]) == 'H') { //Humidity
                content += doc["main"]["humidity"].as<String>();
                i++;
            }
            else if (toupper(command[i + 1]) == 'W') { //Wind
                content += doc["wind"]["speed"].as<String>();
                i++;
            }
            else if (toupper(command[i + 1]) == 'D') {//Wind direction
                content += doc["wind"]["deg"].as<String>();
                i++;
            }
            else if (toupper(command[i + 1]) == 'P') {//Pressure
                content += doc["main"]["pressure"].as<String>();
                i++;
            }
            else if (toupper(command[i + 1]) == 'R') { //Sunrise
                unsigned long t = doc["sys"]["sunrise"].as<unsigned int>();
                t += doc["timezone"].as<unsigned int>(); //add GMT offset
                char buff[8];
                sprintf(buff, "%02d:%02d", hour(t), minute(t));
                content += buff;
                i++;
            }
            else if (toupper(command[i + 1]) == 'S') { //Sunset
                unsigned long t = doc["sys"]["sunset"].as<unsigned int>();
                t += doc["timezone"].as<unsigned int>(); //add GMT offset
                char buff[8];
                sprintf(buff, "%02d:%02d", hour(t), minute(t));
                content += buff;
                i++;
            }
            else {
                content += "Err-";
            }
        }
        else
            content += command[i];
        i++;
    }
#ifdef DEBUG
    Serial.println(content);
#endif
    return content;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * HTTP Request from an server
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
String weather::httpGETRequest(const char* serverName) {
    HTTPClient http;

    // Your IP address with path or Domain name with URL path 
    http.begin(serverName);

    // Send HTTP POST request
    int httpResponseCode = http.GET();

    String payload = "{}";

    if (httpResponseCode > 0) {
#ifdef DEBUG
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
#endif
        payload = http.getString();
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();

    return payload;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Change the display color based on the temperature
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
CRGB weather::GetTempColor(float temperature) {
    CRGB color;
    if (temperature <= 0) {
        color = CRGB::Blue;
    }
    else if (temperature > 0 && temperature < 5) {
        color = CRGB::LightBlue;
    }
    else if (temperature >= 5 && temperature < 10) {
        color = CRGB::Aqua;
    }
    else if (temperature >= 10 && temperature < 15) {
        color = CRGB::Green;
    }
    else if (temperature >= 15 && temperature < 20) {
        color = CRGB::GreenYellow;
    }
    else if (temperature >= 20 && temperature < 25) {
        color = CRGB::Orange;
    }
    else if (temperature >= 25 && temperature < 30) {
        color = CRGB::OrangeRed;
    }
    else
        color = CRGB::Red;

    return color;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Last row of display has the G R A D letters between Minutes set it on when Weather data active
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void weather::SetGrad(bool state) { //true on, false off
    if (USE_GRAD == 0)
        return;
    for (int i = 1; i < 8; i += 2) {
        if (state)
            Led::ids[i].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
        else
            Led::ids[i].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
    }
    FastLED.show();
}
