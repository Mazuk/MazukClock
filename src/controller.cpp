#include <ArduinoJson.h>
#include <Arduino.h>

#include "config.h"
#include "grid.h"
#include "controller.h"
#include "httpServer.h"
#include "color.h"
#include "gui.h"
#include "wifi.h"
#include "time.h"
#include "led.h"
#include "utcOffset.h"
#include "animation.h"
#include "ticker.h"
#include "weather.h"

void Controller::index() {
  HttpServer::web.sendHeader("Location", "http://" + HttpServer::ip);
  HttpServer::web.setContentLength(CONTENT_LENGTH_UNKNOWN);
  //send header part1
  String content = Gui::index(1);
  HttpServer::web.send(200, "text/html", content);
  for (int i = 2; i < 8; i++) {
      content = Gui::index(i);//send in pices due to ESP8266 size limitation for variables
        #ifdef DEBUG
              Serial.println(content);
        #endif
      HttpServer::web.sendContent(content);
  }
 
  HttpServer::web.sendContent(""); //indetification for content end
    #ifdef DEBUG
      Serial.println("Page sent!");
    #endif // DEBUG

}

void Controller::saveColor() {
  String json = HttpServer::web.arg("plain");

  DynamicJsonDocument doc(2048);
  deserializeJson(doc, json);

  Config::color_fg = Color::hexToRgb(doc["fg"].as<String>());
  Config::color_bg = Color::hexToRgb(doc["bg"].as<String>());
  Config::color_mx = Color::hexToRgb(doc["mx"].as<String>());
  Config::mlength = doc["mlength"].as<int>();
  Config::atime = doc["atime"].as<int>();

  Config::color_m1 = Color::hexToRgb(doc["m1"].as<String>());
  Config::color_m2 = Color::hexToRgb(doc["m2"].as<String>());
  Config::color_m3 = Color::hexToRgb(doc["m3"].as<String>());
  Config::color_m4 = Color::hexToRgb(doc["m4"].as<String>());

 
  Config::power_supply = doc["power_supply"].as<int>();
  Config::brightness =
    (doc["brightness"].as<double>() > Led::getMaxBrightnessPercnt()) ? Led::getMaxBrightnessPercnt() : doc["brightness"].as<double>();
  Config::animation_bell = doc["animation_bell"].as<int>();
  Config::animation_type = doc["animation_type"].as<String>();
  Config::save();
  Grid::setTime(Time::hour, Time::minute);
  animation::test_start = true;
  HttpServer::web.send(200, "text/html", "");
}

void Controller::saveTime() {
  String json = HttpServer::web.arg("plain");

  DynamicJsonDocument doc(2048);
  deserializeJson(doc, json);

  Config::automatic_timezone = doc["tz_auto"].as<int>() == 1;

  // store the last known value anyway.
  Config::timezone = doc["tz"].as<int>();

  if (Config::automatic_timezone) {
    UtcOffset::updateLocalizedUtcOffset();
  }

  Config::ntp = doc["ntp"].as<String>();
  Config::DNS_name = doc["DNS_name"].as<String>();
  Config::viertel = doc["viertel"].as<int>() == 1;
 
  Config::save();
  //Serial.println("Reset..");
  delay(1000);
  ESP.restart();
  Grid::setTime(Time::hour, Time::minute);

  HttpServer::web.send(200, "text/html", "");
}

void Controller::saveDnd() {
  String json = HttpServer::web.arg("plain");

  DynamicJsonDocument doc(2048);
  deserializeJson(doc, json);


  Config::dnd_active = doc["dnd_active"].as<int>() == 1;
  Config::dnd_start.hour = doc["dnd_start_hour"].as<int>();
  Config::dnd_start.minute = doc["dnd_start_minute"].as<int>();
  Config::dnd_end.hour = doc["dnd_end_hour"].as<int>();
  Config::dnd_end.minute = doc["dnd_end_minute"].as<int>();
  //TICKER
  Config::ticker = doc["ticker"].as<String>();
  Config::color_tc = Color::hexToRgb(doc["tc"].as<String>());
  Config::tspeed = doc["tspeed"].as<int>();
  Config::tu = doc["tu"].as<int>();
  Config::save();
  animation::ClearDisplay();
  ticker::DisplayTicker(String(Config::ticker), Config::tspeed, CRGB(Config::color_tc.r, Config::color_tc.g, Config::color_tc.b ));
  animation::SetDisplay();
  FastLED.show();
  //Grid::setTime(Time::hour, Time::minute);

  HttpServer::web.send(200, "text/html", "");
}

void Controller::saveWeather() {
    String json = HttpServer::web.arg("plain");

    DynamicJsonDocument doc(2048);
    deserializeJson(doc, json);



    //Weather strings
    Config::WX = doc["WX"].as<int>();
    Config::WXcommand = doc["WXcommand"].as<String>();
    Config::apikey = doc["apikey"].as<String>();
    Config::lon = doc["lon"].as<String>();
    Config::lat = doc["lat"].as<String>();
    Config::save();
    animation::ClearDisplay();
    weather::SetGrad(true);
    ticker::DisplayTicker(String(weather::GetWeather()), 150, weather::GetTempColor(weather::temperature));
    weather::SetGrad(false);
    if (animation::MovinRainbowSet != true) {
        animation::SetDisplay();
        //Grid::setTime(Time::hour, Time::minute);
        FastLED.show();
    }
    //Grid::setTime(Time::hour, Time::minute);

    HttpServer::web.send(200, "text/html", "");
}


void Controller::deleteWiFi() {
  Wifi::reset();
  ESP.restart();
  delay(1000);
  HttpServer::web.send(200, "text/html", "");
}

String Controller::getLogoSvg() {
  String content = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><svg width=\"32\" height=\"32\" version=\"1.1\" viewBox=\"0 0 270.93 270.93\" xmlns=\"http://www.w3.org/2000/svg\"><rect x=\".13216\" y=\".13216\" width=\"270.67\" height=\"270.67\" rx=\"26.433\" ry=\"26.433\" fill=\"#333\"/><rect x=\"15.992\" y=\"15.992\" width=\"238.95\" height=\"238.95\" rx=\"23.335\" ry=\"23.335\" fill=\"#eee\"/><g fill=\"#3d72a8\"><circle cx=\"90.488\" cy=\"50.271\" r=\"13.098\"/><circle cx=\"47.096\" cy=\"50.271\" r=\"13.098\"/><circle cx=\"90.488\" cy=\"93.662\" r=\"13.098\"/><circle cx=\"133.88\" cy=\"93.662\" r=\"13.098\"/><circle cx=\"177.4\" cy=\"93.662\" r=\"13.098\"/><circle cx=\"133.88\" cy=\"180.45\" r=\"13.098\"/><circle cx=\"177.4\" cy=\"180.45\" r=\"13.098\"/><circle cx=\"220.79\" cy=\"180.45\" r=\"13.098\"/><circle cx=\"47.096\" cy=\"180.45\" r=\"13.098\"/></g></svg>";

  //HttpServer::web.send(200, "image/svg+xml", content);
  return content;
}

void Controller::getManifest() {
  String content = "";

  content += "{";
  content += "\"background_color\": \"#FFFFFF\",";
  content += "\"description\": \"WordClock\",";
  content += "\"dir\": \"ltr\",";
  content += "\"display\": \"standalone\",";
  content += "\"icons\": [{\"sizes\": \"128x128\", \"src\": \"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAIAAAACACAMAAAD04JH5AAAAXVBMVEUzMzMAAAAzMzMyMjIzMzMzMzMyMjI1NTXu7u4zMzM9cqiIiIhIeq3AztyZss1kjrji5uh8nsFzmb7T2+SvwtWmvNJWhLPM1+FPT0/Hx8erq6ujo6NycnJcXFyKqMdkRsj5AAAACHRSTlPwANm7MYVREuXaafsAAAKuSURBVHja7NHJeoMwDEVhx2aQHXkgDAEC7/+aTRqD+KDtKjKb/su74WCJS5TlSopEpMqzSxQDCqkTk8UmoFT6BKpcAjKpTyGzd0Ap9Ulk+R2g9GnUK6DQJyqeAbsDzNOV0TTvjnARmd7oxwDMwtjrjUzkmjwgiYcmuVD0+wMkMtAjKCH1YoBkBr2QQqR8/+MVxBrQQ1L9IWCEpMZDQICkwj5ghsTmXcAEG3XTWtizbVPD50y7gCus7t48oYMth6/R3+FTrr8GWDRvNyC3uKFlDwhoFo7+f90wcAc4s0JYII2OO8AbYuNmDfHcAWhIG7fWEOQO6Axp4tYY0nEHVIbUcasNqbgD7E/n9jRa7gB6AqxhcUd6AOYAKkALxCJ9nz0AbNWhdwG2gvPYVRb4A/72H/DFjh3qAAjDQBi2E5BAN5C8/2PiGMlPmolT7OxS8an2bgYYMDEAiyjdWLEqAVzF3Nm4GiED8BjxavWsXJ/BEAF4jtO7Hf2xKQBZIImP5LK93kIHYCRjdmN6XTQAhtIsvTY9gLF8NL+fGgCLyWiDuTQAVrO0wx0YFABQTrMWu/dBAYD1vJYy2uO1AH5Q5D8ZP7yGBhhggAEGGHC3b8YqCANREGytRCIhTbj//0zLHIwcTKGrsFuGBxnSBOa9LUABClCAAhSgAFcCGxNEiEoM6p0R40XlwOACAP4REaLyej8IxN4QkaLywOACAP6R0aJyTA8PvTtmhKikz9v19pwRopKkd30/wGhRueFTmQsKRovKfRp86BsSRohKUA1/RcN4UXlgcAEA/8h4UfncOEgA+kdGiEoO/tHf8F0KUIACFKAABShAAT4OcN6+nPPXzvvjBYd8xSNecknVfPJFp0jVa2oc5stu8bpfvPCYr3zGS6/52m+8+JyvfufL78n6/wsOPRSv+uhUewAAAABJRU5ErkJggg==\", \"type\": \"image/png\"}],";
  content += "\"lang\": \"en-US\",";
  content += "\"name\": \"Mazuks WordClock\",";
  content += "\"short_name\": \"WordClock\",";
  content += "\"start_url\": \"/\",";
  content += "\"theme_color\": \"#3d72a8\"";
  content += "}";

  HttpServer::web.send(200, "application/json", content);
}

void Controller::notFound() {
  if(HttpServer::web.method() == HTTP_OPTIONS) {
    HttpServer::web.sendHeader("Allow", "DELETE,GET,HEAD,OPTIONS,POST,PUT");
    HttpServer::web.send(200, "text/html", "");

    return;
  }

  HttpServer::web.send(404, "text/html", "NOT FOUND");
}
