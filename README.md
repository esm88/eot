Equation of time & solar clock program
--------------------------------------

Gives the difference between Local Mean Time and Apparent Solar Time.

The program also gives various other parameters of the sun.

It can also function as a clock, displaying 8 different kinds of time.

Equations sourced from [The Astronomical Almanac][1].

![Octave](octave.png "Octave graph showing the equation of time")

**To install:**

First, you should change the `LONGITUDE` constant in `eot.c` to your _own_ longitude. The clock mode uses this to calculate the local times.

You can then compile, install, and run with:

```
$ make
# make install
$ eot
```

**To get a fancy graph (GNU Octave required):**

`./graph.sh`

This will generate a csv file and run an Octave script, placing an 'x' on the current date (example image above).

**For a live clock:**

`./live.sh`

This will give a running clock of the currrent JD, GMT/LMT, GHA/LHA, GAST/LAST, GMST/LMST and EOT.

Note: 'local' is based on the longitude as defined by the LONGITUDE constant. Please change to your _own_ value (negative values are west of Greenwich).

**Free42 version**

I have also written an RPN version for the Free42 calculator (`eot.42s`). To get the sun parameters for today, enter **0** and execute SUN. If you want a specific date, enter the date in `YYYY.MMDD` format, for example: `2024.0401` for April 1st, 2024. Execute SUN.

The results are displayed in both register X and Y, using CMPLX mode. This allows 4 parameters to be shown at once. Register X shows the right ascension and the angle part shows the declination. Register Y shows the ecliptic longitude and the angle part is the equation of time. Although decimal points are shown, the results are actually in the following units:

```
R.A.: hours.minutes
Dec.: degrees.minutes
Long: degrees.minutes
EOT : minutes.seconds
```

Here is an example:

```
Y: 12.15 ∠-3.43
X: 0.45 ∠4.50
```

Here, the parameters are 0h 45m right ascension, 4° 50m declination, 12° 15m ecliptic longitude, and the equation of time is -3m 43s.

**FLAG 10** controls whether the calculations will be for noon (12h) or midnight (00h). For noon, flag 10 should be **clear** (`CF 10`). For midnight, flag 10 should be **set** (`SF 10`).

Note that the Free42 version uses the built in DDAYS function. The 'current' date will be based on your local time rather than GMT.

The Free42 version will not work with the original HP-42S as it requires the TIME functions. It will however work on a DM42. Make sure you are in DEGREES mode!

I have also included the binary file **eot.raw** which can be imported to Free42. This saves having to manually enter the program.

There is also an HP-15C version (`eot.15c`). This uses a simpler algorithm. Enter the day number (of year) and run LBL A. It is not as accurate as the C and Free42 versions.

**Notes:**

The program has been tested on Debian 12 (amd64) and IRIX 5.3 (mips). On IRIX 5.3, the program fails on dates before 1970-01-01 and after 2038-01-19.

The equations used for the `sun_calc()` function were taken from the 2016 edition of The Astronomical Almanac, page C5 ("Low precision formulas for the Sun").

The claimed precision (for 1950 ~ 2050):

- better than 3.5s (for EOT)
- better than 1.0' (for R.A./Dec)
- better than 0.0003 au (for distance)

Note that the program actually uses **UTC**, not GMT. The term 'GMT' is often used to mean UTC, but _true_ GMT is actually UT1. UTC is currently kept within 0.9s of UT1 through the introduction of leap seconds (DUT1 is the difference). This is likely to change as leap seconds are to be abolished by 2035.

Additionally, the modern prime meridian (based on WGS84) is actually 5.3" east of the Airy Transit Circle at Greenwich Royal Observatory.

I consider these inaccuracies negligible. If you require high precision for astronomy, consider other programs such as [XEphem](https://github.com/XEphem/XEphem) or [Cartes du Ciel](http://www.ap-i.net/skychart/)

If you discover any bugs in the program, please report them to me: github.esm.me.uk

[1]: https://aa.usno.navy.mil/publications/asa_history
