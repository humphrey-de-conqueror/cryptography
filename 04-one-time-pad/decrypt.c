#include <stdio.h>

/* ====================
 * decryption section
 * -> XOR each ciphertext byte
 * -> with the corresponding key byte
 * -> recover the plaintext
 * ==================== */
int main(void)
{
	FILE *input;
	FILE *key;
	FILE *output;
	int ciphertext;
	int key_byte;

	/* ====================
	 * file section
	 * -> open ciphertext
	 * -> open the same one-time key
	 * -> create plaintext
	 * ==================== */
	input = fopen("encrypted.bin", "rb");
	key = fopen("key.bin", "rb");
	output = fopen("decrypted.txt", "wb");

	if (input == NULL || key == NULL || output == NULL)
	{
		printf("Error opening file.\n");
		return 1;
	}

	/* ====================
	 * decryption loop
	 * -> read one ciphertext byte
	 * -> read one key byte
	 * -> XOR them
	 * -> recover the plaintext byte
	 * ==================== */
	while ((ciphertext = fgetc(input)) != EOF)
	{
		key_byte = fgetc(key);

		if (key_byte == EOF)
		{
			printf("Key is too short.\n");

			fclose(input);
			fclose(key);
			fclose(output);

			return 1;
		}

		fputc(ciphertext ^ key_byte, output);
	}

	/* ====================
	 * cleanup section
	 * -> close all files
	 * ==================== */
	fclose(input);
	fclose(key);
	fclose(output);

	return 0;
}