#include <stdio.h>
#include <string.h>

#define KEY "KEY"

/* ====================
 * encryption section
 * -> encrypt each alphabetic character
 * -> use a different shift based on the key
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
	 * -> open the plaintext for reading
	 * -> open the ciphertext for writing
	 * ==================== */
	input = fopen("original.txt", "r");
	output = fopen("encrypted.txt", "w");

	if (input == NULL || output == NULL)
	{
		printf("Error opening file.\n");
		return 1;
	}

	/* ====================
	 * encryption loop
	 * -> read one byte at a time
	 * -> encrypt alphabetic characters
	 * -> preserve spaces and punctuation
	 * ==================== */
	while ((c = fgetc(input)) != EOF)
	{
		/* ====================
		 * uppercase section
		 * -> convert the plaintext letter to 0-25
		 * -> convert the key letter to 0-25
		 * -> add both values
		 * -> wrap around the alphabet
		 * ==================== */
		if (c >= 'A' && c <= 'Z')
		{
			c = ((c - 'A') +
			     (KEY[key_index] - 'A')) % 26 + 'A';

			key_index++;

			if (key_index == key_length)
			{
				key_index = 0;
			}
		}

		/* ====================
		 * lowercase section
		 * -> convert the plaintext letter to 0-25
		 * -> convert the key letter to 0-25
		 * -> add both values
		 * -> wrap around the alphabet
		 * ==================== */
		else if (c >= 'a' && c <= 'z')
		{
			c = ((c - 'a') +
			     (KEY[key_index] - 'A')) % 26 + 'a';

			key_index++;

			if (key_index == key_length)
			{
				key_index = 0;
			}
		}

		/* ====================
		 * output section
		 * -> write the encrypted byte
		 * ==================== */
		fputc(c, output);
	}

	/* ====================
	 * cleanup section
	 * -> close both files
	 * ==================== */
	fclose(input);
	fclose(output);

	return 0;
}