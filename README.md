Equation of time calculator
---------------------------

Gives the difference between Mean Time and Apparent Solar Time.

https://en.wikipedia.org/wiki/Equation_of_time#Alternative_calculation

Values converted to radians to work with C trig functions

**To compile:**

`gcc eot.c -o eot -lm`

**To run:**

`./eot`

...and enter the day of year (between 1 and 366)


**To get a fancy graph (GNU octave required):**

`./graph.sh`

This will generate a csv file and run an Octave script

** HP-15C version **

I have also written an RPN version for the HP-15C calculator (`eot.rpn`).
Enter the day number and and run program A. The result is displayed in
minutes and seconds, separated by a decimal point.
