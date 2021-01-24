CC = g++
CFLAGS = -c
LFLAGS = 
OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
SRC = $(patsubst %.cpp, %, $(wildcard *.cpp))
all: $(OBJS)
	$(CC) $(LFLAGS) $^ -o 5
$(OBJS): %.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@
