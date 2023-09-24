PROGRAM = engine
OBJS = main.o
CFLAGS =
LIBFLAGS = -lcurses -lm

all:
	make $(PROGRAM)
	make test

clean:
	rm *.o
	rm $(PROGRAM)

test: test.s
	ca65 -t c64 test.s
	ld65 -C /usr/share/cc65/cfg/c64-asm.cfg -o test.prg test.o c64.lib

$(PROGRAM): $(OBJS) fake6502.c
	gcc -o $(PROGRAM) $(CFLAGS) $(OBJS) $(LIBFLAGS)

%.o: %.c
	gcc -o $@ $(CFLAGS) -c $<
