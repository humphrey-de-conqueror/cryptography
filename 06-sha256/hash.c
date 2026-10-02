#include <stdio.h>
#include <openssl/sha.h>

/* ====================
 * hash configuration
 * -> SHA-256 produces a 256-bit digest
 * -> 256 bits = 32 bytes
 * ==================== */
#define SHA256_LENGTH 32

/* ====================
 * hash section
 * -> read the input
 * -> feed every byte into SHA-256
 * -> retrieve the final digest
 * ==================== */
int main(void)
{
	FILE *input;
	SHA256_CTX context;
	unsigned char buffer[4096];
	unsigned char digest[SHA256_LENGTH];
	size_t bytes_read;
	int i;

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
	 * SHA-256 initialization
	 * -> initialize the internal hash state
	 * ==================== */
	SHA256_Init(&context);

	/* ====================
	 * hashing loop
	 * -> read chunks from the file
	 * -> feed each chunk into SHA-256
	 * ==================== */
	while ((bytes_read = fread(buffer, 1, sizeof(buffer), input)) > 0)
	{
		SHA256_Update(&context, buffer, bytes_read);
	}

	/* ====================
	 * finalization section
	 * -> finish the SHA-256 calculation
	 * -> store the 32-byte digest
	 * ==================== */
	SHA256_Final(digest, &context);

	/* ====================
	 * output section
	 * -> print each digest byte as hexadecimal
	 * ==================== */
	for (i = 0; i < SHA256_LENGTH; i++)
	{
		printf("%02x", digest[i]);
	}

	printf("\n");

	/* ====================
	 * cleanup section
	 * -> close the input file
	 * ==================== */
	fclose(input);

	return 0;
}