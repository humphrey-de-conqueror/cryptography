#include <stdio.h>
#include <string.h>

#define KEY "secret"

/* ====================
 * decryption section
 * -> read the ciphertext one byte at a time
 * -> XOR each byte with the same key byte
 * -> recover the original plaintext
 * ==================== */
int main(void)
{
	FILE *input;
	FILE *output;
	int c;
	int key_index;
	int key_length;

	key_index = 0;
	key_length = strlen(KEY);

	/* ====================
	 * file section
	 * -> open ciphertext in binary mode
	 * -> open plaintext in binary mode
	 * ==================== */
	input = fopen("encrypted.bin", "rb");
	output = fopen("decrypted.txt", "wb");

	if (input == NULL || output == NULL)
	{
		printf("Error opening file.\n");
		return 1;
	}

	/* ====================
	 * XOR decryption loop
	 * -> read one ciphertext byte
	 * -> XOR it with the same key byte
	 * -> recover the plaintext byte
	 * -> repeat the key
	 * ==================== */
	while ((c = fgetc(input)) != EOF)
	{
		c = c ^ KEY[key_index];

		fputc(c, output);

		key_index++;

		if (key_index == key_length)
		{
			key_index = 0;
		}
	}

	/* ====================
	 * cleanup section
	 * -> close both files
	 * ==================== */
	fclose(input);
	fclose(output);

	return 0;
}