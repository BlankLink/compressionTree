
CODES= main.cpp CompressionTree.cpp Leaf.cpp
OBJECTS= $(CODES:%.cpp=tmp/%.o)
SOURCES= $(CODES:%=src/%)
CC=g++
EXE= bin/compressorr
CFLAGS= -c -std=c++11 -Wall -I include/

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

tmp/%.o: src/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm tmp/*
	rm bin/*
