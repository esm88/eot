Equation of time calculator
---------------------------

Gives the difference between Local Mean Time and Apparent Solar Time.

Equations sourced from The Astronomical Almanac:

https://en.wikipedia.org/wiki/Astronomical_Almanac

"Yields a precision better than 3.5s between the years 1950 and 2050."

**To compile:**

`$ make`

**To install:**

`# make install`

**To run:**

`$ eot`

To get more parameters of the sun (right ascension, declination, ecliptic longitude, distance):

`$ eot -v`

**To get a fancy graph (GNU octave required):**

`./graph.sh`

This will generate a csv file and run an Octave script

**For a live clock:**

`./live.sh`

This will give a running clock of the currrent GMT/LMT, GHA/LHA, GAST/LAST, GMST/LMST and EOT

Note: 'local' is based on the longitude as defined by the LONGITUDE constant. Please change to your **own** value (negative values are west of Greenwich).

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
