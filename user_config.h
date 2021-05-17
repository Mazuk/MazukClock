/* Grid rows

   Options
   -------
   10: 10 rows
   11: 11 rows
*/
#define GRID_ROWS 11

/* Grid cols

   Options
   -------
   11: 11 cols
*/
#define GRID_COLS 11

/* Postition of first LED

   Options
   -------
   0: top-left
   1: top-right
   2: buttom-left
   3: buttom-right
*/
#define GRID_FIRST 2

/* Grid language

   Options
   -------
   1: de_DE:     german
   2: de_DE_alt: german alternative
*/
//other grid language is supported by the app. Do not change here!
#define GRID_LANGUAGE de_DE

/* Position of single minute LEDs

   Options
   -------
   0: before, led 0,1,2,3
   1: after, led GRID_ROWS * GRID_COLS + 1,2,3,4
   2: new grid LED 2,4,6,8
   3: not used
*/
#define GRID_SINGLE_MINUTES 2
/* GRID has the GRAD letters between the minutes

    Options
    -------
    0: NO
    1: ENABLED
*/
#define USE_GRAD 0

/* Data PIN, where the LED strip is connected to */
/* Don't use D4 or change setup status LED OFF in main sketch */
#define DATA_PIN D3
