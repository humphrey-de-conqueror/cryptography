#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

/* ====================
 * SHA-256 constants
 * -> SHA-256 operates on 32-bit words
 * ==================== */
#define SHA256_WORD_BITS 32

/* ====================
 * SHA-256 initial state
 * -> eight 32-bit words
 * -> these are the starting values
 *    before any message block is processed
 * ==================== */
static const uint32_t SHA256_INITIAL_STATE[8] = {
	0x6a09e667,
	0xbb67ae85,
	0x3c6ef372,
	0xa54ff53a,
	0x510e527f,
	0x9b05688c,
	0x1f83d9ab,
	0x5be0cd19
};

/* ====================
 * SHA-256 round constants
 * -> one constant is used in each
 *    of the 64 compression rounds
 *
 * -> these constants are fixed by
 *    the SHA-256 specification
 * ==================== */
static const uint32_t SHA256_K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

/* ====================
 * bit rotation section
 * -> rotate a 32-bit value to the right
 *
 * Example:
 *
 * 10110001
 *      ↓ ROTR(2)
 * 01101100
 *
 * Bits shifted out on the right
 * re-enter on the left.
 * ==================== */
static uint32_t rotate_right(uint32_t value, unsigned int bits)
{
	return (value >> bits) | (value << (SHA256_WORD_BITS - bits));
}

/* ====================
 * SHA-256 choice function
 * -> select bits from x or y
 * -> based on the corresponding bits of z
 *
 * Ch(x,y,z) =
 *     (x AND y) XOR
 *     (NOT x AND z)
 * ==================== */
static uint32_t choose(uint32_t x, uint32_t y, uint32_t z)
{
	return (x & y) ^ (~x & z);
}

/* ====================
 * SHA-256 majority function
 * -> determine the majority bit
 * -> among x, y and z
 *
 * Maj(x,y,z) =
 *     (x AND y) XOR
 *     (x AND z) XOR
 *     (y AND z)
 * ==================== */
static uint32_t majority(uint32_t x, uint32_t y, uint32_t z)
{
	return (x & y) ^ (x & z) ^ (y & z);
}

/* ====================
 * SHA-256 big sigma 0
 * -> combine three right rotations
 * ==================== */
static uint32_t big_sigma_zero(uint32_t x)
{
	return rotate_right(x, 2) ^
	       rotate_right(x, 13) ^
	       rotate_right(x, 22);
}

/* ====================
 * SHA-256 big sigma 1
 * -> combine three right rotations
 * ==================== */
static uint32_t big_sigma_one(uint32_t x)
{
	return rotate_right(x, 6) ^
	       rotate_right(x, 11) ^
	       rotate_right(x, 25);
}

/* ====================
 * SHA-256 small sigma 0
 * -> combine rotations and a right shift
 * ==================== */
static uint32_t small_sigma_zero(uint32_t x)
{
	return rotate_right(x, 7) ^
	       rotate_right(x, 18) ^
	       (x >> 3);
}

/* ====================
 * SHA-256 small sigma 1
 * -> combine rotations and a right shift
 * ==================== */
static uint32_t small_sigma_one(uint32_t x)
{
	return rotate_right(x, 17) ^
	       rotate_right(x, 19) ^
	       (x >> 10);
}

/* ====================
 * convert block to words
 *
 * SHA-256 interprets each group
 * of four bytes as one 32-bit
 * big-endian integer.
 *
 * Example:
 *
 *     bytes:
 *     48 65 6c 6c
 *
 *     becomes:
 *     0x48656c6c
 *
 * The first 16 words are taken
 * directly from the padded block.
 * ==================== */
static void create_message_words(const unsigned char *block,
				  uint32_t *words)
{
	int i;
	int offset;

	for (i = 0; i < 16; i++) {
		offset = i * 4;

		words[i] =
			((uint32_t)block[offset] << 24) |
			((uint32_t)block[offset + 1] << 16) |
			((uint32_t)block[offset + 2] << 8) |
			(uint32_t)block[offset + 3];
	}
}

 /* ====================
  * expand message schedule
  *
  * SHA-256 starts with 16 words
  * extracted directly from the
  * 512-bit message block.
  *
  * It then generates another
  * 48 words.
  *
  * Total:
  *
  *     W[0] ... W[63]
  *
  * Formula:
  *
  *     W[t] =
  *         sigma1(W[t - 2])
  *         + W[t - 7]
  *         + sigma0(W[t - 15])
  *         + W[t - 16]
  * ==================== */
static void expand_message_schedule(uint32_t *words)
{
	int i;

	for (i = 16; i < 64; i++) {
		words[i] =
			small_sigma_one(words[i - 2]) +
			words[i - 7] +
			small_sigma_zero(words[i - 15]) +
			words[i - 16];
	}
}

 /* ====================
  * SHA-256 compression
  *
  * Process one 512-bit block
  * through all 64 rounds.
  *
  * Working variables:
  *
  *     a b c d e f g h
  *
  * Each round calculates:
  *
  *     T1 = h
  *        + Sigma1(e)
  *        + Ch(e,f,g)
  *        + K[t]
  *        + W[t]
  *
  *     T2 = Sigma0(a)
  *        + Maj(a,b,c)
  *
  * The variables are then shifted
  * for the next round.
  * ==================== */
