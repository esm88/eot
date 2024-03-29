PREFIX=/usr/local

all:
	cc eot.c sun.c -o eot -lm

astro:
	cc eot.c sun.c -o eot -lm -DASTRO

install:
	cp eot ${PREFIX}/bin/
	chown root:root ${PREFIX}/bin/eot
	cp eot.1 /usr/share/man/man1/
