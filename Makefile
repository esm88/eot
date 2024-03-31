PREFIX=/usr/local

all:
	cc eot.c sun.c ddays.c -o eot -lm

install:
	cp eot ${PREFIX}/bin/
	chown root:root ${PREFIX}/bin/eot
	cp eot.1 /usr/share/man/man1/
