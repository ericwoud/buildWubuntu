CC      = gcc
CFLAGS  = -g
RM      = rm -f


default: all

all: linksys_bootcount

linksys_bootcount: linksys_bootcount.c
	$(CC) $(CFLAGS) -o linksys_bootcount linksys_bootcount.c

clean veryclean:
	$(RM) linksys_bootcount
