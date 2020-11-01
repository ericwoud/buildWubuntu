CC      = gcc
CCX     = arm-linux-gnueabihf-gcc
RM      = rm -f

default: all

all: linksys_bootcount

linksys_bootcount: linksys_bootcount.c
	$(CC) -o linksys_bootcount linksys_bootcount.c

linksys_bootcount_cross: linksys_bootcount.c
	$(CCX) -o linksys_bootcount linksys_bootcount.c

clean veryclean:
	$(RM) linksys_bootcount
