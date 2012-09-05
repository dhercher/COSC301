CC=gcc
CFLAGS= -g -Wall
OBJS= lab01.o main.o
DEPS= lab01.h
TARGET= lab01
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@
.c.o: $(DEPS)
	$(CC) -c $< $(CFLAGS)