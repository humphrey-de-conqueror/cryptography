#include <stdio.h>
#include <string.h>

#define KEY "secret"

/* ====================
 * encryption section
 * -> read the input one byte at a time
 * -> XOR each byte with the corresponding key byte
 * -> repeat the key when necessary
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
	 * -> open plaintext in binary mode
	 * -> open ciphertext in binary mode
	 * ==================== */
	input = fopen("original.txt", "rb");
	output = fopen("encrypted.bin", "wb");

	if (input == NULL || output == NULL)
	{
		printf("Error opening file.\n");
		return 1;
	}

	/* ====================
	 * XOR encryption loop
	 * -> read one byte
	 * -> XOR it with one key byte
	 * -> write the resulting byte
	 * -> advance and repeat the key
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