static void compress(uint32_t *state, const uint32_t *words)
{
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;
	uint32_t e;
	uint32_t f;
	uint32_t g;
	uint32_t h;
	uint32_t t1;
	uint32_t t2;
	uint32_t temporary;
	int i;

	/* ====================
	 * copy current hash state
	 * into working variables
	 * ==================== */
	a = state[0];
	b = state[1];
	c = state[2];
	d = state[3];
	e = state[4];
	f = state[5];
	g = state[6];
	h = state[7];

	/* ====================
	 * perform 64 compression
	 * rounds
	 * ==================== */
	for (i = 0; i < 64; i++) {
		t1 = h +
		     big_sigma_one(e) +
		     choose(e, f, g) +
		     SHA256_K[i] +
		     words[i];

		t2 = big_sigma_zero(a) +
		     majority(a, b, c);

		/* ====================
		 * shift working variables
		 * ==================== */
		h = g;
		g = f;
		f = e;

		temporary = d + t1;
		e = temporary;

		d = c;
		c = b;
		b = a;

		temporary = t1 + t2;
		a = temporary;
	}

	/* ====================
	 * add the compressed result
	 * back into the hash state
	 *
	 * SHA-256 does not replace the
	 * state with a,b,c,d,e,f,g,h.
	 *
	 * It adds them to the previous
	 * state values.
	 * ==================== */
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;
	state[5] += f;
	state[6] += g;
	state[7] += h;
}

/* ====================
 * process one SHA-256 block
 *
 * A block always contains
 * exactly 64 bytes.
 *
 * The block goes through:
 *
 *     64 bytes
 *        ↓
 *     W[0..15]
 *        ↓
 *     W[0..63]
 *        ↓
 *     compression
 *
 * Keeping these operations in
 * one function prevents the same
 * sequence from being repeated.
 * ==================== */
static void process_block(const unsigned char *block,
			  uint32_t *state)
{
	uint32_t words[64];

	/* ====================
	 * convert block into
	 * the initial 16 words
	 * ==================== */
	create_message_words(block, words);

	/* ====================
	 * expand to 64 words
	 * ==================== */
	expand_message_schedule(words);

	/* ====================
	 * compress this block
	 * ==================== */
	compress(state, words);
}

static void pad_final_block(unsigned char *buffer,
			    size_t bytes_read,
			    uint64_t total_length,
			    uint32_t *state)
{
	uint64_t bit_length;
	size_t i;

	/*
	 * Append the mandatory 1-bit.
	 */
	buffer[bytes_read] = 0x80;

	/*
	 * If there is not enough room for the
	 * 8-byte message length, process this
	 * block and use a second block.
	 */
	if (bytes_read >= 56) {
		for (i = bytes_read + 1; i < 64; i++)
			buffer[i] = 0;

		process_block(buffer, state);

		for (i = 0; i < 64; i++)
			buffer[i] = 0;
	} else {
		for (i = bytes_read + 1; i < 56; i++)
			buffer[i] = 0;
	}

	/*
	 * Append the original message length
	 * in bits, using big-endian byte order.
	 */
	bit_length = total_length * 8;

	for (i = 0; i < 8; i++)
		buffer[56 + i] =
			(unsigned char)(bit_length >> (56 - i * 8));
}

/* ====================
 * SHA-256 file hashing
 *
 * Read the file in 64-byte blocks.
 *
 * Complete blocks are compressed
 * immediately.
 *
 * The final partial block is kept
 * until EOF so that SHA-256 padding
 * can be added correctly.
 * ==================== */
static int sha256_file(const char *filename,
		       uint32_t *state)
{
	FILE *file;
	unsigned char buffer[64];
	size_t bytes_read;
	size_t i;
	uint64_t total_length;

	/* ====================
	 * open file
	 * ==================== */
	file = fopen(filename, "rb");

	if (file == NULL) {
		perror("fopen");
		return 1;
	}

	total_length = 0;

	/* ====================
	 * initialize SHA-256 state
	 * ==================== */
	for (i = 0; i < 8; i++)
		state[i] = SHA256_INITIAL_STATE[i];

	/* ====================
	 * read complete blocks
	 * ==================== */
	while (1) {
		bytes_read = fread(buffer, 1, 64, file);

		if (bytes_read == 64) {
			/* ====================
			* process complete block
			* ==================== */
			process_block(buffer, state);

			total_length += 64;
		} else {
			break;
		}
	}

	/* ====================
	 * check for file error
	 * ==================== */
	if (ferror(file)) {
		fprintf(stderr, "fread failed\n");
		fclose(file);
		return 1;
	}

	fclose(file);

	/* ====================
	 * total_length currently
	 * contains only complete
	 * 64-byte blocks.
	 *
	 * Add the remaining bytes
	 * from the final block.
	 * ==================== */
	total_length += bytes_read;

	pad_final_block(buffer, bytes_read, total_length, state);

	process_block(buffer, state);

	return 0;
}

/* ====================
 * print SHA-256 digest
 *
 * The final SHA-256 value consists
 * of eight 32-bit words:
 *
 *     H[0] H[1] ... H[7]
 *
 * Each word is printed as exactly
 * eight hexadecimal characters.
 *
 * 8 words × 8 characters
 * = 64 hexadecimal characters
 * ==================== */
static void print_digest(const uint32_t *state)
{
	int i;

	for (i = 0; i < 8; i++)
		printf("%08x", state[i]);

	printf("\n");
}

int main(int argc, char *argv[])
{
	uint32_t state[8];

	/*
	 * Require exactly one input filename.
	 *
	 * Example:
	 *     ./sha256 test-55
	 */
	if (argc != 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		return 1;
	}

	if (sha256_file(argv[1], state) != 0)
		return 1;

	print_digest(state);

	return 0;
}