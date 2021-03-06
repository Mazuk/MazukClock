#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <RTClib.h>

#include "time.h"
#include "dnd.h"
#include "grid.h"
#include "utcOffset.h"
#include "led.h"

#include "animation.h"
#include "ticker.h"

int Time::hour = -1;
int Time::minute = -1;
int Time::second = -1;
bool Time::RTCdisable = false;

WiFiUDP Time::udp;
NTPClient Time::ntpClient = NTPClient(Time::udp);
RTC_DS3231 Time::rtc;
DateTime Time::now = Time::rtc.now();
//Week Days
String weekDays[7] = { "Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag" };
//Month names
String months[12] = { "Januar", "Februar", "M?rz", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember" };
int Time::Year;
int Time::Month;
int Time::Day;
String Time::MonthName;
String Time::weekDay;

void Time::setup() {
  Time::ntpClient.begin();
  if (!Time::ntpClient.update())
      Serial.println("No NTP update at this time!");
  //check if RTC is available and valid
  if (!rtc.begin()) {
      Serial.println("Couldn't find RTC");
      Time::RTCdisable = true;
  }
  else if (rtc.lostPower()) {
      Serial.println("RTC lost power, lets set the time!");
      // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  else {
      DateTime now = rtc.now();
      char buf[30];
      sprintf(buf, "RTC initialized! Time:  %02d:%02d:%02d", now.hour(), now.minute(), now.second());
      Serial.println(buf);
      Time::RTCdisable = false;
  }
 
}

void Time::loop() {
    int static h;
    int static m;
    int static s = 5;
    int static NTPdisable = true;
    unsigned long  now = millis();
    unsigned long static lastTime;
    unsigned long epochTime;
    // no real time when Demo Mode. Random time increase every 2 sec the minute till next change
    if (Config::animation_set == DEMO) {
       if (now - lastTime < 2000) // use millis to have better access to httpserver, not blocking loop!
            return;
        lastTime = now;
       
        
        if (s >= 5) {
            s = 0;
            m = 5 * (char)random(12); // only 5 min steps
            h = (char)random(12);
            Time::weekDay = weekDays[(char)random(7)];
            Time::Day = 1 + (char)random(30);
            Time::MonthName = months[(char)random(12)];
            Time::Year = 2000 + (char)random(100);
        }
        Time::minute = m + s;
        Time::hour = h;
        s++;
        #ifdef DEBUGTIME
            Serial.print(Time::hour);
            Serial.print(":");
            Serial.println(Time::minute); 
            Serial.print(Time::weekDay);
            Serial.print(" ");
            Serial.print(Time::Day);
            Serial.print(". ");
            Serial.print(Time::MonthName);
            Serial.print(" ");
            Serial.println(Time::Year);
        #endif
        Grid::setTime(Time::hour, Time::minute);
        return;
    }
    
    if (Time::ntpClient.update()) {
        h = Time::ntpClient.getHours();
        m = Time::ntpClient.getMinutes();
        s = Time::ntpClient.getSeconds();
        epochTime = Time::ntpClient.getEpochTime();
        if (NTPdisable) {
            Serial.print("NTP use: ");
            Serial.println(Config::ntp);
        }
        NTPdisable = false;
    }
    //Use RTC time if no NTP time (no Wifi?)
    else if(!Time::RTCdisable) {
        DateTime now = rtc.now();
        h = now.hour();
        m = now.minute();
        s = now.second();
        if(!NTPdisable)
            Serial.println("RTC use");
        NTPdisable = true;   
    }else
        Serial.println("No time update!");
    //removed not to get jumping time in case of no NTP server available & no RTC
    //else { // set Time to 12:00 to indicate no NTP update
    //    h = 12;
    //    m = 0;
    //}

    //Count down at each day end
    if (m == 59 && h == 23 && !(DND::active(hour, minute) && Config::animation_set != DEMO)) {
        ticker::Numbers(60 - s);
    }
    else {//blink the last 5 sec the minute 4 led before changing time
        if (s > 53 && m % 5 == 4 && s != Time::second && !(DND::active(hour, minute) && Config::animation_set != DEMO)) {
            if (s % 2 == 0) {
                if (GRID_SINGLE_MINUTES == 0) {
                    Led::ids[3].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
                }
                else if (GRID_SINGLE_MINUTES == 1) {
                    Led::ids[NUM_LEDS - 4 + 3].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
                }
                else if (GRID_SINGLE_MINUTES == 2) {
                    Led::ids[8].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
                }
            }
            else {
                if (GRID_SINGLE_MINUTES == 0) {
                    Led::ids[3].setRGB(Config::color_m4.r, Config::color_m4.g, Config::color_m4.b);
                }
                else if (GRID_SINGLE_MINUTES == 1) {
                    Led::ids[NUM_LEDS - 4 + 3].setRGB(Config::color_m4.r, Config::color_m4.g, Config::color_m4.b);
                }
                else if (GRID_SINGLE_MINUTES == 2) {
                    Led::ids[8].setRGB(Config::color_m4.r, Config::color_m4.g, Config::color_m4.b);
                }
            }
            Time::second = s;
            FastLED.show();

        }
    }

  if((m != Time::minute) || animation::test_start) {
        if(m == 0 && h == Time::hour) {
            h = (h + 1) % 24;
        }
        //set sync RTC to NTP time every minute only if NTP is available
        if (!Time::RTCdisable && !NTPdisable) {
            rtc.adjust(DateTime(Time::ntpClient.getEpochTime()));
            #ifdef DEBUGTIME
                Serial.print("NTP Time: ");
                Serial.println(Time::ntpClient.getFormattedTime());
                DateTime now = rtc.now();
                char buf[25];
                sprintf(buf, "RTC Sync Time: %02d:%02d:%02d", now.hour(), now.minute(), now.second());
                Serial.println(buf);
            #endif
        }
        //Get a time structure
        Time::weekDay = weekDays[Time::ntpClient.getDay()];
        struct tm* ptm = gmtime((time_t*)&epochTime);

        Time::Day = ptm->tm_mday;
        Time::Month = ptm->tm_mon + 1;
        Time::MonthName = months[Month - 1];
        Time::Year = ptm->tm_year + 1900;
        Time::hour = h;
        Time::minute = m;
        #ifdef DEBUGTIME
            Serial.print("Month day: ");
            Serial.print(Time::Day);
            Serial.print(". ");   
            Serial.print("Month: ");
            Serial.println(Month); 
            Serial.print("Month name: ");
            Serial.print(Time::MonthName);
            Serial.print(" ");
            Serial.print("Year: ");
            Serial.println(Time::Year);
            char buf[15];
            sprintf(buf, "Time: %02d:%02d", Time::hour, Time::minute);
            Serial.println(buf);
        #endif
        Grid::setTime(Time::hour, Time::minute);
        
        if (Config::automatic_timezone && !NTPdisable) {
            UtcOffset::updateLocalizedUtcOffset();
            Time::ntpClient.setTimeOffset(Config::timezone);
        }
       
  }
}
