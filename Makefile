all: benchmark

CC = g++
OPT = -g -flto -Ofast
CFLAGS = $(OPT) -Wall -march=native
LIBS = -lssl -lcrypto 

benchmark: benchmark.cpp veb.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

test: test.cpp veb.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f benchmark
	rm -f test
