#ifndef WORDCLOCK_TIME_H
#define WORDCLOCK_TIME_H

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <RTClib.h>

class Time {
  public:
    static int hour;
    static int minute;
    static int second;
    static int Year;
    static int Month;
    static int Day;
    //static String weekDays[];
    //static String months[];
    static String MonthName;
    static String weekDay;
    static bool RTCdisable;
    static WiFiUDP udp;
    static NTPClient ntpClient;
    static RTC_DS3231 rtc;
    static DateTime now;

    static void setup();

    static void loop();
};

#endif
