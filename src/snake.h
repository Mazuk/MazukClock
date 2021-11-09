#ifndef WORDCLOCK_SNAKE_H
#define WORDCLOCK_SNAKE_H
#include <FastLED.h>

#define X_MAX 11


#define GAME_DELAY 3  // *attime in ms

#define LED_TYPE_SNAKE 1
#define LED_TYPE_OFF   2
#define LED_TYPE_FOOD  3
#define LED_TYPE_BLOOD 4
#define SHIT 1
#define EAT 0

#define DIRECTION_NONE  0
#define DIRECTION_UP    1
#define DIRECTION_DOWN  2
#define DIRECTION_LEFT  3
#define DIRECTION_RIGHT 4

#define GAME_STATE_RUNNING 1
#define GAME_STATE_END     2
#define GAME_STATE_INIT    3
#define GAME_STATE_PAUSED  4

#define MAX_TAIL_LENGTH 16
#define MIN_TAIL_LENGTH 2


//CRGB leds[NUM_LEDS]; //Led Aufruf
/*
const int matrix[11][11] = {
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
*/

typedef struct {
    int x;
    int y;
} Coords;

class Snake {
public:
    //static CRGB leds[NUM_LEDS]; //Led Aufruf
    //Array für LED Buffer
    static int ledmatrix[NUM_LEDS][3];
    static int tempctr;
    static int userDirection;
    static int gameState; // Game state 
    static int gameloop; // controll for game loops max interartions
    static Coords head;
    static Coords tail[MAX_TAIL_LENGTH];
    static Coords food;
    static unsigned long lastDrawUpdate;
    static unsigned int wormLength;

    static int LeftPlayground;
    static int PositionFood; // Position der Nahrung, die von der KI angesteuert wird

    static void toggleLed(int x, int y, int type, int place);
    static void KI();
    static bool Crash(int Richtung);
    static void Food();
    static void updateGame(byte place);
    static void updateTail(byte place);
    static bool isCollision();
    static void fill_ledmatrix_buff(int* display);
    static void initGame(byte place);
    static void endGame();
    static void StartSnake();
};
#endif