#ifndef WORDCLOCK_CONFIG_H
#define WORDCLOCK_CONFIG_H

#include <Arduino.h>

#include "../user_config.h"
#include "types.h"

//#define DEBUG
#define DEBUGTIME

/***********************************************
 * CONFIG START
 **********************************************/
 /*
 * Version
 */
#ifndef VERSION
#define VERSION "V2.83"
#endif

 /* Grid rows
  *
  * Options
  * -------
  * 10: 10 rows
  * 11: 11 rows
  */
#ifndef GRID_ROWS
#define GRID_ROWS 11
#endif

  /* Grid cols
   *
   * Options
   * -------
   * 11: 11 cols
   */
#ifndef GRID_COLS
#define GRID_COLS 11
#endif

   /* Postition of first LED
    *
    * Options
    * -------
    * 0: top-left
    * 1: top-right
    * 2: buttom-left
    * 3: buttom-right
    */
#ifndef GRID_FIRST
#define GRID_FIRST 2
#endif

    /* Grid language
     *
     * Options
     * -------
     * 1: de-DE:     german
     * 2: de-DE_alt: german alternative
     */
#ifndef GRID_LANGUAGE
#define GRID_LANGUAGE de_DE
     //#define GRID_LANGUAGE de_DE_alt
#endif

/* Position of single minute LEDs
 *
 * Options
 * -------
 * 0: before
 * 1: after
 * 2: not used
 */
#ifndef GRID_SINGLE_MINUTES
#define GRID_SINGLE_MINUTES 0
#endif

 /* GRID has the GRAD letters between the minutes
  *
  * Options
  * -------
  * 0: NO
  * 1: ENABLED
  */
#ifndef USE_GRAD
#define USE_GRAD 0
#endif

 /* Data PIN, where the LED strip is connected to */
#ifndef DATA_PIN
#define DATA_PIN D3
#endif

#ifndef CONSUMPTION_D1_MINI
#define CONSUMPTION_D1_MINI 170 // 170mA in active mode
#endif

#ifndef CONSUMPTION_PER_LED
#define CONSUMPTION_PER_LED 60 // 60mA per LED on full brightness in white.
#endif

#define DEMO 12 //indicator for demo mode in aniation select. Always highest animation numer
#define WEATHER 50 //Indication for weather ticker
#define TICKER 51 //the ticker
#define LENGTH 20 //indey where the length of the animation json is stored
/***********************************************
 * CONFIG END
 **********************************************/

class Config {
public:
    static color_t color_bg;
    static color_t color_fg;
    static color_t color_mx;
    static color_t color_m1;
    static color_t color_m2;
    static color_t color_m3;
    static color_t color_m4;

    static int power_supply;
    static double brightness;
    static bool automatic_timezone;
    static int timezone;
    static bool dnd_active;
    static clock_time_t dnd_start;
    static clock_time_t dnd_end;
    static String ntp;
    static String DNS_name;
    static String ticker;
    static color_t color_tc;
    static int tspeed;
    static int tu;
    static bool viertel;
    static int animation_bell;
    static String animation_type;
    static int last_element;
    static int animation[];
    static int animation_set;
    static int mlength;
    static int atime;
    static int WX;
    static String WXcommand;
    static String apikey;
    static String lat;
    static String lon;
    static void save();
    static void load();
};
#endif
