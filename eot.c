/**********************************************************/
/* Equation of time & solar clock program                 */
/* Copyright (C) 2024 Ellie McNeill. Licensed under GPLv3 */
/* Equations sourced from The Astronomical Almanac:       */
/* https://aa.usno.navy.mil/publications/asa_history      */
/**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "sun.h"

/* User's longitude (negative is west, positive is east) */
#define LONGITUDE -3.75

int main(int argc, const char *argv[]){

    struct ymd date = { 0, 0, 0 };
    const char *signs[] = {"Aries","Taurus","Gemini","Cancer","Leo","Virgo",
        "Libra","Scorpio","Sagittarius","Capricorn","Aquarius","Pisces"};
    double days;        /* Days since 2000-01-01 00:00 */
    struct sun *s;      /* Pointer to the sun paramaters from sun.c */
    float mt, ha, st;   /* Mean Time, Hour Angle, Sidereal Time */
    time_t current;
    short flags = 0;    /* Flags for various options/arguments */
    short i;

    while(--argc) {     /* Only run if there are still arguments */
        if(argv[argc][0] == '-') {
            for(i = 1; argv[argc][i]; i++) {
                switch(argv[argc][i]) {
                case 'g':
                    return graph();
                case 'n':
                    flags = flags | NOW ; break;
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
                    printf("-%c: invalid option\n", argv[argc][i]);
                    return 1;
                }
            }
        } else {
            if(flags & DATE) {
                printf("Please provide only ONE date\n");
                return 1;
            }
            sscanf(argv[argc], "%hd-%hd-%hd", &date.y, &date.m, &date.d);
            if((date.y < 1950 || date.y > 2049)) {
                printf("Please provide a date between 1950 and 2050\n");
                return 1;
            }
            if(checkdate(date)) {
                printf("%d-%d-%d: not a real date\n", date.y, date.m, date.d);
                return 1;
            }
            days = ddays(date);
            flags = flags | DATE | DAY;
        }
    }

    if(!(flags & DATE)) {   /* Only enter this block if no date supplied */
        if((flags & NOW) && (flags & MIDNIGHT)) {
            printf("You cannot combine -m and -n options\n");
            return 1;
        }
        time(&current); /* Get current POSIX time */
        days = (current - Y2K)  / 86400.0 ; /* 86400 secs in a day */
        if(flags & NOW)
            days -= 0.5;        /* Julian days begin at NOON */
        else if(flags & MIDNIGHT)
            days = ((int)days) - 0.5;
        else
            days = (int)days;
    }

    if((flags & DATE) && (flags & MIDNIGHT))
        days -= 0.5;

    if(flags & DATE)
        printf("Date: %d-%d-%d\n", date.y, date.m, date.d);

    if(flags & DAY)
        printf("Day: J2000.0 %+.3f\n", days);

    if(flags & JULIAN)
        printf("JD: %.5f\n", days + 2451545.0);

    s = sun_calc(days);

    if(flags & VERBOSE) {
        printf("R.A.= %dh %.0fm\n", (int)s->ra, conv(s->ra));
        printf("Dec.= %c%ddeg %.fm\n", s->dec < 0 ? '-' : '+',
            abs((int)s->dec), fabs(conv(s->dec)));
        printf("Long= %ddeg %.fm\n", (int)s->lon, conv(s->lon));
        printf("Dist= %.4f AU\n", s->dist);
    }

    if(flags & ZODIAC) {
        printf("%s ", signs[(int)(s->lon/30)]);
        s->lon = mod(s->lon,30);
        printf("%ddeg %.0fm\n", (int)s->lon, conv(s->lon));
    }

    if(flags & TIME) {

        mt = ((days - (int)days) * 24) + 0.00001;
        /* Introducing this slight error ^^^^^^^ makes the seconds  */
        /* work better in the 'live' mode ('rounder' floating point */
        /* numbers causes uneven jumping of the seconds)            */

        ha = mt + ((s->eot) / 60);      /* GHA */
        ha = correct(ha);
        mt -= 12;                       /* GMT */
        mt = correct(mt);
        st = ha + s->ra;                /* GMST */
        st = correct(st);

        ha_ast(ha, 'G');                /* "Greenwich" */
        printf("GMT ");
        printtime(mt, 1);
        printf("GMST");
        printtime(st, 1);

        ha += (LONGITUDE / 15.0);       /* LHA */
        ha = correct(ha);
        mt += (LONGITUDE / 15.0);       /* LMT */
        mt = correct(mt);
        st = ha + s->ra;                /* LMST */
        st = correct(st);

        ha_ast(ha, 'L');                /* "Local" */
        printf("LMT ");
        printtime(mt, 1);
        printf("LMST");
        printtime(st, 1);
        putchar('\n');
    }

    printf("EOT is %c%dm %.0fs\n",  s->eot < 0 ? '-' : '+',
        abs((int)s->eot), fabs(conv(s->eot)));
    return 0;
}

int graph(){   /* Generate list for an entire year */

    struct ymd date = { 0, 1, 1 };  /* Year starts on Jan 1 */
    short i, end;
    time_t current;

    time(&current);
    date.y = (gmtime(&current)->tm_year) + 1900;
    i = ddays(date) - 1;    /* Start date */
    end = i + 366;      /* End date */

    while(i++ < end)
        printf("%.2f,", sun_calc(i)->eot);
    printf("\n");
    return 0;
}

float conv(const float num){
    return ((num - (int)num) * 60);
}

void ha_ast(const float ha, const char c){

    float ast;          /* Apparent Solar Time (AST) */

    putchar('\n');
    putchar(c);
    printf("HA ");
    printtime(ha, 0);
    printf(" (%ddeg %.fm)\n", (int)(ha * 15), conv(ha * 15));

    ast = ha - 12;      /* AST is just the HA +/- 12h (either works) */
    ast = correct(ast);
    putchar(c);
    printf("AST");
    printtime(ast, 1);
}

void printtime(const float hours, const int n){

    short mins, secs;

    mins = (int)conv(hours);
    secs = (int)((conv(hours) - mins) * 60);
    printf("= %02d:%02d:%02d", (int)hours, mins, secs);
    if(n)
        putchar('\n');
}
