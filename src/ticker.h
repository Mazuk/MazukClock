#include <FastLED.h>

class ticker {
public:
    static void ShowIP();
    static void DisplayIP(int speed);
    static void Numbers(int i1);
    static void DisplayTicker(String textIN, int speed, CRGB myColor);
    static void Utf8Filter(char* text);
    static String TickerCommand(char* text);
};