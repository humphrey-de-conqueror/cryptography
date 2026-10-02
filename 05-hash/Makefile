CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g

TARGET = hash

all: $(TARGET)

$(TARGET): hash.c
	$(CC) $(CFLAGS) hash.c -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean