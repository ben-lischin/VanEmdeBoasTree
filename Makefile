all: test

CC = g++
OPT= -g -flto -Ofast
CFLAGS = $(OPT) -Wall -march=native
LIBS = -lssl -lcrypto 

test: test.cc
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f test test.o
