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

`$eot -v`

**To get a fancy graph (GNU octave required):**

`./graph.sh`

This will generate a csv file and run an Octave script

**Free42 version**

I have also written an RPN version for the Free42 calculator (`eot.42s`). Execute EOT. The result is displayed in register X in minutes and seconds, separated by a decimal point. Make sure you are in degrees mode.

Note: This version will not work with the original HP-42S as it requires the TIME functions. It will however work on a DM42.

There is also an HP-15C version (`eot.15c`). This uses a simpler algorithm. Enter the day number (of year) and run LBL A. It is not as accurate as the C and Free42 versions.
