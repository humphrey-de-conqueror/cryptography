#include <stdio.h>

#define SHIFT 3

int main(void)
{
	FILE *input;
	FILE *output;
	int c;

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
	 * decryption section
	 * -> loop through each byte
	 * -> reverse the Caesar shift
	 * -> leave other characters unchanged
	 * ==================== */
	while ((c = fgetc(input)) != EOF)
	{
		/* ====================
		 * uppercase section
		 * -> convert A-Z into the range 0-25
		 * -> subtract the shift
		 * -> handle alphabet wrapping
		 * -> convert back into A-Z
		 * ==================== */
		if (c >= 'A' && c <= 'Z')
		{
			c = ((c - 'A' - SHIFT + 26) % 26) + 'A';
		}

		/* ====================
		 * lowercase section
		 * -> convert a-z into the range 0-25
		 * -> subtract the shift
		 * -> handle alphabet wrapping
		 * -> convert back into a-z
		 * ==================== */
		else if (c >= 'a' && c <= 'z')
		{
			c = ((c - 'a' - SHIFT + 26) % 26) + 'a';
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