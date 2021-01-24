CC = g++
CFLAGS = -c
LFLAGS = -O2 -Wall
OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
SRC = $(patsubst %.cpp, %, $(wildcard *.cpp))
all: $(OBJS)
	$(CC) $(LFLAGS) $^ -o 8
$(OBJS): %.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@
