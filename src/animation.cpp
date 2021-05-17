#include <Arduino.h>
#include <FastLED.h>
#include <ArduinoJson.h>

#include "animation.h"
#include "grid.h"
#include "led.h"
#include "config.h"
#include "snake.h"
#include "ticker.h"
#include "weather.h"

// Define the array of leds
CRGB leds[NUM_LEDS]; // Defines individual addreaasable LED in the LED chain

// Define Led Matrix
int animation::ledmatrix[NUM_LEDS];
int animation::ledmatrix_old[NUM_LEDS];

int T1 = 0; // LED Powerup Sequence delay
bool animation::test_start = false;
bool animation::MovinRainbowSet = false;
bool animation::SnakeSet = false;

//Define each LED number
int animation::matrix[11][11] = {
{0,1,2,3,4,5,6,7,8,9,10},
{11,12,13,14,15,16,17,18,19,20,21},
{22,23,24,25,26,27,28,29,30,31,32},
{33,34,35,36,37,38,39,40,41,42,43},
{44,45,46,47,48,49,50,51,52,53,54},
{55,56,57,58,59,60,61,62,63,64,65},
{66,67,68,69,70,71,72,73,74,75,76},
{77,78,79,80,81,82,83,84,85,86,87},
{88,89,90,91,92,93,94,95,96,97,98},
{99,100,101,102,103,104,105,106,107,108,109},
{110,111,112,113,114,115,116,117,118,119,120}
};
/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Pre-load the Matrix with 0 values
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void animation::setup() {

    for (int i = 0; i < NUM_LEDS; i++) {
        Led::ids[i].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
        animation::ledmatrix[i] = 0;
        animation::ledmatrix_old[i] = 0;
    }
    ledtest();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Powerup sequence for unit
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void animation::ledtest() {
    FastLED.setBrightness(Config::brightness * 255);
    // Flash LED to show different color
    for (int x = 0; x < NUM_LEDS; x++) {
        Led::ids[x] = CRGB::Blue;
    }
    FastLED.show();
    delay(300);

    for (int x = 0; x < NUM_LEDS; x++) {
        Led::ids[x] = CRGB::Green;
    }
    FastLED.show();
    delay(300);

    for (int x = 0; x < NUM_LEDS; x++) {
        Led::ids[x] = CRGB::Red;
    }
    FastLED.show();
    delay(300);

    for (int x = 0; x < NUM_LEDS; x++) {
        Led::ids[x] = CRGB::Yellow;
    }
    FastLED.show();
    delay(300);

    for (int x = 0; x < NUM_LEDS; x++) {
        Led::ids[x] = CRGB::Magenta;
    }
    FastLED.show();
    delay(300);
    unsigned long  now = millis();
    unsigned long  lastTime = now;

    while (now - lastTime < 2000) {
        RainbowWave(100, 10);
        FastLED.show();
        now = millis();
    }
    for (int x = 0; x < NUM_LEDS; x++) {
        Led::ids[x] = CRGB::Black;
    }
    FastLED.show();
    delay(30);
    //show the assined IP address on the display
    // 
    ticker::DisplayIP(100); // 100ms delay speed
    animation::ClearDisplay();
    animation::test_start = true;
    // 
    //delay(100);
    //ticker::DisplayTicker(String(Config::ticker), 150, CRGB::Red);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
* animation selection
* 0 nothing
* 1 matrix
* 2 Rainbow display
* 3 cycle spirale in2out
* 4 cycle spirale out2in
* 5 Regenschauer
* 6 Typewriter
* 7 Buchstaben falles runter
* 8 langsames überbelden der Uhrzeit änderung
* 9 BuchstabenLotto random showing all only selected stay
* 10 Moving Rainbow letters
* 11 Snake animation eating the old time and shit the new
* 12 Demo Mode zufällige Uhrzeit alles2sec eine Minute
* *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void animation::animation_select(int ticker) {
    String WeatherString;
    switch (ticker) {
    case WEATHER:
        if (Config::WX == 0)//if disabled
            return;
        WeatherString = weather::GetWeather(); //continue to display time till a response from OpenWeatherMap
        animation::ClearDisplay();
        weather::SetGrad(true);//Switch GRAD on
        ticker::DisplayTicker(WeatherString, Config::tspeed, weather::GetTempColor(weather::temperature));
        weather::SetGrad(false);
        if (animation::MovinRainbowSet != true) {
            animation::SetDisplay();
        }
        return;
        break;
    case TICKER:
        if (Config::tu == 0)//if disabled
            return;
        animation::ClearDisplay();
        ticker::DisplayTicker(String(Config::ticker), Config::tspeed, CRGB(Config::color_tc.r, Config::color_tc.g, Config::color_tc.b));
        if (animation::MovinRainbowSet != true) {
            animation::SetDisplay();
            FastLED.show();
            delay(1000);
        }
        return;
        break;
    }

    int ani = 0;
    int length;

    length = Config::animation[LENGTH];

    if (length > 0) {
        if (Config::animation_set != DEMO)
            length += 1;
        ani = (rand() % length);
    }
    animation::MovinRainbowSet = false;
    switch (Config::animation[ani]) {
    case 1:
        //Matrixeffect(); //old Matrix style
        Matrixeffect2();
        break;
    case 2:
        RainbowDisplay();
        break;
    case 3:
        CircleDisplay(1); //Cycle IN
        break;
    case 4:
        CircleDisplay(0); //Cyle OUT
        break;
    case 5:
        RainDisplay();
        break;
    case 6:
        TypeWriter();
        break;
    case 7:
        Dropeffect();
        break;
    case 8:
        Fadeeffect();
        break;
    case 9:
        RandomLetters();
        break;
    case 10:
        animation::MovinRainbowSet = true;
        animation::ClearDisplay();
        MovingRainbow();
        break;
    case 11:
        Snake::StartSnake();
        break;
    case DEMO:
        //DemoMode;
        break;
    default:
        CircleDisplay(1);
    }


}

//loop to have a moving rainbow update
void animation::loop() {
    if (animation::MovinRainbowSet == true) {
        animation::MovingRainbow();
        FastLED.show();
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Cycle animation out to inner or reverse 0=out2in 1=in2out
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void animation::CircleDisplay(bool dir) { // Running LED chaser from inside to out

    char LED[] = { 60,49,50,61,72,71,70,59,48,37,38,39,40,51,62,73,84,83,82,81,80,69,58,47,36,25,26,27,28,29,30,41,52,63,74,85,96,95,94,93,92,91,90,79,68,57,46,35,24,13,14,15,16,17,18,19,20,
    31,42,53,64,75,86,97,108,107,106,105,104,103,102,101,100,89,78,67,56,45,34,23,12,1,2,3,4,5,6,7,8,9,10,21,32,43,54,65,76,87,98,109,120,119,118,117,116,115,114,113,112,111,110,99,88,77,66,55,44,33,22,11,0 };
    
    char LED10[] = { 60,49,50,61,72,71,70,59,48,37,38,39,40,51,62,73,84,83,82,81,80,69,58,47,36,25,26,27,28,29,30,41,52,63,74,85,96,95,94,93,92,91,90,79,68,57,46,35,24,13,14,15,16,17,18,19,20,
    31,42,53,64,75,86,97,108,107,106,105,104,103,102,101,100,89,78,67,56,45,34,23,12,1,2,3,4,5,6,7,8,9,10,21,32,43,54,65,76,87,98,109,99,88,77,66,55,44,33,22,11,0 };
    if (GRID_ROWS == 10) {
        for (int i = 0; i < NUM_LEDS - 4; i++)
            LED[i] = LED10[i];
    }
    CRGB color[] = { CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green, CRGB::Blue, CRGB::Indigo, CRGB::Violet };
    char idx;
    int j;
    int fade = 256 - (Config::atime * 2);
    for (int i = 0; i < NUM_LEDS - 4; i++) {
        if (dir) {
            idx = LED[i];
        }
        else
            idx = LED[NUM_LEDS - 4 - i - 1]; //out2in reverse the array scan -1 because 0-120
        //Led::ids[Led::getLedId(idx)].setRGB(Config::color_mx.r, Config::color_mx.g, Config::color_mx.b);
        Led::ids[Led::getLedId(idx)] = color[i % 7];

        for (j = 0; j < i; j++) {
            if (dir) {
                if (animation::ledmatrix[Led::getLedId(LED[j])] == 0)
                    Led::ids[Led::getLedId(LED[j])].fadeToBlackBy(fade);
            }
            else {
                if (animation::ledmatrix[Led::getLedId(LED[NUM_LEDS  - 4 - j - 1])] == 0)
                    Led::ids[Led::getLedId(LED[NUM_LEDS - 4 - j - 1])].fadeToBlackBy(fade);
            }
        }

        FastLED.show();
        delay(Config::atime);      // Set delay between typing out time characters
        if (animation::ledmatrix[Led::getLedId(idx)] == 1) {
            Led::ids[Led::getLedId(idx)].maximizeBrightness();
            Led::ids[Led::getLedId(idx)].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
        }
        FastLED.show();
        delay(5);
    }

    //fade all out after reaching the end
    for (int k = 0; k < Config::atime / 5; k++) {
        for (int j = 0; j < NUM_LEDS - 4; j++) {
            if (dir) {
                if (animation::ledmatrix[Led::getLedId(LED[j])] == 0)
                    Led::ids[Led::getLedId(LED[j])].fadeToBlackBy(fade);
            }
            else
            {
                if (animation::ledmatrix[Led::getLedId(LED[NUM_LEDS - 4 - j - 1])] == 0)
                    Led::ids[Led::getLedId(LED[NUM_LEDS - 4 - j - 1])].fadeToBlackBy(fade);
            }
        }
        FastLED.show();
        delay(Config::atime / 3);
    }

    SetDisplay(); //needed in case different background color
    delay(200);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Matrix effect
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void animation::Matrixeffect() {
    int T30 = Config::atime - 30;
    if (T30 < 0)
        T30 = 5;
    int w = 0; // temp variable
    int x = 0;
    int usedMatrix[NUM_LEDS];
    // Generate random delay speeds for each falling charater in columns
    int dly1[11] = { random(20), random(20), random(20), random(20), random(20), random(20), random(20), random(20), random(20), random(20), random(20) };
    int ptr[11] = { 0,0,0,0,0,0,0,0,0,0,0 }; // Points to the LED in each column we need to deal with

    for (int t = 0; t < Config::mlength * 20; t++) { // Counter for completing all changes for 11 rows

        for (int c = 0; c < GRID_COLS; c++) { // Count through columns and check timer not exceeded
            if ((t > (dly1[c] * (ptr[c] + 1))) && ptr[c] < GRID_ROWS) { // If timer exceeded then erase current value and draw a white curosr in this position based on random time period     
                // Write over the previous value
                // Calculate the LED value from the Column and ptr value
                w = Led::getLedId(c + GRID_COLS * ptr[c]); // Calculate the LED value in the 11 x 11 Matrix

                if (animation::ledmatrix[w] == 1) { // If the bit set in LED Matrix then leave FG color
                    Led::ids[w].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b); //foreground color
                }
                else {
                    Led::ids[w].setRGB(Config::color_mx.r, Config::color_mx.g, Config::color_mx.b); //matrix color
                }
                ptr[c]++; // Increment row and print White value    
                if (ptr[c] < GRID_ROWS) {
                    // Calculate the LED value from the Column and ptr value for leading white letter
                    w = Led::getLedId(c + GRID_COLS * ptr[c]); // Calculate the LED value in the 11 x 11 Matrix
                    Led::ids[w].setRGB(Config::color_mx.r, Config::color_mx.g, Config::color_mx.b); //matrix color  
                }
                FastLED.show();
                delay(T30); // need to avoid flickering  
            }
        }
    }

    // Now clear the screen
    int ptr2[11] = { 0,0,0,0,0,0,0,0,0,0,0 }; // Points to the LED in each column we need to deal with

    for (int t = 0; t < Config::mlength * 20; t++) { // Counter for completing all changes for 10 rows
        for (int c = 0; c < GRID_COLS; c++) { // Count through columns and check timer not exceeded
            if ((t > (dly1[c] * (ptr2[c] + 1))) && ptr2[c] < GRID_ROWS) { // If timer exceeded then erase current value and draw a white curosr in this position based on random time period     
                // Write over the previous value
                // Calculate the LED value from the Column and ptr value
                w = Led::getLedId(c + GRID_COLS * ptr2[c]); // Calculate the LED value in the 11 x 11 Matrix

                if (animation::ledmatrix[w] == 1) { // If the bit set in LED Matrix then leave FG color
                    Led::ids[w].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
                }
                else {
                    Led::ids[w].setRGB(Config::color_mx.r, Config::color_mx.g, Config::color_mx.b); // matrix color
                    usedMatrix[x++] = w; //for fade out only the matrix leds
                }

                ptr2[c]++; // Increment row and print White value    
                if (ptr2[c] < GRID_ROWS) {
                    // Calculate the LED value from the Column and ptr value
                    w = Led::getLedId(c + GRID_COLS * ptr2[c]); // Calculate the LED value in the 11 x 11 Matrix
                    Led::ids[w].setRGB(Config::color_mx.r, Config::color_mx.g, Config::color_mx.b); // matrix color                  
                }
                FastLED.show();
                delay(T30);
            }
        }
        //fade to black all used matrix LEDs
        for (int k = 0; k < x; k++) {
            Led::ids[usedMatrix[k]].fadeToBlackBy(80);
        }
    }
    SetDisplay(); //needed in case different background color
}


/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Matrix2 more realistic effect
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void animation::Matrixeffect2() {
    int T30 = Config::atime - 50;
    int len = Config::mlength;
    if (len == 0)
        len = 5;
    if (T30 < 0)
        T30 = 5;
    int w = 0; // temp variable
    int x = 0;
    int usedMatrix[NUM_LEDS];
    CRGB colorM;
    CRGB color[] = { CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green, CRGB::Blue, CRGB::Indigo, CRGB::Violet };
    // Generate random delay speeds for each falling charater in columns
    int dly1[11] = { random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10), random(10) };
    // Generate random length min 2

    int lendly[11] = { random(len) + 1, random(len) + 1, random(len) + 1, random(len) + 1, random(len) + 1, random(len) + 1, random(len) + 1, random(len) + 1, random(len) + 1, random(len) + 1, random(len) + 1 };
    int ptr[11] = { 0,0,0,0,0,0,0,0,0,0,0 }; // Points to the LED in each column we need to deal with
    //generate a random column number
    int randompool[11] = { 5,1,9,3,0,2,8,4,7,6,10 };
    int randomcolumns[11];
    for (int r = (GRID_COLS - 1); r >= 0; r--) {
        int ran = rand() % (r + 1);
        randomcolumns[r] = randompool[ran];
        for (int i = ran; i < r; ++i) {
            randompool[i] = randompool[i + 1];
        }
    }

    for (int t = 0; t < 150; t++) { // Counter for completing all changes for 11 rows
        for (int i = 0; i < GRID_COLS; i++) { // Count through columns and check timer not exceeded
            int c = randomcolumns[i];

            if (t > (dly1[c] * ptr[c])) { // If timer exceeded then erase current value and draw a white curosr in this position based on random time period     
                if (ptr[c] < GRID_ROWS) {
                    w = Led::getLedId(c + GRID_COLS * ptr[c]); // Calculate the LED value in the 11 x 11 Matrix
                    unsigned char dim = (i * c) + 100;
                    if (Config::mlength == 0)
                        Led::ids[w] = color[i % 7];
                    else
                        Led::ids[w].setRGB(Config::color_mx.r, Config::color_mx.g, Config::color_mx.b); //matrix color

                    Led::ids[w].fadeToBlackBy(dim);
                    int cd = ptr[c] - lendly[c];
                    if (cd >= 0 && cd < GRID_ROWS) {
                        w = Led::getLedId(c + GRID_COLS * cd); // Calculate the LED value in the 11 x 11 Matrix
                        if (animation::ledmatrix[w] == 1) { // If the bit set in LED Matrix then leave FG color
                            Led::ids[w].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b); //foreground color
                        }
                        //fade out the end
                        for (int j = cd; j >= 0; j--) {
                            if (animation::ledmatrix[Led::getLedId(c + GRID_COLS * j)] == 0)
                                Led::ids[Led::getLedId(c + GRID_COLS * j)].fadeToBlackBy(200);
                        }
                    }
                }
                else {
                    int cd = ptr[c] - (lendly[c] + 3);
                    if (cd < GRID_ROWS) {
                        if (animation::ledmatrix[Led::getLedId(c + GRID_COLS * cd)] == 0) {
                            colorM = Led::ids[Led::getLedId(c + GRID_COLS * cd)];
                            Led::ids[Led::getLedId(c + GRID_COLS * cd)].setRGB(0, 0, 0);
                        }
                        cd++; // next row
                        if (cd == GRID_ROWS)
                            cd--;
                        if (animation::ledmatrix[Led::getLedId(c + GRID_COLS * cd)] == 0)
                            Led::ids[Led::getLedId(c + GRID_COLS * cd)] = colorM;
                        else
                            Led::ids[Led::getLedId(c + GRID_COLS * cd)].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b); //foreground color
                    }
                }

                ptr[c]++; // Increment row 
                FastLED.show();
                delay(T30);
            }
        }
    }
    SetDisplay(); //needed in case different background color
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Fade effect old fade away new fade in
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void animation::Fadeeffect() {
    //Set the minute LEDs also to fade out
    if (GRID_SINGLE_MINUTES == 0) {
        for (int idx = 0; idx < 4; idx++)
            animation::ledmatrix_old[idx] = 1;
    }
    else if (GRID_SINGLE_MINUTES == 1) {
        for (int idx = NUM_LEDS - 4; idx < NUM_LEDS; idx++)
            animation::ledmatrix_old[idx] = 1;
    }
    else if (GRID_SINGLE_MINUTES == 2) {
        for (int idx = 2; idx <= 8; idx += 2)
            animation::ledmatrix_old[idx] = 1;
    }
    for (int i = 0; i <= 255; i++) { //fade steps
        for (int idx = 0; idx < NUM_LEDS; idx++) {
            if (animation::ledmatrix_old[idx] != animation::ledmatrix[idx]) { // do fade only on different selections
                //0: before, led 0,1,2,3
                if (GRID_SINGLE_MINUTES == 0) {
                    //select the right color also for the minute fade out
                    switch (idx) {
                    case 0:
                        Led::ids[idx].setRGB(Config::color_m1.r, Config::color_m1.g, Config::color_m1.b);
                        break;
                    case 1:
                        Led::ids[idx].setRGB(Config::color_m2.r, Config::color_m2.g, Config::color_m2.b);
                        break;
                    case 2:
                        Led::ids[idx].setRGB(Config::color_m3.r, Config::color_m3.g, Config::color_m3.b);
                        break;
                    case 3:
                        Led::ids[idx].setRGB(Config::color_m4.r, Config::color_m4.g, Config::color_m4.b);
                        break;
                    default:
                        Led::ids[idx].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
                    }
                }
                //1: after, led GRID_ROWS * GRID_COLS + 1,2,3,4
                else if (GRID_SINGLE_MINUTES == 1) {
                    //select the right color also for the minute fade out
                    switch (idx) {
                    case NUM_LEDS - 4:
                        Led::ids[NUM_LEDS - 4].setRGB(Config::color_m1.r, Config::color_m1.g, Config::color_m1.b);
                        break;
                    case NUM_LEDS - 3:
                        Led::ids[NUM_LEDS - 3].setRGB(Config::color_m2.r, Config::color_m2.g, Config::color_m2.b);
                        break;
                    case NUM_LEDS - 2:
                        Led::ids[NUM_LEDS - 2].setRGB(Config::color_m3.r, Config::color_m3.g, Config::color_m3.b);
                        break;
                    case NUM_LEDS - 1:
                        Led::ids[NUM_LEDS - 1].setRGB(Config::color_m4.r, Config::color_m4.g, Config::color_m4.b);
                        break;
                    default:
                        Led::ids[idx].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
                    }
                }
                //2: new grid LED 2,4,6,8 
                else if (GRID_SINGLE_MINUTES == 2) {
                    //select the right color also for the minute fade out
                    switch (idx) {
                    case 2:
                        Led::ids[idx].setRGB(Config::color_m1.r, Config::color_m1.g, Config::color_m1.b);
                        break;
                    case 4:
                        Led::ids[idx].setRGB(Config::color_m2.r, Config::color_m2.g, Config::color_m2.b);
                        break;
                    case 6:
                        Led::ids[idx].setRGB(Config::color_m3.r, Config::color_m3.g, Config::color_m3.b);
                        break;
                    case 8:
                        Led::ids[idx].setRGB(Config::color_m4.r, Config::color_m4.g, Config::color_m4.b);
                        break;
                    default:
                        Led::ids[idx].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
                    }
                } else
                    Led::ids[idx].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);

                //fade old out
                if (animation::ledmatrix_old[idx] == 1) {
                    Led::ids[idx].nscale8_video(255 - i);
                }
                //fade new in
                if (animation::ledmatrix[idx] == 1) {
                    Led::ids[idx].nscale8_video(i * i / 255);// none linear, slower fade in
                }
            }
        }
        FastLED.show();
        delay(10);
    }
    SetDisplay(); //needed in case different background color
    return;
}
/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Drop effect where characters fall from the sky randomly and then settle in time letters
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void animation::Dropeffect() {

    int w = 0; // calculated LED ID variable
    int t = 0;
    int R = 15; // random range
    int ran;
    CRGB color;
    //define random column order
    int randompool[GRID_COLS + 1];

    // First drop old matrix letters
    // Generate random delay speeds for each falling charater in columns
    int dly[11] = { (char)random(R), (char)random(R), (char)random(R), (char)random(R), (char)random(R), (char)random(R), (char)random(R), (char)random(R), (char)random(R), (char)random(R), (char)random(R) };
    int ptr[11] = { 0,0,0,0,0,0,0,0,0,0,0 }; // Points to the LED in each column we need to deal with

    for (t = 0; t < (R * GRID_COLS); t++) { // Counter for completing all changes
        //fill column pool 
        for (int i = 0; i <= GRID_COLS; i++) {
            randompool[i] = i;
        }
        for (int r = (GRID_COLS - 1); r >= 0; r--) { // Count through columns and check timer not exceeded
            //generate a random column number
            ran = rand() % (r + 1);
            int c = randompool[ran];
            for (int i = ran; i < r; ++i) {
                randompool[i] = randompool[i + 1];
            }

            if ((t > (dly[c] * (ptr[c] + 1))) && ptr[c] < GRID_ROWS) { // If timer exceeded then erase current value and draw a white curosr in this position based on random time period     
                // Write over the previous value
                // Calculate the LED value from the Column and ptr value
                w = Led::getLedId(c + GRID_COLS * ptr[c]); // Calculate the LED value in the 11 x 11 Matrix

                if (animation::ledmatrix_old[w] == 1) { //&& animation::ledmatrix[w] == 0) { // drop only changed letters
                    color = Led::ids[w]; //save the present color if rainbow to drop the correct color
                    Led::ids[w].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2); //background color
                    //animation::ledmatrix_old[w] = 0;
                    FastLED.show();
                    delay(Config::atime / 2);
                }
                ptr[c]++; // Increment row and print FG color value      
                if (ptr[c] < GRID_ROWS) {
                    if (animation::ledmatrix_old[w] == 1) { //&& animation::ledmatrix[w] == 0) {
                        animation::ledmatrix_old[w] = 0;
                        // Calculate the next LED
                        w = Led::getLedId(c + GRID_COLS * ptr[c]);
                        animation::ledmatrix_old[w] = 1;
                        Led::ids[w] = color; // set the correct color because can be rainbow color
                        //Led::ids[w].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b); 
                        FastLED.show();
                        delay(Config::atime / 2);
                    }
                }
            }
        }
    }

    // Now drop the new time to position
    // Generate random delay speeds for each falling charater in columns
    byte dly2[11] = { (char)random(R), (char)random(R), (char)random(R), (char)random(R), (char)random(R), (char)random(R), (char)random(R), (char)random(R), (char)random(R), (char)random(R), (char)random(R) };
    byte ptr2[11] = { 0,0,0,0,0,0,0,0,0,0,0 }; // Points to the LED in each column we need to deal with
    int ptrLOWEST[11] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 }; //lowest row pointer
    //find the lowest row and which column for the new time
    for (int r = GRID_ROWS - 1; r >= 0; r--) {
        for (byte c = 0; c < GRID_COLS; c++) {
            w = Led::getLedId(c + GRID_COLS * r);
            if (animation::ledmatrix[w] == 1 && ptrLOWEST[c] < r) {//do only if so far now lowest row found && animation::ledmatrix_old[w] == 0
                ptrLOWEST[c] = r;
            }
        }
    }
    for (byte t = 0; t < (R * GRID_ROWS); t++) { // Counter for completing all changes 
        //fill column pool 
        for (int i = 0; i < GRID_COLS; i++) {
            randompool[i] = i;
        }
        //for (byte c = 0; c < GRID_COLS; c++) { // Count through columns and check timer not exceeded
        for (int r = (GRID_COLS - 1); r >= 0; r--) {
            //generate a random column number
            ran = rand() % (r + 1);
            int c = randompool[ran];
            for (int i = ran; i < r; ++i) {
                randompool[i] = randompool[i + 1];
            }
            if ((t > (dly2[c] * (ptr2[c] + 1))) && ptr2[c] < GRID_ROWS) { // If timer exceeded then erase current value and draw a white curosr in this position based on random time period     
                // Write over the previous value
                // Calculate the LED value from the Column and ptr value
                w = Led::getLedId(c + GRID_COLS * ptr2[c]); // Calculate the LED value in the 11 x 11 Matrix
                if (ptr2[c] <= ptrLOWEST[c]) {// animation::ledmatrix[w] == 1) { // If the bit set in LED Matrix then leave White
                    Led::ids[w].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b); //foreground color
                    FastLED.show();
                    delay(Config::atime / 2);
                    if (animation::ledmatrix[w] == 0)
                        Led::ids[w].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2); //background color
                    FastLED.show();
                    delay(Config::atime / 2);
                }

                ptr2[c]++; // Increment row and print FG color value  
                w = Led::getLedId(c + GRID_COLS * ptr2[c]);
                if (ptr2[c] <= ptrLOWEST[c]) {
                    Led::ids[w].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
                    FastLED.show();
                    delay(Config::atime / 2);
                }
                else
                    break;
            }
        }
    }
    delay(10);
    EraseMinutes();
    SetDisplay(); //needed in case different background color
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Letters in rainbow colors
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void animation::RainbowDisplay() { // Step through each location in the Matrix Display Array Data in LED Matrix
    byte colorflag = 0;

    for (int l = 0; l < NUM_LEDS; l++) {
        if (animation::ledmatrix[l] == 1) {
            if (colorflag == 0) {
                Led::ids[l] = CRGB::Red;
            }
            else if (colorflag == 1) {
                Led::ids[l] = CRGB::Orange;
            }
            else if (colorflag == 2) {
                Led::ids[l] = CRGB::Green;
            }
            else if (colorflag == 3) {
                Led::ids[l] = CRGB::Aqua;
            }
            else if (colorflag == 4) {
                Led::ids[l] = CRGB::Indigo;
            }
            else if (colorflag == 5) {
                Led::ids[l] = CRGB::Violet;
            }
            colorflag++;
            if (colorflag == 6) {
                colorflag = 0; // Reset to beginneing of rainbow
            }
        }
        else {
            Led::ids[l].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2); //background color
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Letters in rainbow colors activly changing
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void animation::MovingRainbow() { // Step through each location in the Matrix Display Array Data in LED Matrix

    int deltahue = 110 - Config::atime;
    int speed = 60 - (Config::atime / 2);
    int initialhue = beat8(speed, 255);
    CHSV hsv;
    hsv.hue = initialhue;
    hsv.val = 255;
    hsv.sat = 240;
    for (int i = GRID_COLS; i < NUM_LEDS-4; i++) {
        if (animation::ledmatrix[i] == 1) {
            Led::ids[i] = hsv;
            hsv.hue += deltahue;
        }
        else
            Led::ids[i].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2); //background color
    }
}


/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Letters dropping in blue / rain fall
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void animation::RainDisplay() { // Step through each location in the Matrix Display Array Data in LED Matrix
    byte w = 0; // temp variable

    // Generate random delay speeds for each falling charater in columns
    byte dly1[11] = { (char)random(5), (char)random(5), (char)random(5), (char)random(5), (char)random(5), (char)random(5), (char)random(5), (char)random(5), (char)random(5), (char)random(5), (char)random(5) };
    byte ptr[11] = { 0,0,0,0,0,0,0,0,0,0,0 }; // Points to the LED in each column we need to deal with

    for (byte t = 0; t < 3; t++) { // Counter for completing all changes for 11 rows

        for (byte c = 0; c < GRID_COLS; c++) { // Count through columns and check timer not exceeded
            if ((t > (dly1[c] * (ptr[c] + 1))) && ptr[c] < GRID_ROWS) { // If timer exceeded then erase current value and draw a white curosr in this position based on random time period     
                // Write over the previous value
                // Calculate the LED value from the Column and ptr value
                w = Led::getLedId(c + GRID_COLS * ptr[c]); // Calculate the LED value in the 11 x 11 Matrix

                if (animation::ledmatrix[w] == 1) { // If the bit set in LED Matrix then leave White
                    Led::ids[w].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b); //foreground color
                }
                else {
                    Led::ids[w] = CRGB::Blue; // .setRGB(Config::color_mx.r, Config::color_mx.g, Config::color_mx.b); //matrix color
                }

                FastLED.show();
                delay(Config::atime);
                ptr[c]++;
            }
        }
    }

    // Now clear the screen
   // Generate random delay speeds for each falling charater in columns
    byte dly2[11] = { (char)random(5), (char)random(5), (char)random(5), (char)random(5), (char)random(5), (char)random(5), (char)random(5), (char)random(5), (char)random(5), (char)random(5), (char)random(5) };
    byte ptr2[11] = { 0,0,0,0,0,0,0,0,0,0,0 }; // Points to the LED in each column we need to deal with

    for (byte t = 0; t < 50; t++) { // Counter for completing all changes for 10 rows
        for (byte c = 0; c < GRID_COLS; c++) { // Count through columns and check timer not exceeded
            if ((t > (dly2[c] * (ptr2[c] + 1))) && ptr2[c] < GRID_ROWS) { // If timer exceeded then erase current value and draw a white curosr in this position based on random time period     
                // Write over the previous value
                // Calculate the LED value from the Column and ptr value
                w = Led::getLedId(c + GRID_COLS * ptr2[c]); // Calculate the LED value in the 11 x 11 Matrix

                if (animation::ledmatrix[w] == 1) { // If the bit set in LED Matrix then leave White
                    Led::ids[w].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
                }
                else {
                    Led::ids[w].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2); //background color
                }
                FastLED.show();
                delay(5);
                ptr2[c]++; // Increment row and print White value    

                if (ptr2[c] < GRID_ROWS) {
                    // Calculate the LED value from the Column and ptr value
                    w = Led::getLedId(c + GRID_COLS * ptr2[c]); // Calculate the LED value in the 11 x 11 Matrix
                    Led::ids[w] = CRGB::Blue; //setRGB(Config::color_mx.r, Config::color_mx.g, Config::color_mx.b); // matrix color
                }
                FastLED.show();
                delay(5);
            }
        }
    }
    SetDisplay(); //needed in case different background color
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Letters typed in from left to right
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void animation::TypeWriter() { // Step through each location in the Matrix Display Array Data in LED Matrix

    for (int l = 0; l < NUM_LEDS - 4; l++) {
        Led::ids[Led::getLedId(l)] = CRGB::Blue;
        FastLED.show();
        delay(Config::atime);
        //Led::ids[Led::getLedId(l)] = CRGB::Black;
        Led::ids[Led::getLedId(l)].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2); //background color
        FastLED.show();
        delay(10);
        if (animation::ledmatrix[Led::getLedId(l)] == 1) {
            Led::ids[Led::getLedId(l)].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
            FastLED.show();
            delay(Config::atime);
        }
    }
    delay(500);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * randomly placed letters. Only used will stay others disappier
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void animation::RandomLetters() {
    //clear the Display
    void ClearDisplay();
    //first generate an array with all LED idxs as a pool for the random 
    int randompool[NUM_LEDS - 4 + 1];
    int usedPool[NUM_LEDS - 4];
    byte idx;
    int i;
    int l = 0;
    int ran;

    //define pool
    for (i = 0; i < NUM_LEDS-4; i++) {
        randompool[i] = i;
    }
    randompool[NUM_LEDS-4] = -1; //define end of array

    //now loop all LEDs
    for (i = 0; i < NUM_LEDS - 4 ; i++) {
        //get one LED position
        ran = random(NUM_LEDS - 4 - i);
        idx = Led::getLedId(randompool[ran]);
        Led::ids[idx].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b); // select this LED
        //Led::ids[idx] = color[i % 8];
        FastLED.show();
        if (animation::ledmatrix[idx] == 0) { //if not a timeletter switch off after a delay T
            delay(Config::atime);
            usedPool[l++] = idx; //save used numbers
            //fade slowly out the none selected
            for (int k = 0; k < l; k++) {
                Led::ids[usedPool[k]].fadeToBlackBy(128);
            }
            FastLED.show();
        }
        delay(5);//prevent flickering
        //remove the found random number from pool
        int j = ran;
        while (randompool[j] != -1) {
            randompool[j] = randompool[j + 1];
            j++;
        }
        randompool[j - 1] = -1; //new end
    }
    //fade all out after reaching the end
    for (int j = 0; j < 10; j++) {
        for (int k = 0; k < l; k++) {
            Led::ids[usedPool[k]].fadeToBlackBy(128);
        }
        FastLED.show();
        delay(Config::atime);
    }
    SetDisplay(); //needed in case differen background color
    delay(100);
}

//save the active LED on
void animation::SetMatrix(int idx) {
    animation::ledmatrix[idx] = 1;
}

//clear all LEDs
void animation::SetMatrixOFF() {
    for (byte n = 0; n < NUM_LEDS; n++) {
        animation::ledmatrix[n] = 0;
    }
}

//clear the last row / minutes LEDs
void animation::EraseMinutes() {
    //delete last minute row
    if (GRID_SINGLE_MINUTES == 0) {
        for (int i = 0; i < 4; i++) {
            Led::ids[i].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
            animation::ledmatrix[i] = 0;
        }
    }  
    else if (GRID_SINGLE_MINUTES == 1) {
        for (int i = NUM_LEDS-4; i < NUM_LEDS; i++) {
            Led::ids[i].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
            animation::ledmatrix[i] = 0;
        }
    }
    else if (GRID_SINGLE_MINUTES == 2) {
        for (int i = 0; i < GRID_COLS; i++) {
            Led::ids[i].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
            animation::ledmatrix[i] = 0;
        }
    }
}

//erase the whole display and set to BG color
void animation::ClearDisplay() {
    for (int i = 0; i < NUM_LEDS; i++) {
        Led::ids[i].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
    }
    FastLED.show();
}

//set the display based on the selected LEDs
void animation::SetDisplay() {
    for (int i = 0; i < NUM_LEDS; i++) {
        Led::ids[i].maximizeBrightness();
        if (animation::ledmatrix[i] == 1) {
            Led::ids[i].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);

        }
        else
            Led::ids[i].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
    }
}

//generate the color changes for each LED in moving Rainbow
void animation::RainbowWave(int thisSpeed, int deltaHue) {  // The fill_rainbow call doesn't support brightness levels.
    int thisHue = beatsin8(thisSpeed, 0, 255);                // A simple rainbow wave.
   // int thisHue = beat8(thisSpeed, 255);                  // A simple rainbow march.
    fill_rainbow(Led::ids, NUM_LEDS, thisHue, deltaHue);    // Use FastLED's fill_rainbow routine.
}


