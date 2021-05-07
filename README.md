"# MazukClock" 
A wordclock based on the  original project from Panbachi
https://www.panbachi.de/eigene-word-clock-bauen/#more-441
The original has been changed for many addons like
11 animations during the time change.
Add and detecting the use of an RTC module
Ticker function
OpenWeatherMap for weather data as ticker
A new GUI wich allows all configurations

List of used libraries:
ArduinoJson by Benoit Blanchon v6.17.3
esp8266_mdns by mrdunk v1.1.7
FastLED by Daniel Garcia v3.4.0
NTPClient by Fabrice Weinberg v3.2.0
RTClib by Adafruit v1.13.0
Time by Michael Margolis v1.6.0
WiFiManager by tyapu,tablatronix v2.0.3-alpha
WiFi by Arduino v1.2.7

A lot of the additional software parts are from the WWW and have been modified to adoped the WordClock
6.April 2021
V2.62 initial commit
7.April 2021
V2.63 Drop animation uses the correct color for the old time. (Rainbow animation)
14.April 2021
V2.64 Add a filled heart access in ticker with $f $F not filled $h $H
16.April 2021
V2.65 Add weekday $W and year $J to ticker. Ticker extended by 1 to pass fully
V2.66 Bell in colors
17.April 2021
V2.67 Fix utf8 conversion text shifting
18.April 2021
V2.68 In case of no data from open Weather just display the last successful received data
20.April 2021
V2.69 Changed order of displaying the weather ticker. Wait first for the response from OpenWeatherMap before clearing the display
V2.70 Weather update in case of no response use old data only if not older than 10 min / 600sec
23.April 2021
V2.71 Bug fix in end of day countdown
6.May 2021
V2.72 Fixed in case of only 10 row no minute display. set config GRID_SINGLE_MINUTES 2 not used
Fixed bug when no RTC connected to get date from NTP
V2.73 add to config to enable the GRAD letters USE_GRAD 
V2.74 fix also not using the minute 4 for the last 5 sec blinking before change when no GRID_SINGLE_MINUTES
7.May 2021
V2.75 FIX for 10 rows matrix
V2.76 add random date to DEMO mode. Clear remaining chr in ticker due UTF conversion