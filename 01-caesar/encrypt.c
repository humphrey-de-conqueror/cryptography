#include <stdio.h>

/* ====================
 * configuration section
 * -> define the encryption key
 * ==================== */
#define SHIFT 3

int main(void)
{
	FILE *input;
	FILE *output;
	int c;

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
	 * encryption section
	 * -> loop through each byte
	 * -> shift alphabetic characters
	 * -> leave other characters unchanged
	 * ==================== */
	while ((c = fgetc(input)) != EOF)
	{
		/* ====================
		 * uppercase section
		 * -> convert A-Z into the range 0-25
		 * -> apply the shift
		 * -> convert back into A-Z
		 * ==================== */
		if (c >= 'A' && c <= 'Z')
		{
			c = ((c - 'A' + SHIFT) % 26) + 'A';
		}

		/* ====================
		 * lowercase section
		 * -> convert a-z into the range 0-25
		 * -> apply the shift
		 * -> convert back into a-z
		 * ==================== */
		else if (c >= 'a' && c <= 'z')
		{
			c = ((c - 'a' + SHIFT) % 26) + 'a';
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