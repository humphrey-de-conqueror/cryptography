CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g

TARGET = sha256

all: $(TARGET)

$(TARGET): sha256.c
	$(CC) $(CFLAGS) sha256.c -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean