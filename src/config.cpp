#include <FS.h>
#include <ArduinoJson.h>

#include "config.h"
#include "types.h"
#include "time.h"
#include "led.h"
#include "utcOffset.h"


void Config::save() {
    
  File file = SPIFFS.open("/wordclock_config.json", "w");
  
  if(!file) {
    Serial.println("Can't open wordclock_config.json for writing");
    return;
  }

  Serial.println("Save config.");

  Time::ntpClient.setPoolServerName(Config::ntp.c_str());
  Time::ntpClient.setTimeOffset(Config::timezone);

  StaticJsonDocument<1530> doc;
  StaticJsonDocument<256> animationBuff;
  
  doc["color_bg_r"] = Config::color_bg.r;
  doc["color_bg_g"] = Config::color_bg.g;
  doc["color_bg_b"] = Config::color_bg.b;
  doc["color_fg_r"] = Config::color_fg.r;
  doc["color_fg_g"] = Config::color_fg.g;
  doc["color_fg_b"] = Config::color_fg.b;
  doc["color_mx_r"] = Config::color_mx.r;
  doc["color_mx_g"] = Config::color_mx.g;
  doc["color_mx_b"] = Config::color_mx.b;

  doc["color_m1_r"] = Config::color_m1.r;
  doc["color_m1_g"] = Config::color_m1.g;
  doc["color_m1_b"] = Config::color_m1.b;

  doc["color_m2_r"] = Config::color_m2.r;
  doc["color_m2_g"] = Config::color_m2.g;
  doc["color_m2_b"] = Config::color_m2.b;

  doc["color_m3_r"] = Config::color_m3.r;
  doc["color_m3_g"] = Config::color_m3.g;
  doc["color_m3_b"] = Config::color_m3.b;

  doc["color_m4_r"] = Config::color_m4.r;
  doc["color_m4_g"] = Config::color_m4.g;
  doc["color_m4_b"] = Config::color_m4.b;

  doc["mlength"] = Config::mlength;
  doc["atime"] = Config::atime;
  doc["power_supply"] = Config::power_supply;
  doc["brightness"] = Config::brightness;
  doc["animation_bell"] = Config::animation_bell;
  doc["animation_type"] = Config::animation_type;
 
  deserializeJson(animationBuff, Config::animation_type);
  //Serial.println(Config::animation_type);
  
  Config::animation_set = 0;
  int i = 0;
  while ((int)animationBuff[i] != 0 && i < 30) { // just in case if garbage i < 30
      Config::animation_set = (int)animationBuff[i]; // to check if animation / DEMO highest selection #
      Config::animation[i] = (int)animationBuff[i];
      i++;
  }
  i--;
  Config::animation[LENGTH] = i; // how many elements selected
    #ifdef DEBUG
      Serial.print("Config Len:");
      Serial.println(Config::animation[LENGTH]);
    #endif
  doc["tz_auto"] = Config::automatic_timezone;
  doc["timezone"] = Config::timezone;
  doc["dnd_active"] = Config::dnd_active;
  doc["dnd_start_hour"] = Config::dnd_start.hour;
  doc["dnd_start_minute"] = Config::dnd_start.minute;
  doc["dnd_end_hour"] = Config::dnd_end.hour;
  doc["dnd_end_minute"] = Config::dnd_end.minute;
  doc["ntp"] = Config::ntp;
  doc["DNS_name"] = Config::DNS_name;
  doc["viertel"] = Config::viertel;
  doc["ticker"] = Config::ticker;
  doc["color_tc_r"] = Config::color_tc.r;
  doc["color_tc_g"] = Config::color_tc.g;
  doc["color_tc_b"] = Config::color_tc.b;
  doc["tspeed"] = Config::tspeed;
  doc["tu"] = Config::tu;
  doc["WX"] = Config::WX;
  doc["WXcommand"] = Config::WXcommand;
  doc["apikey"] = Config::apikey;
  doc["lon"] = Config::lon;
  doc["lat"] = Config::lat;


  serializeJson(doc, file);
#ifdef DEBUG
  serializeJsonPretty(doc, Serial);
#endif
  file.close();
}

void Config::load() {
  //default black
  Config::color_bg.r = 0;
  Config::color_bg.g = 0;
  Config::color_bg.b = 0;

  //default white
  Config::color_fg.r = 255;
  Config::color_fg.g = 255;
  Config::color_fg.b = 255;

  //single minute 1
  Config::color_m1.r = 255;
  Config::color_m1.g = 255;
  Config::color_m1.b = 255;
  //single minute 2
  Config::color_m2.r = 255;
  Config::color_m2.g = 255;
  Config::color_m2.b = 255;
  //single minute 3
  Config::color_m3.r = 255;
  Config::color_m3.g = 255;
  Config::color_m3.b = 255;
  //single minute 4
  Config::color_m4.r = 255;
  Config::color_m4.g = 255;
  Config::color_m4.b = 255;

  //default green
  Config::color_mx.r = 0;
  Config::color_mx.g = 155;
  Config::color_mx.b = 0;
  Config::mlength = 10; //matrix vertical length
  Config::atime = 30; //animation time delay

  Config::power_supply = 500; // default: 500mA (USB 2.0 specification)
  Config::brightness = 0.5;
  Config::animation_bell = 0; //quarter animation
  Config::animation_type = "{}";
  Config::automatic_timezone = true;
  Config::timezone = 0;

  Config::dnd_active = false;
  Config::dnd_start.hour = -1;
  Config::dnd_start.minute = -1;
  Config::dnd_end.hour = -1;
  Config::dnd_end.minute = -1;
  Config::ntp = "ptbtime1.ptb.de"; //pool.ntp.org
  Config::DNS_name = "wordclock";
  Config::ticker = "Hallo";
  //default white
  Config::color_tc.r = 255;
  Config::color_tc.g = 255;
  Config::color_tc.b = 255;
  Config::tspeed = 100;
  Config::tu = 0;
  Config::viertel = true;
  Config::WX = 0;
  Config::WXcommand = "$TC";
  Config::apikey = "";
  Config::lon = "8";
  Config::lat = "48";

  File file = SPIFFS.open("/wordclock_config.json", "r");

  if(!file) {
    Serial.println("Failed to open config file. Will try to make");
    File file = SPIFFS.open("/wordclock_config.json", "w");

    if (!file) {
        Serial.println("Can't open wordclock_config.json for writing");
        //return;
    }
    Config::save();
    return;
  }else
     Serial.println("Load config.");
  
  StaticJsonDocument<1530> doc;
  StaticJsonDocument<256> animationBuff;
  deserializeJson(doc, file);
#ifdef DEBUG
  serializeJsonPretty(doc, Serial);
#endif

  Config::color_bg.r = doc["color_bg_r"].as<int>();
  Config::color_bg.g = doc["color_bg_g"].as<int>();
  Config::color_bg.b = doc["color_bg_b"].as<int>();

  Config::color_fg.r = doc["color_fg_r"].as<int>();
  Config::color_fg.g = doc["color_fg_g"].as<int>();
  Config::color_fg.b = doc["color_fg_b"].as<int>();

  Config::color_mx.r = doc["color_mx_r"].as<int>();
  Config::color_mx.g = doc["color_mx_g"].as<int>();
  Config::color_mx.b = doc["color_mx_b"].as<int>();

  Config::color_m1.r = doc["color_m1_r"].as<int>();
  Config::color_m1.g = doc["color_m1_g"].as<int>();
  Config::color_m1.b = doc["color_m1_b"].as<int>();

  Config::color_m2.r = doc["color_m2_r"].as<int>();
  Config::color_m2.g = doc["color_m2_g"].as<int>();
  Config::color_m2.b = doc["color_m2_b"].as<int>();

  Config::color_m3.r = doc["color_m3_r"].as<int>();
  Config::color_m3.g = doc["color_m3_g"].as<int>();
  Config::color_m3.b = doc["color_m3_b"].as<int>();

  Config::color_m4.r = doc["color_m4_r"].as<int>();
  Config::color_m4.g = doc["color_m4_g"].as<int>();
  Config::color_m4.b = doc["color_m4_b"].as<int>();


  Config::mlength = doc["mlength"].as<int>();
  if (doc["atime"])
      Config::atime = doc["atime"].as<int>();
  if(doc["power_supply"]) {
    Config::power_supply = doc["power_supply"].as<int>();
  }

  if(doc["brightness"]) {
    Config::brightness =
      (doc["brightness"].as<double>() > Led::getMaxBrightnessPercnt()) ? Led::getMaxBrightnessPercnt() : doc["brightness"].as<double>();
  }

  Config::animation_bell = doc["animation_bell"].as<int>();
  Config::animation_type = doc["animation_type"].as<String>();
  deserializeJson(animationBuff, Config::animation_type);
  Config::animation_set = 0;
  int i = 0;
  while ((int)animationBuff[i] != 0 && i < 30) { // just in case if garbage i < 30
      Config::animation_set = (int)animationBuff[i]; // to check if animation / DEMO highest selection #
      Config::animation[i] = (int)animationBuff[i];
      i++;
  }
  i--;
  Config::animation[LENGTH] = i; // how many elements selected
#ifdef DEBUG
  Serial.print("Config Len:");
  Serial.println(Config::animation[LENGTH]);
#endif

  if (doc["tz_auto"]) {
    Config::automatic_timezone = doc["tz_auto"].as<bool>();
  }

  // load the last known timezone in any case.
  Config::timezone = doc["timezone"].as<int>();

  if (Config::automatic_timezone) {
    UtcOffset::updateLocalizedUtcOffset();
  }
  if (doc["dnd_active"]) 
     Config::dnd_active = doc["dnd_active"].as<bool>();
  Config::dnd_start.hour = doc["dnd_start_hour"].as<int>();
  Config::dnd_start.minute = doc["dnd_start_minute"].as<int>();
  Config::dnd_end.hour = doc["dnd_end_hour"].as<int>();
  Config::dnd_end.minute = doc["dnd_end_minute"].as<int>();


  if(doc["ntp"]) {
    Config::ntp = doc["ntp"].as<String>();
  }

  if (doc["DNS_name"]) {
      Config::DNS_name = doc["DNS_name"].as<String>();
  }
  
  Config::viertel = doc["viertel"].as<bool>();
  if (doc["ticker"]) {
      Config::ticker = doc["ticker"].as<String>();
  }

  Config::color_tc.r = doc["color_tc_r"].as<int>();
  Config::color_tc.g = doc["color_tc_g"].as<int>();
  Config::color_tc.b = doc["color_tc_b"].as<int>();
  Config::tspeed = doc["tspeed"].as<int>();
  Config::tu = doc["tu"].as<int>();


  Config::WX = doc["WX"].as<int>();
  if (doc["WXcommand"]) 
    Config::WXcommand = doc["WXcommand"].as<String>();
  if (doc["apikey"])
    Config::apikey = doc["apikey"].as<String>();
  if (doc["lon"])
    Config::lon = doc["lon"].as<String>();
  if (doc["lat"])
    Config::lat = doc["lat"].as<String>();

  Time::ntpClient.setPoolServerName(Config::ntp.c_str());
  Time::ntpClient.setTimeOffset(Config::timezone);

 

  file.close();
}

color_t Config::color_bg{};
color_t Config::color_fg{};
color_t Config::color_mx{};
color_t Config::color_m1{};
color_t Config::color_m2{};
color_t Config::color_m3{};
color_t Config::color_m4{};
int Config::mlength{};
int Config::atime{};
int Config::power_supply{};
double Config::brightness{};
bool Config::automatic_timezone{};
int Config::timezone{};
bool Config::dnd_active{};
clock_time_t Config::dnd_start{};
clock_time_t Config::dnd_end{};
String Config::ntp{};
int Config::animation_bell{};
String Config::animation_type{};
int Config::animation[32];
int Config::animation_set;
String Config::DNS_name{};
String Config::ticker{};
color_t Config::color_tc{};
int Config::tspeed{};
int Config::tu{};
bool  Config::viertel{};
int Config::last_element;
int Config::WX{};
String Config::WXcommand{};
String Config::apikey{};
String Config::lon{};
String Config::lat{};


