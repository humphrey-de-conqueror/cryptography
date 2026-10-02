CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g

TARGETS = generate_key encrypt decrypt

all: $(TARGETS)

generate_key: generate_key.c
	$(CC) $(CFLAGS) generate_key.c -o generate_key

encrypt: encrypt.c
	$(CC) $(CFLAGS) encrypt.c -o encrypt

decrypt: decrypt.c
	$(CC) $(CFLAGS) decrypt.c -o decrypt

clean:
	rm -f $(TARGETS) key.bin encrypted.bin decrypted.txt

.PHONY: all clean