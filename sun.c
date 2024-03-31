/************************************************************/
/* Solar calculator                                         */
/* Copyright (C) 2024 Ellie McNeill. Licensed under GPLv3   */
/* Equations are from The Astronomical Almanac, Section C   */
/* "Low precision formulas for the Sun"                     */
/* Accuracy (1950-2050): R.A./Dec: 1.0'; EOT: 3.5s          */
/************************************************************/

#include "sun.h"

struct sun sun_calc(const float n){ /* n = days since J2000.0 epoch */

    double L;       /* Mean longitude of sun, corrected for aberration */
    double g;       /* Mean anomaly */
    double lambda;  /* Ecliptic longitude */
    double epsilon; /* Obliquity of ecliptic */
    double alpha;   /* Right ascension */
    struct sun sol; /* Structure to return */

    if(fabs(n) > (MAX_DAY)) {
        sol.dist = 0;   /* indicates error condition */
        return sol;
    }

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

    if(lambda > 90)             /* Get alpha into the same */
        alpha = alpha + 180;    /* quadrant as lambda */
    if(lambda > 270)
        alpha = alpha + 180;

    sol.ra = alpha;
    sol.lon = lambda;
    sol.dist = 1.00014 - (0.01671 * (cos(g))) - (0.00014 * cos(2 * g));
    sol.eot = (L - alpha) * 4;

    return sol;
}

double mod(double x, double y){ /* Floating-point modulo function */
    x = (x / y);
    return (x - (int)x) * y;
}
