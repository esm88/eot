#include <math.h>
#include <stdlib.h>

/* POSIX time at 2000-01-01 00:00 */
#define Y2K 946684800

/* Flag bits */
#define DAY 1
#define ALL 2
#define DATE 4
#define JULIAN 8
#define ZODIAC 16
#define VERBOSE 32
#define MIDNIGHT 64
#define EXACT 128

#define DPR (180.0 / 3.14159265)    /* Degrees per radian */
#define MAX_DAY 18262               /* Max days from J2000.0 */

struct sun { float ra, dec, lon, dist, eot; };

double mod(double, double);
struct sun sun_calc(const float);
