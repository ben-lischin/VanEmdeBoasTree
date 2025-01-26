all: test benchmark

CC = g++
OPT = -g -flto -Ofast
CFLAGS = $(OPT) -Wall -march=native
LIBS = -lssl -lcrypto 

test: test.cpp veb.cpp
	$(CC) -g $^ -o $@ -lm

benchmark: benchmark.cpp veb.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f test
	rm -f benchmark
