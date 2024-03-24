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

**To get a fancy graph (GNU octave required):**

`./graph.sh`

This will generate a csv file and run an Octave script

**HP-42S version**

I have also written an RPN version for the HP-42S calculator (`eot.42s`).
Enter the day number and run EOT. The result is displayed in register X in minutes and seconds, separated by a decimal point. Make sure you are in degrees mode.

There is also an HP-15C version (`eot.15c`).
