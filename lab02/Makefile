CC=gcc
CFLAGS= -g -Wall
OBJS= prob05.c
DEPS= lab02.h
TARGET= lab02
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

clean:
	rm -f $(OBJS) $(TARGET)
.c.o: $(DEPS)
	$(CC) -c $< $(CFLAGS)