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
#include <stdlib.h>
#include <ctype.h>

#define Y2K 946684800       /* POSIX time at 2000-01-01 00:00 */
#define DATE 1
#define EXACT 2
#define JULIAN 4

float equation_of_time(float);
void graph(void);

int main(int argc, char *argv[]){

    float days; /* Days since 2000-01-01 00:00 */
    float eot;  /* Equation of Time */
    float secs;
    time_t current;
    short flags = 0;    /* Flags for -j, -e, DATE input */

    while(--argc) {     /* Only run if there are still arguments */
        if(argv[argc][0] == '-' && isalpha(argv[argc][1])) {
            switch(argv[argc][1]) {
            case 'g':
                graph();
                return 0;
            case 'e':
                flags = flags ^ EXACT;
                break;
            case 'j':
                flags = flags ^ JULIAN;
                break;
            default:
                printf("-%c: invalid option\n", argv[argc][1]);
                return 1;
            }
        } else {
            if(flags & DATE) {
                printf("Please provide only ONE date\n");
                return 1;
            }
            days = atof(argv[argc]);    /* ASSUME it's a number */
            flags = flags ^ DATE;
        }
    }

    if(!(flags & DATE)) {
        time(&current); /* Get current POSIX time */
        days = (current - Y2K) / 86400.0 ; /* 86400 secs in a day */
        if(flags & EXACT)
            days = days -0.5;   /* Julian days begin at NOON */
        else
            days = (int) days;
    } else {
        printf("Day: J2000.0 %+.3f\n", days);
    }

    #ifdef DEBUG
    printf("Days= %.3f\n", days);
    #endif

    if(flags & JULIAN)
        printf("JD: %.3f\n", days + 2451545.0);

    eot = equation_of_time(days);

    secs = 60*(eot - (int)eot);
    if(secs < 0)
        secs = -secs;
    printf("EOT is %dm %.0fs\n", (int)eot, secs);
    return 0;
}

void graph(){   /* Generate a list for the entire year 2000 */
    short i;
    for(i=0;i<366;i++)
        printf("%.2f,", equation_of_time(i));
    printf("\n");
}
