/************************************************************/
/* Solar calculator                                         */
/* Copyright (C) 2024 Ellie Neills. Licensed under GPLv3    */
/* Equations are from The Astronomical Almanac, Section C   */
/* "Low precision formulas for the Sun"                     */
/* Accuracy (1950-2050): R.A./Dec: 1.0'; EOT: 3.5s          */
/************************************************************/

#include "sun.h"

#define DPR (180.0 / 3.14159265)    /* Degrees per radian */

struct sun sol; /* Stores sun parameters */

struct sun *sun_calc(const float n){ /* n = days since J2000.0 epoch */

    double L;       /* Mean longitude of sun, corrected for aberration */
    double g;       /* Mean anomaly */
    double lambda;  /* Ecliptic longitude */
    double epsilon; /* Obliquity of ecliptic */
    double alpha;   /* Right ascension */

    L = 280.460 + 0.9856474 * n;
    L = mod(L, 360);    /* Put L in range 0 - 360 */
    L = L / DPR;        /* Convert L to radians */

    g = 357.528 + 0.9856003 * n;
    g = mod(g, 360);    /* Put g in range 0 - 360 */
    g = g / DPR;        /* Convert g to radians */

    lambda = L + ((1.915 / DPR) * sin(g)) + ((0.020 / DPR) * sin(2 * g));

    L = L * DPR;    /* Convert L back to degrees */

    epsilon = (23.439 / DPR) - (0.0000004 / DPR) * n;

    sol.dec = (asin(sin(epsilon) * sin(lambda))) * DPR; /* Declination */

    alpha = atan(cos(epsilon) * tan(lambda));
    lambda = (lambda * DPR);    /* Convert lambda to degrees */
    alpha = (alpha * DPR);      /* Convert alpha to degrees */

    if(lambda < -90)            /* Get alpha into the same */
        alpha = alpha - 180;    /* quadrant as lambda */
    if(lambda < -270)
        alpha = alpha - 180;

    if(lambda > 90)             /* Get alpha into the same */
        alpha = alpha + 180;    /* quadrant as lambda */
    if(lambda > 270)
        alpha = alpha + 180;

    sol.ra = alpha;
    sol.lon = lambda;
    sol.dist = 1.00014 - (0.01671 * (cos(g))) - (0.00014 * cos(2 * g));
    sol.eot = (L - alpha) * 4;

    sol.ra /= 15;           /* 15 degrees = 1 hour */
    sol.ra = correct(sol.ra);
    sol.lon /= 15;          /* correct() needs hours, not degrees */
    sol.lon = correct(sol.lon);
    sol.lon *= 15;

    return &sol;
}

double mod(double x, const double y){ /* Floating-point modulo function */
    x = (x / y);
    return (x - (int)x) * y;
}

int ddays(const struct ymd date){    /* Calculates days from 2000-01-01 */

    time_t posix;
    struct tm intime;

    intime.tm_year = date.y - 1900;
    intime.tm_mon = date.m - 1;
    intime.tm_mday = date.d;
    intime.tm_hour = 0;
    intime.tm_min = 0;
    intime.tm_sec = 0;
    intime.tm_isdst = 0;

    #if defined(__linux__) || defined(__bsd__)
        posix = timegm(&intime);    /* Prefer timegm() to mktime() */
        /* timegm: non-standard GNU extension. Also present on the BSDs. */
    #else
        /* Fallback if timegm not available */
        posix = mktime(&intime) - timezone;
    #endif

    return((posix - Y2K) / 86400);
}

float correct(float hours){     /* Fix out-of-range values */

    if(hours < 24)
        hours += 24;
    if(hours > 24)
        hours -= 24;
    return hours;
}

int checkdate(const struct ymd date){ /* Returns 0 if valid, 1 otherwise */
    if(date.d < 1)
        return 1;
    switch(date.m) {
    case 2:             /* Feb has 28 days, or 29 if leap-year */
        if(date.d > 29)
            return 1;
        if((date.y % 4) && (date.d > 28))   /* non-leap year */
            return 1;                       /* WRONG for 1900/2100 etc. */
        return 0;
    case 4:
    case 6:
    case 9:
    case 11:
        if(date.d > 30) /* Apr/Jun/Sep/Nov have 30 days */
            return 1;
        return 0;
    case 1:             /* All the rest have 31 days */
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if(date.d > 31)
            return 1;
        return 0;
    default:            /* Error: month <1 or >12 */
        return 1;
    }
}
