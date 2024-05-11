/*************************************************************************/
/* Equation of time & solar calculator                                   */
/* Copyright (C) 2024 Ellie McNeill. Licensed under GPLv3                */
/* Gives the difference between Local Mean Time and Apparent Solar Time  */
/* Equations sourced from The Astronomical Almanac:                      */
/* https://en.wikipedia.org/wiki/Astronomical_Almanac                    */
/*************************************************************************/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "sun.h"

/* User's longitude (negative is west, positive is east) */
#define LONGITUDE -3.75

struct ymd date = { 0, 1, 1 };  /* Default date is Jan 1 */
float ha, ast;      /* Hour Angle, Apparent Solar Time */
short secs;

int main(int argc, char *argv[]){

    const char *signs[] = {"Aries","Taurus","Gemini","Cancer","Leo","Virgo",
        "Libra","Scorpio","Sagittarius","Capricorn","Aquarius","Pisces"};
    double days;        /* Days since 2000-01-01 00:00 */
    struct sun *s;  /* Pointer to the sun paramaters from sun.c */
    time_t current;
    short flags = 0;    /* Flags for various options/arguments */

    while(--argc) {     /* Only run if there are still arguments */
        if(argv[argc][0] == '-' && isalpha(argv[argc][1])) {
            if(argv[argc][2]) {     /* is not a '\0' */
                printf("I can only handle one option per '-'\n");
                return 1;
            }
            switch(argv[argc][1]) {
            case 'g':
                return graph();
            case 'n':
                flags = flags | NOW | TIME; break;
            case 'j':
                flags = flags | JULIAN; break;
            case 'a':
                flags = flags | VERBOSE | JULIAN | ZODIAC | DAY; break;
            case 'z':
                flags = flags | ZODIAC; break;
            case 'm':
                flags = flags | MIDNIGHT; break;
            case 'v':
                flags = flags | VERBOSE; break;
            case 't':
                flags = flags | TIME; break;
            default:
                printf("-%c: invalid option\n", argv[argc][1]);
                return 1;
            }
        } else {
            if(flags & DATE) {
                printf("Please provide only ONE date\n");
                return 1;
            }
            sscanf(argv[argc], "%d-%d-%d", &date.y, &date.m, &date.d);
            /* HOPE that the user supplies a valid date! */
            if((date.y < 1950 || date.y > 2049)) {
                printf("Please provide a date between 1950 and 2050\n");
                return 1;
            }
            days = ddays();
            flags = flags | DATE;
        }
    }


    if(!(flags & DATE)) {   /* Only enter this block if no date supplied */
        if((flags & NOW) && (flags & MIDNIGHT)) {
            printf("You cannot combine -m and -n options\n");
            return 1;
        }
        time(&current); /* Get current POSIX time */
        days = (current - Y2K) / 86400.0 ; /* 86400 secs in a day */
        if(flags & NOW)
            days = days - 0.5;   /* Julian days begin at NOON */
        else if(flags & MIDNIGHT)
            days = ((int)days) - 0.5;
        else
            days = (int) days;
    }

    if((flags & DATE) && (flags & MIDNIGHT))
        days = days - 0.5;

    if(flags & DATE)
        printf("Date: %d-%d-%d\n", date.y, date.m, date.d);

    if((flags & DATE) || (flags & DAY))
        printf("Day: J2000.0 %+.3f\n", days);

    if(flags & JULIAN)
        printf("JD: %.3f\n", days + 2451545.0);

    s = sun_calc(days);

    if(s->ra >= 360) {
        s->ra = s->ra - 360;
        s->lon = s->lon - 360;
    }

    s->ra = s->ra / 15;   /* 15 degrees = 1 hour */
    if(s->ra < 0)
        s->ra += 24;    /* Fix negative R.A. values */

    if(s->lon < 0)
        s->lon += 360;  /* Fix negative longitude values */

    if(flags & VERBOSE) {
        printf("R.A.= %dh %.0fm\n", (int)s->ra, conv(s->ra));
        printf("Dec.= %+ddeg %.fm\n", (int)s->dec, fabs(conv(s->dec)));
        printf("Long= %ddeg %.fm\n", (int)s->lon, conv(s->lon));
        printf("Dist= %.4f AU\n", s->dist);
    }

    if(flags & TIME) {
        ha = ((days - (int)days) + ((s->eot) / 1440)) * 24;
                    /* 1440 minutes in a day ---^ */
        printtime('G');     /* "Greenwich" */

        ha = ha + (LONGITUDE / 15);    /* 15 degrees per hour rotation */
        if(ha > 24)         /* Correct overflow */
            ha = ha - 24;
        printtime('L');     /* "Local" */
    }

    if(flags & ZODIAC) {
        printf("%s ", signs[(int)(s->lon/30)]);
        s->lon = mod(s->lon,30);
        printf("%ddeg %.0fm\n", (int)s->lon, conv(s->lon));
    }

    printf("EOT is %+dm %.0fs\n", (int)s->eot, fabs(conv(s->eot)));
    return 0;
}

int ddays(){    /* Calculates days from 2000-01-01 */

    time_t posix;
    struct tm intime;

    intime.tm_year = date.y - 1900;
    intime.tm_mon = date.m - 1;
    intime.tm_mday = date.d;
    intime.tm_hour = 0;
    intime.tm_min = 0;
    intime.tm_sec = 0;

    #if defined(__linux__) || defined(__bsd__)
    posix = timegm(&intime);    /* Prefer timegm() to mktime() */
    /* timegm: non-standard GNU extension. Also present on the BSDs. */
    #else
    posix = mktime(&intime);    /* Fallback if timegm not available */
    #endif

    return((posix - Y2K) / 86400);
}

int graph(){   /* Generate list for an entire year */

    short i, end;

    fprintf(stderr, "Year? ");  /* stderr prevents redirection into file */
    scanf("%d", &date.y);
    if((date.y < 1950 || date.y > 2049)) {
        fprintf(stderr, "Enter a year between 1950 and 2049\n");
        return 1;
    }
    i = ddays();        /* Start date */
    end = i + 366;      /* End date */

    while(i++ < end)
        printf("%.2f,", sun_calc(i)->eot);
    printf("\n");
    return 0;
}

float conv(const float num){
    return ((num - (int)num) * 60);
}

void printtime(char c){

    if(ha < 0)          /* Correct underflow */
        ha = ha + 24;
    secs = (int)((conv(ha) - (int)conv(ha)) * 60);
    putchar(c);
    printf("HA = %02d:%02d:%02d ", (int)ha, (int)conv(ha), secs);
    printf("(%ddeg %.fm)\n", (int)(ha * 15), conv(ha * 15));
        /* 1 hour = 15 degrees */

    ast = ha - 12;      /* Apparent Solar Time (AST) */
    if(ast < 0)
        ast = ast + 24;
    putchar(c);
    printf("AST= %02d:%02d:%02d\n\n", (int)ast, (int)conv(ast), secs);
}
