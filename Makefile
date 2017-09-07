CFLAGS  = -std=c99 -pedantic -Wall
LDFLAGS = -lX11

all: xkblayout

xkblayout: xkblayout.c
	$(CC) $(CFLAGS) $(LDFLAGS) xkblayout.c -o xkblayout

clean:
	rm -f xkblayout

.PHONY: clean
