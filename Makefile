PREFIX=/usr/local

all:
	cc eot.c sun.c -o eot -lm

install:
	cp -i eot ${PREFIX}/bin/
	chown root:root ${PREFIX}/bin/eot
	cp -i eot.1 /usr/share/man/man1/
