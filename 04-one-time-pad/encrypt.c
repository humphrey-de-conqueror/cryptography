#include <stdio.h>

/* ====================
 * encryption section
 * -> XOR each plaintext byte
 * -> with one unique key byte
 * -> write the ciphertext
 * ==================== */
int main(void)
{
	FILE *input;
	FILE *key;
	FILE *output;
	int plaintext;
	int key_byte;

	/* ====================
	 * file section
	 * -> open plaintext
	 * -> open one-time key
	 * -> create ciphertext
	 * ==================== */
	input = fopen("original.txt", "rb");
	key = fopen("key.bin", "rb");
	output = fopen("encrypted.bin", "wb");

	if (input == NULL || key == NULL || output == NULL)
	{
		printf("Error opening file.\n");
		return 1;
	}

	/* ====================
	 * encryption loop
	 * -> read one plaintext byte
	 * -> read one key byte
	 * -> XOR them
	 * -> write the ciphertext byte
	 * ==================== */
	while ((plaintext = fgetc(input)) != EOF)
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

		fputc(plaintext ^ key_byte, output);
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