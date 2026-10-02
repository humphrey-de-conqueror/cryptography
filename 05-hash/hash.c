#include <stdio.h>

/* ====================
 * hash configuration
 * -> define the initial hash value
 * ==================== */
#define INITIAL_HASH 5381

/* ====================
 * hash section
 * -> read every byte from the input
 * -> incorporate the byte into the hash
 * -> produce a fixed-size integer
 * ==================== */
int main(void)
{
	FILE *input;
	int c;
	unsigned long hash;

	hash = INITIAL_HASH;

	/* ====================
	 * file section
	 * -> open the input as binary data
	 * ==================== */
	input = fopen("original.txt", "rb");

	if (input == NULL)
	{
		printf("Error opening file.\n");
		return 1;
	}

	/* ====================
	 * hashing loop
	 * -> read one byte
	 * -> update the hash state
	 * ==================== */
	while ((c = fgetc(input)) != EOF)
	{
		hash = ((hash << 5) + hash) ^ (unsigned long)c;
	}

	/* ====================
	 * output section
	 * -> print the final digest
	 * ==================== */
	printf("%lu\n", hash);

	/* ====================
	 * cleanup section
	 * -> close the input file
	 * ==================== */
	fclose(input);

	return 0;
}