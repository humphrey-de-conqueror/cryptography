CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g

TARGETS = encrypt decrypt

all: $(TARGETS)

encrypt: encrypt.c
	$(CC) $(CFLAGS) encrypt.c -o encrypt

decrypt: decrypt.c
	$(CC) $(CFLAGS) decrypt.c -o decrypt

clean:
	rm -f $(TARGETS) encrypted.txt decrypted.txt

.PHONY: all clean