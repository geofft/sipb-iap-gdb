CFLAGS=-ggdb3

all: meep

core: meep
	sh -c "ulimit -c unlimited; ./meep -c"
