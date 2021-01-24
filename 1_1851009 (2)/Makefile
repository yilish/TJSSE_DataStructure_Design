CC = g++
CFLAGS = -c
LFLAGS = -O2 -Wall
OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
SRC = $(patsubst %.cpp, %, $(wildcard *.cpp))
all: $(OBJS)
	$(CC) $(LFLAGS) $^ -o 2
$(OBJS): %.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@
