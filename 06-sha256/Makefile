CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g
LDLIBS = -lcrypto

TARGET = hash

all: $(TARGET)

$(TARGET): hash.c
	$(CC) $(CFLAGS) hash.c -o $(TARGET) $(LDLIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean