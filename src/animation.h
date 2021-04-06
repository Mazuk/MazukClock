#ifndef ANIMATION_H
#define ANIMATION_H

#include "config.h"

class animation {
public:
	

	static void SetMatrix(int idx);
	static void SetMatrixOFF();
	static void EraseMinutes();
	static void setup();
	static void ledtest();
	static void animation_select(int ani);
	static void loop();
	static void CircleDisplay(bool dir);
	static void Matrixeffect();
	static void Matrixeffect2();
	static void Dropeffect();
	static void Fadeeffect();
	static void RainbowDisplay();
	static void RainDisplay();
	static void TypeWriter();
	//static void DemoMode();
	static void RandomLetters();
	static void MovingRainbow();
	static void ClearDisplay();
	static void SetDisplay();
	static void RainbowWave(int thisSpeed, int deltaHue);
	static bool test_start;
	static int ledmatrix[];
	static int ledmatrix_old[];
	static bool MovinRainbowSet;
	static bool SnakeSet;
	static int matrix[11][11];
	
};


#endif
