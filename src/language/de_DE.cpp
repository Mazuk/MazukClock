#include <Arduino.h>

#include "de_DE.h"
#include "../config.h"
#include "../dnd.h"
#include "../led.h"
#include "../animation.h"

#define ANI 5 // run animation every x minutes;

void Grid_de_DE::setTime(int hour, int minute) {
	if (hour == -1 || minute == -1) {
		return;
	}
	int m = minute;
	int h = hour;
	
	if (DND::active(hour, minute) && Config::animation_set != DEMO) { // don't do if in Demo
		for (int i = 0; i < NUM_LEDS; i++) {
			Led::ids[i].setRGB(0, 0, 0);
		}
		FastLED.show();
		return;
	}
	

	int singleMinute = minute % 5;
	int hourLimit = 6;
	//5 minute stepes 5,10,15,...
	minute = (minute - (minute % 5));
	if (Config::viertel) {
		if (minute >= 25) {
			hour += 1;
		}
	}
	else {
		if (minute >= 15) {
			hour += 1;
		}
	}

	minute = minute / 5;
	hour = hour % 12;
	//animation quarter and full hour
	if (Config::animation_bell > 0) {
		if (m == 15 && Config::animation_bell == 2) {
			//Grid::setDingDong(1,200,0,0);
			Grid::setCube(1);
			//Grid::setBall(1);
		}
		if (m == 30 && Config::animation_bell == 2) {
			//Grid::setDingDong(1,200,0,0);
			Grid::setBall(2);
		}

		if (m == 45 && Config::animation_bell == 2) {
			//Grid::setDingDong(1,200,0,0);
			Grid::setCube(3);
		}

		if (m == 0 && Config::animation_bell == 2) {
			//Grid::setDingDong(1,200,0,0); //quarter bell
			//Grid::setCube(4);
			Grid::setBall(4);
		}
		if (m == 0) {
		  //convert to 24 -> 12 hr --> h % 12;
		  h %= 12;
		  h = (h == 0) ? h = 12 : h; // in case 12 or middnight 0 o'clock use 12
		  Grid::setDingDong(h, 0, 200, 0); //full hour bell
		}
	}
  
	//set max brightness to 20%
	if (Config::animation_set < 1 || (Config::animation_set == DEMO && Config::animation[LENGTH] == 0)) {
		for (int i = 0; i < NUM_LEDS; i++) {
			Led::ids[i].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);	  
		}
	} 

	animation::SetMatrixOFF(); //set array all to 0

	//new design
	if (m <= 4 || (m >= 30) && (m <= 34)) {
		for (int i = 0; i < 5; i++) { 
			if (Config::animation_set > 0 && !(Config::animation_set == DEMO && Config::animation[LENGTH] == 0))
				animation::SetMatrix(Led::getLedId(Grid_de_DE::time_it_is[i]));
			else {
				Led::ids[Led::getLedId(Grid_de_DE::time_it_is[i])].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
				animation::SetMatrix(Led::getLedId(Grid_de_DE::time_it_is[i]));
			}
		}
	 
	}

	for (int m = 0; m < 12; m++) {
		if (Config::viertel) {
			if (Grid_de_DE::time_minutes[minute][m] >= 0) {
				if (Config::animation_set > 0 && !(Config::animation_set == DEMO && Config::animation[LENGTH] == 0))
					animation::SetMatrix(Led::getLedId(Grid_de_DE::time_minutes[minute][m]));
				else {
					Led::ids[Led::getLedId(Grid_de_DE::time_minutes[minute][m])].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
					animation::SetMatrix(Led::getLedId(Grid_de_DE::time_minutes[minute][m]));
				}
			}
		}
		else {
			if (Grid_de_DE::time_minutes_alt[minute][m] >= 0) {
				if (Config::animation_set > 0 && !(Config::animation_set == DEMO && Config::animation[LENGTH] == 0))
					animation::SetMatrix(Led::getLedId(Grid_de_DE::time_minutes_alt[minute][m]));
				else {
					Led::ids[Led::getLedId(Grid_de_DE::time_minutes[minute][m])].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
					animation::SetMatrix(Led::getLedId(Grid_de_DE::time_minutes_alt[minute][m]));
				}
			}
		}

	}

	if (hour == 1 && minute == 0) {
		hourLimit = 3;
	}

	for (int h = 0; h < hourLimit; h++) {
		if (Config::viertel) {
			if (Grid_de_DE::time_hours[hour][h] >= 0) {
				if (Config::animation_set > 0 && !(Config::animation_set == DEMO && Config::animation[LENGTH] == 0))
					animation::SetMatrix(Led::getLedId(Grid_de_DE::time_hours[hour][h]));
				else {
					Led::ids[Led::getLedId(Grid_de_DE::time_hours[hour][h])].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
					animation::SetMatrix(Led::getLedId(Grid_de_DE::time_hours[hour][h]));
				}
			}
		}
		else {
			if (Grid_de_DE::time_hours_alt[hour][h] >= 0) {
				if (Config::animation_set > 0 && !(Config::animation_set == DEMO && Config::animation[LENGTH] == 0))
					animation::SetMatrix(Led::getLedId(Grid_de_DE::time_hours_alt[hour][h]));
				else {
					Led::ids[Led::getLedId(Grid_de_DE::time_hours_alt[hour][h])].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
					animation::SetMatrix(Led::getLedId(Grid_de_DE::time_hours_alt[hour][h]));
				}
			}
		}
	}
  
	//animation every change
	if (((Config::animation_set > 0 && !(Config::animation_set == DEMO && Config::animation[LENGTH] == 0)) && (m % ANI == 0)) || (animation::test_start && Config::animation[LENGTH] >= 0)) {
		#ifdef DEBUG
			Serial.println("Animation started!");
		#endif
		//animation::test_start = false;
		animation::EraseMinutes();
		animation::animation_select(Config::animation_set);
		//copy the old time
		for (int i = 0; i < NUM_LEDS; i++) {
			animation::ledmatrix_old[i] = animation::ledmatrix[i];
		}
	}
	animation::test_start = false;
	//show ticker if set
	if (Config::tu != 0) {
		if (m % Config::tu == 0)
			animation::animation_select(TICKER);
	}
	//show weather if set
	if (Config::WX != 0) {
		if (m % Config::WX == 0)
			animation::animation_select(WEATHER);
	}
	// single minutes
	if (singleMinute >= 1) {
		Led::ids[2].setRGB(Config::color_m1.r, Config::color_m1.g, Config::color_m1.b);

	}
	if (singleMinute >= 2) {
		Led::ids[4].setRGB(Config::color_m2.r, Config::color_m2.g, Config::color_m2.b);
	
	}
	if (singleMinute >= 3) {
		Led::ids[6].setRGB(Config::color_m3.r, Config::color_m3.g, Config::color_m3.b);

	}
	if (singleMinute >= 4) {
		Led::ids[8].setRGB(Config::color_m4.r, Config::color_m4.g, Config::color_m4.b);

	}
  
  FastLED.setBrightness(Config::brightness * 255);
  FastLED.show();
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Show a bell for full hour
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void Grid_de_DE::setDingDong(int dingdong, int r, int g, int b) {
	while (dingdong > 0) {
		//clear all to BG color normaly black
		for (int i = 0; i < NUM_LEDS; i++) {
			Led::ids[i].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
		}
		//FastLED.setBrightness(Config::brightness * 255);
		FastLED.show();
		delay(500);
		//set ding
			
		for (int row = 0; row < 11; row++) {
			for (int col = 0; col < 12; col++) {
				if (Grid_de_DE::ding[row][col] >= 0) {
					Led::ids[Led::getLedId(Grid_de_DE::ding[row][col])].setRGB(r, g, b);
				}
			}
		}

		FastLED.show();
		dingdong--;
		delay(800);
	}
	//clear all to BG color normaly black
	for (int i = 0; i < NUM_LEDS; i++) {
		Led::ids[i].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
	}
	//FastLED.setBrightness(Config::brightness * 255);
	FastLED.show();
	delay(10);
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Animate a cube
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void Grid_de_DE::setCube(int dingdong) {
	int i;
	CRGB color[] = { CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green, CRGB::Blue, CRGB::Indigo, CRGB::Violet };
	while (dingdong > 0) {
		//unsigned char r;
		//unsigned char g;
		//unsigned char b; 
		//clear all to BG color normaly black
		animation::ClearDisplay();
		delay(500);
		//set ding
		int inc = 1;
		//count row 0 to 5 and back to 0
		for (int row = 0; row >=  0; row += inc) {
			i = 0;
			//r = (unsigned char)rand();
			//g = (unsigned char)rand();
			//b = (unsigned char)rand();
			if (row == 5) //reverse loop
				inc = -1;
			while(Grid_de_DE::cube[row][i] >= 0) {
				//Led::ids[Grid_de_DE::cube[row][i]].setRGB(r, g, b);
				Led::ids[Grid_de_DE::cube[row][i]]=color[row];
				i++;
			}
			FastLED.setBrightness(Config::brightness * 255);
			FastLED.show();
			delay(200);
			i = 0;
			while (Grid_de_DE::cube[row][i] >= 0) {
				Led::ids[Grid_de_DE::cube[row][i]].setRGB(0, 0, 0);
				i++;
			}
		}
		delay(800);
		dingdong--;
	}
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Ball animation
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void Grid_de_DE::setBall(int dingdong) {
	int i;
	CRGB color[] = { CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green, CRGB::Blue, CRGB::Indigo, CRGB::Violet };
	//clear all to BG color normaly black
	animation::ClearDisplay();
	delay(5);
	while (dingdong > 0) {
		//set ding
		int inc = 1;
		//count row 0 to 5 and back to 0
		for (int row = 0; row >= 0; row += inc) {
			i = 0;
			while (Grid_de_DE::ball[row][i] >= 0) {
				Led::ids[Grid_de_DE::ball[row][i]]=color[row];
				i++;
			}
			FastLED.show();
			delay(200);
			if (row == 6 || inc == -1) {//reverse loop 
				inc = -1;
				i = 0;
				while (Grid_de_DE::ball[row][i] >= 0) {
					Led::ids[Grid_de_DE::ball[row][i]].setRGB(0, 0, 0);
					i++;
				}
			}	
		}
		delay(200);
		Led::ids[Grid_de_DE::ball[0][0]].setRGB(0, 0, 0);
		FastLED.show();
		delay(800);
		dingdong--;
	}
}

int Grid_de_DE::time_it_is[5] = {0, 1, 3, 4, 5}; // es ist

int Grid_de_DE::time_minutes[12][12] = {
  {107, 108, 109,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // uhr
  {  7,   8,   9,  10,  35,  36,  37,  38,  -1,  -1,  -1,  -1}, // fünf nach
  { 11,  12,  13,  14,  35,  36,  37,  38,  -1,  -1,  -1,  -1}, // zehn nach
  { 26,  27,  28,  29,  30,  31,  32,  35,  36,  37,  38,  -1}, // viertel nach
  { 15,  16,  17,  18,  19,  20,  21,  35,  36,  37,  38,  -1}, // zwanzig nach
  {  7,   8,   9,  10,  39,  40,  41,  44,  45,  46,  47,  -1}, // fünf vor halb
  { 44,  45,  46,  47,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // halb
  {  7,   8,   9,  10,  35,  36,  37,  38,  44,  45,  46,  47}, // fünf nach halb
  { 15,  16,  17,  18,  19,  20,  21,  39,  40,  41,  -1,  -1}, // zwanzig vor
  { 26,  27,  28,  29,  30,  31,  32,  39,  40,  41,  -1,  -1}, // viertel vor
  { 11,  12,  13,  14,  39,  40,  41,  -1,  -1,  -1,  -1,  -1}, // zehn vor
  {  7,   8,   9,  10,  39,  40,  41,  -1,  -1,  -1,  -1,  -1}  // fünf vor
};

int Grid_de_DE::time_hours[12][6] = {
  { 49,  50,  51,  52,  53,  -1}, // zwölf
  { 57,  58,  59,  60,  -1,  -1}, // eins
  { 55,  56,  57,  58,  -1,  -1}, // zwei
  { 67,  68,  69,  70,  -1,  -1}, // drei
  { 84,  85,  86,  87,  -1,  -1}, // vier
  { 73,  74,  75,  76,  -1,  -1}, // fünf
  {100, 101, 102, 103, 104,  -1}, // sechs
  { 60,  61,  62,  63,  64,  65}, // sieben
  { 89,  90,  91,  92,  -1,  -1}, // acht
  { 80,  81,  82,  83,  -1,  -1}, // neun
  { 93,  94,  95,  96,  -1,  -1}, // zehn
  { 77,  78,  79,  -1,  -1,  -1}  // elf
};

int Grid_de_DE::time_minutes_alt[12][12] = {
  {107, 108, 109,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // uhr
  {  7,   8,   9,  10,  35,  36,  37,  38,  -1,  -1,  -1,  -1}, // fünf nach
  { 11,  12,  13,  14,  35,  36,  37,  38,  -1,  -1,  -1,  -1}, // zehn nach
  { 26,  27,  28,  29,  30,  31,  32,  -1,  -1,  -1,  -1,  -1}, // viertel
  { 11,  12,  13,  14,  39,  40,  41,  44,  45,  46,  47,  -1}, // zehn vor halb
  {  7,   8,   9,  10,  39,  40,  41,  44,  45,  46,  47,  -1}, // fünf vor halb
  { 44,  45,  46,  47,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // halb
  {  7,   8,   9,  10,  35,  36,  37,  38,  44,  45,  46,  47}, // fünf nach halb
  { 11,  12,  13,  14,  35,  36,  37,  38,  44,  45,  46,  47}, // zehn nach halb
  { 22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  -1}, // dreiviertel		
  { 11,  12,  13,  14,  39,  40,  41,  -1,  -1,  -1,  -1,  -1}, // zehn vor
  {  7,   8,   9,  10,  39,  40,  41,  -1,  -1,  -1,  -1,  -1}  // fünf vor
};

int Grid_de_DE::time_hours_alt[12][6] = {
  { 49,  50,  51,  52,  53,  -1}, // zwölf
  { 57,  58,  59,  60,  -1,  -1}, // eins
  { 55,  56,  57,  58,  -1,  -1}, // zwei
  { 67,  68,  69,  70,  -1,  -1}, // drei
  { 84,  85,  86,  87,  -1,  -1}, // vier
  { 73,  74,  75,  76,  -1,  -1}, // fünf
  {100, 101, 102, 103, 104,  -1}, // sechs
  { 60,  61,  62,  63,  64,  65}, // sieben
  { 89,  90,  91,  92,  -1,  -1}, // acht
  { 80,  81,  82,  83,  -1,  -1}, // neun
  { 93,  94,  95,  96,  -1,  -1}, // zehn
  { 77,  78,  79,  -1,  -1,  -1}  // elf
};

int Grid_de_DE::ding[11][12] = {
  {-1, -1, -1,  -1,  -1,  5,  -1,  -1,  -1,  -1,  -1,  -1}, 
  {-1, -1, -1,  -1,  15,  16,  17,  -1,  -1,  -1,  -1,  -1},
  {-1, -1, -1,  25,  -1,  -1,  -1,  29,  -1,  -1,  -1,  -1},
  {-1, -1, -1,  36,  -1,  -1,  -1,  40,  -1,  -1,  -1,  -1},
  {-1, -1, -1,  47,  -1,  -1,  -1,  51,  -1,  -1,  -1,  -1},
  {-1, -1, 57,  -1,  -1,  -1,  -1,  -1,  63,  -1,  -1,  -1},
  {-1, 67, 68,  69,  70,  71,  72,  73,  74,  75,  -1,  -1},
  {-1, 78, 79,  80,  81,  82,  83,  84,  85,  86,  -1,  -1},
  {-1, -1, -1,  91,  92,  93,  94,  95,  -1,  -1,  -1,  -1},
  {-1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
  {-1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}
};

int Grid_de_DE::cube[6][41] = {
	{60,-1},
	{48,49,50,59,61,70,71,72,-1},
	{36,37,38,39,40,47,51,58,62,69,73,80,81,82,83,84,-1},
	{24,25,26,27,28,29,30,35,41,46,52,57,63,68,74,79,85,90,91,92,93,94,95,96,-1},
	{12,13,14,15,16,17,18,19,20,23,31,34,42,45,53,56,64,67,75,78,86,89,97,100,101,102,103,104,105,106,107,108,-1},
	{0,1,2,3,4,5,6,7,8,9,10,11,21,22,32,33,43,44,54,55,65,66,76,77,87,88,98,99,109,110,111,112,113,114,115,116,117,118,119,120,-1}
};

int Grid_de_DE::ball[7][35] = {
	{60,-1},
	{49,59,61,71,-1},
	{38,48,50,58,62,70,72,82,-1},
	{37,39,47,51,69,73,81,83,-1},
	{26,27,28,36,40,46,52,57,63,68,74,80,84,92,93,94,-1},
	{15,16,17,25,29,35,41,45,53,56,64,67,75,79,85,91,95,103,104,105,-1},
	{4,5,6,14,13,23,18,19,31,24,30,34,42,44,54,55,65,66,76,78,86,90,96,102,106,107,97,101,89,114,115,116,-1}
};
