#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* Equation of time calculator */
/* Copyright (C) 2024 Ellie McNeill */
/* Gives the difference between Mean Time and Apparent Solar Time */
/* https://en.wikipedia.org/wiki/Equation_of_time#Alternative_calculation */
/* Values converted to radians to work with C trig functions! */

#define ORB_ECC 0.01671 /* Earth's orbital eccentricity */
#define AX_TILT 0.4091  /* Earth's axial tilt in RADIANS */

const float rpd = (2 * M_PI) / 365.24; /* Earth orbit (radians/day) */
float equation_of_time(int);
void graph(void);
int doy;    /* day of year */

int main(int argc){
    float eot;  /* Equation of Time */
    time_t posix;

    if(argc > 1)    /* ANY argument enters graph mode */
        graph();

    time(&posix);   /* Get POSIX time */
    doy = gmtime(&posix)->tm_yday; /* Get day of year */
    doy++;  /* Jan 1st is day 1 */
    eot = equation_of_time(doy);

    /* printf("EOT is %f\n", eot); */
    printf("EOT is %dm %.1fs\n", (int)eot, 60*(eot - (int)eot));
    return 0;
}

float equation_of_time(int doy){
    float a, b, c; /* temp values, taken straight from Wiki example */
    a = (doy + 9) * rpd;    /* Start point is Dec. solstice */
    b = a + (ORB_ECC * 2 * sin ((doy - 3) * rpd));  /* Orbit */
    c = (a - (atan((tan(b))/cos(AX_TILT)))) / M_PI; /* Tilt */
    return 720 * (c - (int)(c + 0.5));  /* 720 mins = 12 hours */
}

void graph(){   /* Generate a list for the whole year */
    for(doy=0;doy<366;doy++)
        printf("%.2f,", equation_of_time(doy));
    printf("\n");
    exit(0);
}
