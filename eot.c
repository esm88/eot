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

float equation_of_time(float);
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

    /* scanf("%f", &days); */
    #ifdef DEBUG
    printf("Days= %.3f\n", days);
    #endif

    eot = equation_of_time(days);

    secs = 60*(eot - (int)eot);
    if(secs < 0)
        secs = -secs;
    printf("EOT is %dm %.0fs\n", (int)eot, secs);
    return 0;
}

void graph(){   /* Generate a list for the entire year 2000 */
    int i;
    for(i=0;i<366;i++)
        printf("%.2f,", equation_of_time(i));
    printf("\n");
}
