CFLAGS=-ggdb3

all: meep

meep: meep.c collatz.c

core: meep
	-sh -c "ulimit -c unlimited; ./meep -c"

clean:
	-rm meep core
