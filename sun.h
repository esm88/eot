#include <math.h>
#include <stdlib.h>

/* POSIX time at 2000-01-01 00:00 */
#define Y2K 946684800

/* Flag bits */
#define DAY 1
#define NOW 2
#define DATE 4
#define JULIAN 8
#define ZODIAC 16
#define VERBOSE 32
#define MIDNIGHT 64
#define TIME 128

#define DPR (180.0 / 3.14159265)    /* Degrees per radian */

struct sun { float ra, dec, lon, dist, eot; };
struct ymd { int y, m, d; };

double mod(double, double);     /* Floating point modulo function */
int ddays();    /* Calculates days from 2000-01-01 */
struct sun *sun_calc(const float);
float conv(const float); /* Convert fractional part to mins or secs */
int graph();
void printtime(char);
