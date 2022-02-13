CC = gcc
CFLAGS = -g -I -std=gnu99
all: master

master: config.h master.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm master