#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ====================
 * configuration section
 * -> define the key length
 * -> the key must be at least as long as the plaintext
 * ==================== */
#define KEY_LENGTH 256

/* ====================
 * key generation section
 * -> generate random bytes
 * -> write them directly into the key file
 * ==================== */
int main(void)
{
	FILE *output;
	int i;
	int random_byte;

	/* ====================
	 * file section
	 * -> create the key as binary data
	 * ==================== */
	output = fopen("key.bin", "wb");

	if (output == NULL)
	{
		printf("Error opening key file.\n");
		return 1;
	}

	/* ====================
	 * random section
	 * -> seed the pseudo-random generator
	 * -> generate one byte at a time
	 * ==================== */
	srand((unsigned int)time(NULL));

	for (i = 0; i < KEY_LENGTH; i++)
	{
		random_byte = rand() % 256;
		fputc(random_byte, output);
	}

	/* ====================
	 * cleanup section
	 * -> close the key file
	 * ==================== */
	fclose(output);

	return 0;
}