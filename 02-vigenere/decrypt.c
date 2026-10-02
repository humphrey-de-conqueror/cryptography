#include <stdio.h>
#include <string.h>

#define KEY "KEY"

/* ====================
 * decryption section
 * -> decrypt each alphabetic character
 * -> subtract the corresponding key shift
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
	 * -> open the ciphertext for reading
	 * -> open the decrypted file for writing
	 * ==================== */
	input = fopen("encrypted.txt", "r");
	output = fopen("decrypted.txt", "w");

	if (input == NULL || output == NULL)
	{
		printf("Error opening file.\n");
		return 1;
	}

	/* ====================
	 * decryption loop
	 * -> read one byte at a time
	 * -> decrypt alphabetic characters
	 * -> preserve spaces and punctuation
	 * ==================== */
	while ((c = fgetc(input)) != EOF)
	{
		/* ====================
		 * uppercase section
		 * -> convert the ciphertext letter to 0-25
		 * -> subtract the key value
		 * -> add 26 to prevent a negative value
		 * -> wrap around the alphabet
		 * ==================== */
		if (c >= 'A' && c <= 'Z')
		{
			c = ((c - 'A') -
			     (KEY[key_index] - 'A') + 26) % 26 + 'A';

			key_index++;

			if (key_index == key_length)
			{
				key_index = 0;
			}
		}

		/* ====================
		 * lowercase section
		 * -> convert the ciphertext letter to 0-25
		 * -> subtract the key value
		 * -> add 26 to prevent a negative value
		 * -> wrap around the alphabet
		 * ==================== */
		else if (c >= 'a' && c <= 'z')
		{
			c = ((c - 'a') -
			     (KEY[key_index] - 'A') + 26) % 26 + 'a';

			key_index++;

			if (key_index == key_length)
			{
				key_index = 0;
			}
		}

		/* ====================
		 * output section
		 * -> write the decrypted byte
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