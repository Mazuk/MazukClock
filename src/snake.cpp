#include <Arduino.h>

#include "config.h"
#include "led.h"
#include "snake.h"
#include "animation.h"

int Snake::userDirection; 
int Snake::gameState; 
int Snake::gameloop;
Coords Snake::head;
Coords Snake::tail[MAX_TAIL_LENGTH];
Coords Snake::food;
unsigned long Snake::lastDrawUpdate = 0;
unsigned int Snake::wormLength = 0;

//Array for LED buffer
int Snake::ledmatrix[NUM_LEDS][3];
int Snake::tempctr = 0;
int Snake::LeftPlayground = 0;
int Snake::PositionFood = 0; //Food position 

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Set the LED either with sanke or with the real time
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void Snake::toggleLed(int x, int y, int type, int place)
{
    if (x < 0 || y < 0)
        return;

    int ledIndex = Led::getLedId(animation::matrix[y][x]);
    
    if (place == SHIT) {
        if (animation::ledmatrix[ledIndex] == 1)
            Led::ids[ledIndex].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
        else
            Led::ids[ledIndex].setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
    }
    else {
        CRGB color;
        switch (type) {
        case LED_TYPE_SNAKE:
            color.setRGB(Config::color_mx.r, Config::color_mx.g, Config::color_mx.b); 
            break;
        case LED_TYPE_OFF:
            color.setRGB(Config::color_bg.r * 0.2, Config::color_bg.g * 0.2, Config::color_bg.b * 0.2);
            break;
        case LED_TYPE_FOOD:
            color = CRGB::Aqua;
            break;
        case LED_TYPE_BLOOD:
            color = CRGB::Red;
            break;
        }
        Led::ids[ledIndex] = color;           
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Where is the next food / time letter
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void Snake::Food() 
{
    int t = 0;
    PositionFood = -1;
    for (int i = 0; i < NUM_LEDS-4; i++) { // Check where is the next food
        if (Snake::ledmatrix[i][0] == 1) {
            if (t == 0) {
                t = 1;
                PositionFood = i;
                break;
            }
        }
    }
    #ifdef DEBUG
        Serial.print("Position Food: ");
        Serial.println(PositionFood);
    #endif
}

// ------------------------------------------------------------------------------------------------------

void Snake::updateGame(byte place) //Updates the snake
{
    if ((millis() - lastDrawUpdate) > GAME_DELAY * Config::atime) { // Delayed update after the configuered ms
        gameloop++; // only needed not to be in an endless loop
        if (PositionFood != -1) {
            Snake::KI();
            switch (userDirection) { // which direction to go?
                case DIRECTION_LEFT:
                    if (head.x > 0) 
                        head.x--;
                    else 
                        LeftPlayground = 1;
                    break;
                case DIRECTION_RIGHT:
                    if (head.x < X_MAX - 1) 
                        head.x++;
                    else 
                        LeftPlayground = 1;
                    break;
                case DIRECTION_UP:
                    if (head.y > 0) 
                        head.y--;
                    else 
                        LeftPlayground = 1;
                    break;
                case DIRECTION_DOWN:
                    if (head.y < GRID_ROWS - 1) 
                        head.y++;
                    else 
                        LeftPlayground = 1;
                    break;
                case DIRECTION_NONE:
                    break;
            }
        }
        if (Snake::isCollision() == true || PositionFood == -1) { // Check if there was a crash
            if(place == SHIT)
                Snake::updateTail(place); // update the snake body
            Snake::endGame();
            return;
        }
        if(place == SHIT)
            Snake::toggleLed(tail[wormLength - 1].x, tail[wormLength - 1].y, LED_TYPE_OFF, SHIT); // clear the snake tail and place time
        else
            Snake::toggleLed(tail[wormLength - 1].x, tail[wormLength - 1].y, LED_TYPE_OFF, EAT); // clear snake tail only
        
        Snake::updateTail(place); 
        if (Snake::ledmatrix[animation::matrix[head.y][head.x]][0] == 1) {
            Snake::ledmatrix[animation::matrix[head.y][head.x]][0] = 0;

           if (wormLength < MAX_TAIL_LENGTH && place == EAT) {
               wormLength++;// = wormLength + (head.x % 2);
           }
           else if (wormLength > MIN_TAIL_LENGTH) {
               wormLength--;
           }
           Snake::Food();  
        }
   
        lastDrawUpdate = millis();
        FastLED.show();
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * A ver simple Ai / KI for moving the snake
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void Snake::KI() {

    if (head.x < Snake::ledmatrix[PositionFood][1] && Crash(1) == false && head.x < X_MAX) {
        userDirection = DIRECTION_RIGHT;
        #ifdef DEBUG
            Serial.println("Set right");
        #endif
    }
    else if (head.x > Snake::ledmatrix[PositionFood][1] && Crash(2) == false && head.x >= 0) {
        userDirection = DIRECTION_LEFT;
        #ifdef DEBUG
            Serial.println("Set left");
        #endif
    }

    else if (head.y < Snake::ledmatrix[PositionFood][2] && Crash(3) == false && head.y < GRID_ROWS) {
        userDirection = DIRECTION_DOWN;
        #ifdef DEBUG
            Serial.println("Set down");
        #endif
    }
    else if (head.y > Snake::ledmatrix[PositionFood][2] && Crash(4) == false && head.y >= 0) {
        userDirection = DIRECTION_UP;
        #ifdef DEBUG
            Serial.println("Set up");
        #endif
    }
    else if (Snake::ledmatrix[PositionFood][1] == head.x) {
        #ifdef DEBUG
            Serial.println("Food on X axis");
        #endif
        if (head.x < X_MAX - 1 && Crash(1) == false) {
            userDirection = DIRECTION_RIGHT;
            #ifdef DEBUG
                Serial.println("Set food to X right");
            #endif
        }
        else if (head.x >= 0 && Crash(2) == false) {
            userDirection = DIRECTION_LEFT;
            #ifdef DEBUG
                Serial.println("Set food to X left");
            #endif
        }
    }
    else if (Snake::ledmatrix[PositionFood][2] == head.y) {
        if (head.y < GRID_ROWS - 1 && Crash(3) == false) {
            userDirection = DIRECTION_DOWN;
            #ifdef DEBUG
                Serial.println("Set food to Y down");
            #endif
        }
        else if (head.y >= 0 && Crash(4) == false) {
            userDirection = DIRECTION_UP;
            #ifdef DEBUG
                Serial.println("Set food to Y up");
            #endif
        }
    }

    else {
        if (head.y < GRID_ROWS - 1 && Crash(3) == false) {
            userDirection = DIRECTION_DOWN;
            #ifdef DEBUG
                Serial.println("ELSE - down");
            #endif
        }
        else if (head.y > 0 && Crash(4) == false) {
            userDirection = DIRECTION_UP;
            #ifdef DEBUG
                Serial.println("ELSE - up");
            #endif
        }
        if (head.x < X_MAX - 1 && Crash(1) == false) {
            userDirection = DIRECTION_RIGHT;
            #ifdef DEBUG
                Serial.println("ELSE - right");
            #endif
        }
        else if (head.x > 0 && Crash(2) == false) {
            userDirection = DIRECTION_LEFT;
            #ifdef DEBUG
                Serial.println("ELSE - left");
            #endif
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Check if the returned direction will crash into the tail / body
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
bool Snake::Crash(int Direction) {

    switch (Direction)
    {
    case 1:
        for (byte i = 1; i < wormLength; i++) {
            if (tail[i].x == head.x + 1 && tail[i].y == head.y) {
                #ifdef DEBUG
                    Serial.println("right - prevented crash");
                #endif
                return true;
            }
        }

        break;
    case 2:
        for (byte i = 1; i < wormLength; i++) {
            if (tail[i].x == head.x - 1 && tail[i].y == head.y) {
                #ifdef DEBUG
                    Serial.println("left - prevented crash");
                #endif
                return true;
            }
        }
        break;
    case 3:
        for (byte i = 1; i < wormLength; i++) {
            if (tail[i].x == head.x && tail[i].y == head.y + 1) {
                #ifdef DEBUG
                    Serial.println("down - prevented crash");
                #endif
                return true;
            }
        }
        break;

    case 4:
        for (byte i = 1; i < wormLength; i++) {
            if (tail[i].x == head.x && tail[i].y == head.y - 1) {
                #ifdef DEBUG
                    Serial.println("up - prevented crash");
                #endif
                return true;
            }
        }
        break;
    }
    return false;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Make the tail of the snake
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void Snake::updateTail(byte place) // Update the snake body
{
    if (PositionFood > -1 ) {
        for (int i = wormLength - 1; i > 0; i--) {//-1
            tail[i].x = tail[i - 1].x;
            tail[i].y = tail[i - 1].y;
        }
    }
    tail[0].x = head.x;
    tail[0].y = head.y;
    int i;
    for (i = 0; i < wormLength; i++) {
        if (tail[i].x >= 0) {
            if (PositionFood == -1 && place == SHIT) {
                Snake::toggleLed(tail[wormLength -i].x, tail[wormLength -i].y, LED_TYPE_FOOD, SHIT); //head
                FastLED.show();
                delay(GAME_DELAY * Config::atime);
            }
            else {
                if (i == 0)
                    Snake::toggleLed(tail[i].x, tail[i].y, LED_TYPE_FOOD, EAT); //head
                else
                    Snake::toggleLed(tail[i].x, tail[i].y, LED_TYPE_SNAKE, EAT); //tail
            }
        }   
    }
    Snake::toggleLed(tail[i].x, tail[i].y, LED_TYPE_SNAKE, SHIT); //tail set or delete last led 
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Test if we crached into the snake body
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
bool Snake::isCollision()
{
    // Head outside  X or Y?
    if (LeftPlayground == 1) {
        #ifdef DEBUG
            Serial.println("crash - head outside play field");
        #endif
        LeftPlayground = 0;
        return true;
    }

    // Test for collition with snake body
    for (int i = 1; i < wormLength; i++) {
        if (tail[i].x == head.x && tail[i].y == head.y) {
            #ifdef DEBUG
                Serial.println("crash - head crashed into body");
            #endif
            Snake::toggleLed(tail[i].x, tail[i].y, LED_TYPE_BLOOD, EAT); //head
            FastLED.show();
            delay(800);
            return true;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Fill the playground with the time
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void Snake::fill_ledmatrix_buff(int* display) {
    for (byte x = 0; x < X_MAX; x++) {
        for (byte y = 0; y < GRID_ROWS; y++) {
            tempctr = animation::matrix[y][x];
            if (display[Led::getLedId(tempctr)] == 1)
                Snake::ledmatrix[tempctr][0] = 1;  
            else
                Snake::ledmatrix[tempctr][0] = 0; 
            Snake::ledmatrix[tempctr][1] = x; 
            Snake::ledmatrix[tempctr][2] = y;
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Start the gane
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void Snake::initGame(byte place) 
{
    if (place == EAT) {
        head.x = 0; // Start of snake top left
        head.y = 0;
        food.x = -1; // No food yet
        food.y = -1;
        wormLength = MIN_TAIL_LENGTH; // Max length of snake
        userDirection = DIRECTION_RIGHT; // Start direction
        for (int i = 0; i < MAX_TAIL_LENGTH; i++) { 
            tail[i].x = -1;
            tail[i].y = -1;
        }
        Snake::ledmatrix[0][0] = 1;
        PositionFood = 0;  
    }
    else {
        Snake::Food();
    }
    gameState = GAME_STATE_RUNNING; // Snake to running state
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Game OVER
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void Snake::endGame() {
    gameState = GAME_STATE_END;
    #ifdef DEBUG
        Serial.println("GAME OVER ");
    #endif
}

/*-------------------------------------------------------------------------------------------------------------------------------------------
 * Start moving the snake
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */
void Snake::StartSnake() {
    Snake::fill_ledmatrix_buff(animation::ledmatrix_old);
    Snake::initGame(EAT);
    gameloop = 0;
    while (gameState != GAME_STATE_END  && gameloop < 100) { // loop just in case it can't find an end
        Snake::updateGame(EAT);
        yield(); //needed to reset the soft watchdog
    }

    Snake::fill_ledmatrix_buff(animation::ledmatrix);
    Snake::initGame(SHIT);
    gameloop = 0;
    while (gameState != GAME_STATE_END && gameloop < 100) {
        Snake::updateGame(SHIT);
        yield();
    }
    animation::SetDisplay();
}
