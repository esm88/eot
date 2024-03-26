/*************************************************************************/
/* Equation of time calculator                                           */
/* Copyright (C) 2024 Ellie McNeill. Licensed under GPLv3                */
/* Gives the difference between Local Mean Time and Apparent Solar Time  */
/* Equations sourced from The Astronomical Almanac:                      */
/* https://en.wikipedia.org/wiki/Astronomical_Almanac                    */
/*************************************************************************/

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define Y2K 946684800       /* POSIX time at 2000-01-01 00:00 */
#define DPR (180.0 / M_PI)  /* Degrees per radian */

float equation_of_time(float);
double mod(double, double); /* Floating-point modulo function */
void graph(void);

int main(int argc, char *argv[]){

    float days; /* Days since 2000-01-01 00:00 */
    float eot;  /* Equation of Time */
    float secs;
    time_t current;
    int exact = 0;

    if(argc > 1)
    if(!strcmp(argv[1],"-g")) {
        graph();
        return 0;
    } else if(!strcmp(argv[1],"-e")) {
        exact = 1;
    } else {
        printf("%s: invalid option\n", argv[1]);
        return 1;
    }

    time(&current);                 /* Get current POSIX time */
    days = (current - Y2K) / 86400.0 ; /* 86400 secs in a day */
    if(exact)
        days = days -0.5;   /* Julian days begin at NOON */
    else
        days = (int) days;

    printf("Days: %f\n", days);

    eot = equation_of_time(days);

    secs = 60*(eot - (int)eot);
    if(secs < 0)
        secs = -secs;
    printf("EOT is %dm %.0fs\n", (int)eot, round(secs));
    return 0;
}


float equation_of_time(float n){

/*******************************************/
/* Source: Astronomical Almanac, Section C */
/* "Low precision formulas for the Sun"    */
/* "yields a precision better than 3.5s    */
/*  between the years 1950 and 2050."      */
/*******************************************/

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

    alpha = atan(cos(epsilon) * tan(lambda));
    lambda = (lambda * DPR);    /* Convert lambda to degrees */
    alpha = (alpha * DPR);      /* Convert alpha to degrees */

    if(lambda > 90)             /* Get alpha into the same */
        alpha = alpha + 180;    /* quadrant as lambda */
    if(lambda > 270)
        alpha = alpha + 180;

    return (L - alpha) * 4;
}

void graph(){   /* Generate a list for the entire year 2000 */
    int i;
    for(i=0;i<366;i++)
        printf("%.2f,", equation_of_time(i));
    printf("\n");
}

double mod(double x, double y){
    x = (x / y);
    return (x - (int)x) * y;
}
