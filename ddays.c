#include <stdio.h>
#include <time.h>
#include "sun.h"

int ddays(const struct ymd *date){
    time_t posix;
    struct tm intime;

    intime.tm_year = date->y - 1900;
    intime.tm_mon = date->m - 1;
    intime.tm_mday = date->d;
    intime.tm_hour = 0;
    intime.tm_min = 0;
    intime.tm_sec = 0;

    posix = timegm(&intime);
    /* timegm: non-standard GNU extension. Also present on the BSDs. */

    #ifdef DEBUG
    printf("Input time is: %s", asctime(&intime));
    printf("POSIX: %ld\n", posix);
    printf("Days is %f\n", (posix - Y2K) / 86400.0);
    #endif

    return((posix - Y2K) / 86400);
}
