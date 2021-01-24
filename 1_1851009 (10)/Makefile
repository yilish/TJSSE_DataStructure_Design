CC = g++
CFLAGS = -c
LFLAGS = 
OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
SRC = $(patsubst %.cpp, %, $(wildcard *.cpp))
all: $(OBJS)
	$(CC) $(LFLAGS) $^ -o 10
$(OBJS): %.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@
