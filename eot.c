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
#include "sun.h"

struct sun sun_calc(float);
void graph(void);
float conv(float);

int main(int argc, char *argv[]){

    float days; /* Days since 2000-01-01 00:00 */
    struct sun s;   /* Store parameters of the sun */
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
            case 'a':
                flags = flags ^ ALL;
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

    s = sun_calc(days);

    if(!s.dist) {   /* Distance should never be 0, so an error occurred */
        printf("Please enter a date between the years 1950 and 2049\n");
        return 1;
    }

    if(flags & ALL) {
        if(s.ra >= 360) {
            s.ra = s.ra - 360;
            s.lon = s.lon - 360;
        }
        s.ra = s.ra / 15;   /* 15 degrees = 1 hour */
        printf("R.A.= %dh %.0fm\n", (int)s.ra, conv(s.ra));
        printf("Dec.= %ddeg %.fm\n", (int)s.dec, fabs(conv(s.dec)));
        printf("Long= %ddeg %.fm\n", (int)s.lon, conv(s.lon));
        printf("Dist= %.4f AU\n", s.dist);
    }

    printf("EOT is %dm %.0fs\n", (int)s.eot, fabs(conv(s.eot)));
    return 0;
}

void graph(){   /* Generate a list for the entire year 2000 */
    short i;
    for(i=0;i<366;i++)
        printf("%.2f,", sun_calc(i).eot);
    printf("\n");
}

float conv(float num){  /* Convert fractional part to minutes or seconds */
    return ((num - (int)num) * 60);
}
