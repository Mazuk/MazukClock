#include <ESP8266WiFi.h>

#include "ticker.h"
#include "wifi.h"
#include "config.h"
#include "led.h"
#include "animation.h"
#include "font.h"
#include "time.h"

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Show the assigned IP address
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void ticker::ShowIP() {
    static int i = 0, ii = 0;
    CRGB color;
    char buf[20];
    sprintf(buf, "IP:%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);

    // Shift all pixels to left
    for (int b = 0; b < 10; b++) {
        for (int a = 0; a < GRID_ROWS; a++) {
            color = Led::ids[Led::getLedId(animation::matrix[a][b + 1])];
            Led::ids[Led::getLedId(animation::matrix[a][b])] = color;
        }
    }

    if (i < 5) {
        for (int h = 0; h < 8; h++) {
            if (font_7x5[buf[ii]][i] & (1 << h)) {
                //fixed color in case config is set to black 0
                Led::ids[Led::getLedId(animation::matrix[h + 1][10])].setRGB(200, 200, 200);
                //Led::ids[Led::getLedId(animation::matrix[h+1][10])].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
            }
            else {
                Led::ids[Led::getLedId(animation::matrix[h + 1][10])].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
            }
        }
    }
    else {
        for (int h = 0; h < 8; h++) {
            Led::ids[Led::getLedId(animation::matrix[h + 1][10])].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
        }
    }
    FastLED.show();

    i++;
    if (i > 5) {
        i = 0;
        ii++;
        if (ii > strlen(buf)) { ii = 0; }
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Main call for showing the assigned IP address
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void ticker::DisplayIP(int speed) {
    animation::ClearDisplay();
    char buf[20];
    sprintf(buf, "IP:%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    int StringLength = strlen(buf) + 1;
    StringLength = StringLength * 6;
    for (int i = 0; i <= StringLength; i++) {
        ShowIP();
        delay(speed);
    }

}
// ---- ENDE Print the IP address

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Display the text of the ticker
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void ticker::DisplayTicker(String textIN, int speed, CRGB myColor) {

    char text[55];
    for (int i = 0; i < 50; i++)
        text[i] = 0;
    strcpy(text, textIN.c_str());

    Utf8Filter(text);//remove UTF8 and change to ASCII extend
    strcpy(text, TickerCommand(text).c_str()); //replace variable with data
    int i = 0, ii = 0;
    CRGB color;
    int StringLength = strlen(text) + 2; // to scroll till end
    for (int j = 0; j <= StringLength * 6; j++) {
        // shift all pixels to left
        for (int b = 0; b < 10; b++) {
            for (int a = 0; a < 10; a++) {// don't shift the bottom row
                color = Led::ids[Led::getLedId(animation::matrix[a][b + 1])];
                Led::ids[Led::getLedId(animation::matrix[a][b])] = color;
            }
        }

        if (i < 5) {
            for (int h = 0; h < 8; h++) {
                if (font_7x5[text[ii]][i] & (1 << h)) {
                    Led::ids[Led::getLedId(animation::matrix[h + 1][10])].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
                    Led::ids[Led::getLedId(animation::matrix[h + 1][10])] = myColor;
                }
                else {
                    Led::ids[Led::getLedId(animation::matrix[h + 1][10])].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
                }
            }
        }
        else {
            for (int h = 0; h < 8; h++) {
                Led::ids[Led::getLedId(animation::matrix[h + 1][10])].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
            }
        }
        FastLED.show();

        i++;
        if (i > 5) {
            i = 0;
            ii++;
            if (ii > StringLength) { ii = 0; }
        }
        delay(speed);
    }


}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Filter / convert UTF8 char to ASCII extended
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void ticker::Utf8Filter(char* text) {
    int i = 0;
    int j = 0;
    while (text[i] != 0) {
        if ((unsigned char)text[i] <= 127)
        {
            text[j] = text[i];
            i++;
        }
        else
        {
            unsigned char c1 = text[i];
            unsigned char c2 = text[i + 1];
            text[j] = ((c1 & 0x1f) << 6) + (c2 & 0x3f);
            i += 2;
        }
        j++;
    }
    text[j] = 0x00;
    text[j + 1] = 0x00; //two additional scrolls in ticker
}
/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Ticker variable parser and replace var with content
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
String ticker::TickerCommand(char* text) {
    String content;
    int i = 0;
    while (text[i] != 0) {
        if (text[i] == '$') {
            if (toupper(text[i + 1]) == 'D') { //Date
                content += String(Time::Day) + "." + Time::MonthName;
                i++;
            }
            else if (toupper(text[i + 1]) == 'W') { //WeekDay
                content += Time::weekDay;
                i++;
            }
            else if (toupper(text[i + 1]) == 'J') { //Year
                content += String(Time::Year);
                i++;
            }
            else if (text[i + 1] == 'h') { //Heart)
                content += "\x95";
                i++;
            }
            else if (text[i + 1] == 'H') { //Big Heart)
                content += "\x96";
                i++;
            }
            else if (text[i + 1] == 'f') { //Heart filled)
                content += "\x97";
                i++;
            }
            else if (text[i + 1] == 'F') { //Big Heart filled)
                content += "\x98";
                i++;
            }
        }
        else
            content += text[i];
        i++;
    }
    return content;
}


/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Display a static number max two digits
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void ticker::Numbers(int i1) {
    char number[] = { 48,49,50,51,52,53,54,55,56,57 }; //ASCII number for 0-9
    animation::ClearDisplay();

    for (int i = 0; i < 5; i++) {
        for (int h = 0; h < 8; h++) {
            // 1. Number center align
            if (i1 < 10) {
                if (font_7x5[number[i1]][i] & (1 << h)) {
                    Led::ids[Led::getLedId(animation::matrix[h + 1][i + 3])].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
                }
            }
            else {
                // 1. Number left align
                if (font_7x5[number[i1 / 10]][i] & (1 << h)) {
                    Led::ids[Led::getLedId(animation::matrix[h + 1][i])].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
                }
                // 2. Number right align
                if (font_7x5[number[i1 % 10]][i] & (1 << h)) {
                    Led::ids[Led::getLedId(animation::matrix[h + 1][i + 6])].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
                }
            }
        }
    }
    FastLED.show();
}