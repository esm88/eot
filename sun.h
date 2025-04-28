#include <time.h>
#include <math.h>

/* POSIX time at 2000-01-01 00:00 */
#define Y2K 946684800

/* Flag bits */
#define DAY 1
#define NOW 2
#define DATE 4
#define TIME 8
#define JULIAN 16
#define ZODIAC 32
#define VERBOSE 64
#define MIDNIGHT 128
#define ALT 256

struct sun { float ra, dec, lon, dist, eot; };
struct ymd { short y, m, d; };

double mod(double, const double);   /* Floating point modulo function */
int ddays(const struct ymd);        /* Calculates days from 2000-01-01 */
struct sun *sun_calc(const float);  /* Returns pointer to sun struct */
float conv(const float); /* Converts fractional part to mins or secs */
int graph();
void ha_ast(const float, const char); /* Hour Angle & Apparent Solar Time */
void printtime(const float, const int);
float correct(float);               /* Makes sure hours are in range 0-24 */
int checkdate(const struct ymd);    /* Returns 0 if date valid */
