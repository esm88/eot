/************************************/
/* Equation of time calculator      */
/* Copyright (C) 2024 Ellie McNeill */
/* Licensed under GPLv3             */
/************************************/

#include <math.h>
#include <stdio.h>

#define DPR (180.0 / 3.14159)    /* Degrees per radian */

double mod(double x, double y){     /* Floating-point modulo function */
    x = (x / y);
    return (x - (int)x) * y;
}

float equation_of_time(float n){

/*******************************************/
/* Source: Astronomical Almanac, Section C */
/* "Low precision formulas for the Sun"    */
/* "yields a precision better than 3.5s    */
/*  between the years 1950 and 2050."      */
/* INPUT: Days since J2000.0 epoch         */
/* OUTPUT: Equation of Time (minutes)      */
/*******************************************/

    double L;       /* Mean longitude of sun, corrected for aberration */
    double g;       /* Mean anomaly */
    double lambda;  /* Ecliptic longitude */
    double epsilon; /* Obliquity of ecliptic */
    double alpha;   /* Right ascension */
    float delta;    /* Declination */

    L = 280.460 + 0.9856474 * n;
    L = mod(L, 360);    /* Put L in range 0 - 360 */
    L = L / DPR;        /* Convert L to radians */

    g = 357.528 + 0.9856003 * n;
    g = mod(g, 360);    /* Put g in range 0 - 360 */
    g = g / DPR;        /* Convert g to radians */

    lambda = L + ((1.915 / DPR) * sin(g)) + ((0.020 / DPR) * sin(2 * g));

    L = L * DPR;    /* Convert L back to degrees */

    epsilon = (23.439 / DPR) - (0.0000004 / DPR) * n;

    alpha = atan(cos(epsilon) * tan(lambda));
    lambda = (lambda * DPR);    /* Convert lambda to degrees */
    alpha = (alpha * DPR);      /* Convert alpha to degrees */

    if(lambda > 90)             /* Get alpha into the same */
        alpha = alpha + 180;    /* quadrant as lambda */
    if(lambda > 270)
        alpha = alpha + 180;

    return (L - alpha) * 4;
}
