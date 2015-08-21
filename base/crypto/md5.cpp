#include "MD5.hpp"
#include <stdio.h>
#include <memory.h>

#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x^y^z)
#define I(x,y,z) (y ^ (x | ~z))

#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))

#define ROUND_OPER(M, a, b, c, d, x, s, ac) \
{ \
	a += M(b,c,d) + x + ac; \
	a = ROTATE_LEFT(a,s); \
	a += b; \
}

unsigned char PADDING[] = { 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

unsigned int states[] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476};

MD5::MD5()
{
}

MD5::~MD5()
{
}

void MD5::MD5Init()
{
	memset(m_context.count, 0, sizeof(m_context.count));
	memcpy(m_context.state, states, sizeof(states));
	memset(digest, 0, sizeof(digest));
}

void MD5::MD5Update(unsigned char *input, unsigned int inputlen)
{
	unsigned int i = 0, index = 0, partlen = 0;
	index = (m_context.count[0] >> 3) & 0x3F;
	partlen = 64 - index;
	m_context.count[0] += inputlen << 3;
	if (m_context.count[0] < (inputlen << 3))
		m_context.count[1]++;
	m_context.count[1] += inputlen >> 29;

	if (inputlen >= partlen)
	{
		memcpy(&m_context.buffer[index], input, partlen);
		MD5Transform(m_context.state, m_context.buffer);
		for (i = partlen; i + 64 <= inputlen; i += 64)
			MD5Transform(m_context.state, &input[i]);
		index = 0;
	}
	else
	{
		i = 0;
	}
	memcpy(&m_context.buffer[index], &input[i], inputlen - i);
}

void MD5::MD5Final()
{
	unsigned int index = 0, padlen = 0;
	unsigned char bits[8];
	index = (m_context.count[0] >> 3) & 0x3F;
	padlen = (index < 56) ? (56 - index) : (120 - index);
	MD5Encode(bits, m_context.count, 8);
	MD5Update(PADDING, padlen);
	MD5Update(bits, 8);
	MD5Encode(digest, m_context.state, 16);
}

void MD5::MD5Encode(unsigned char *output, unsigned int *input, unsigned int len)
{
	unsigned int i = 0, j = 0;
	while (j < len)
	{
		output[j] = input[i] & 0xFF;
		output[j + 1] = (input[i] >> 8) & 0xFF;
		output[j + 2] = (input[i] >> 16) & 0xFF;
		output[j + 3] = (input[i] >> 24) & 0xFF;
		i++;
		j += 4;
	}
}

void MD5::MD5Decode(unsigned int *output, unsigned char *input, unsigned int len)
{
	unsigned int i = 0, j = 0;
	while (j < len)
	{
		output[i] = (input[j]) |
			(input[j + 1] << 8) |
			(input[j + 2] << 16) |
			(input[j + 3] << 24);
		i++;
		j += 4;
	}
}

void MD5::MD5Transform(unsigned int state[4], unsigned char block[64])
{
	unsigned int a = state[0];
	unsigned int b = state[1];
	unsigned int c = state[2];
	unsigned int d = state[3];
	unsigned int x[64];
	MD5Decode(x, block, 64);

	static unsigned int arr_idx[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
		1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12,
		5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2,
		0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9};

	static unsigned int sign[] = {7, 12, 17, 22,
		5, 9, 14, 20,
		4, 11, 16, 23,
		6, 10, 15, 21};

	static unsigned int base[] = {0xd76aa478,  0xe8c7b756,  0x242070db,  0xc1bdceee,  0xf57c0faf,  0x4787c62a,  0xa8304613,  0xfd469501,  
		0x698098d8,  0x8b44f7af,  0xffff5bb1,  0x895cd7be,  0x6b901122,  0xfd987193,  0xa679438e,  0x49b40821, 
		0xf61e2562,  0xc040b340,  0x265e5a51,  0xe9b6c7aa,  0xd62f105d,  0x2441453,  0xd8a1e681,  0xe7d3fbc8,  
		0x21e1cde6,  0xc33707d6,  0xf4d50d87,  0x455a14ed,  0xa9e3e905,  0xfcefa3f8,  0x676f02d9,  0x8d2a4c8a, 
		0xfffa3942,  0x8771f681,  0x6d9d6122,  0xfde5380c,  0xa4beea44,  0x4bdecfa9,  0xf6bb4b60,  0xbebfbc70,  
		0x289b7ec6,  0xeaa127fa,  0xd4ef3085,  0x4881d05,  0xd9d4d039,  0xe6db99e5,  0x1fa27cf8,  0xc4ac5665, 
		0xf4292244,  0x432aff97,  0xab9423a7,  0xfc93a039,  0x655b59c3,  0x8f0ccc92,  0xffeff47d,  0x85845dd1,  
		0x6fa87e4f,  0xfe2ce6e0,  0xa3014314,  0x4e0811a1,  0xf7537e82,  0xbd3af235,  0x2ad7d2bb,  0xeb86d391
	};

	static int Round_times = 4;

#define ROUND(M, a, b, c, d, x, s, ac, round) \
	{\
	for (int i = 0; i < Round_times; ++i)\
	{\
	int x_idx = round*16+4*i;\
	ROUND_OPER(M, a, b, c, d, x[arr_idx[x_idx+0]], s[round*4+0], ac[x_idx+0]); \
	ROUND_OPER(M, d, a, b, c, x[arr_idx[x_idx+1]], s[round*4+1], ac[x_idx+1]); \
	ROUND_OPER(M, c, d, a, b, x[arr_idx[x_idx+2]], s[round*4+2], ac[x_idx+2]); \
	ROUND_OPER(M, b, c, d, a, x[arr_idx[x_idx+3]], s[round*4+3], ac[x_idx+3]); \
	}\
	++round;\
	}\

	int round = 0;
	/*Round 1*/
	ROUND(F, a, b, c, d, x, sign, base, round);	
	/*Round 2*/
	ROUND(G, a, b, c, d, x, sign, base, round);
	/*Round 3*/
	ROUND(H, a, b, c, d, x, sign, base, round);
	/*Round 4*/
	ROUND(I, a, b, c, d, x, sign, base, round);

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
}

void MD5::ToHexString(char out[33])
{

#define TOHEX(x) (x)>=10?(x)-10+'a':(x)+'0'

	for (int i = 0; i < 16; i++)
	{
		unsigned char high = (digest[i]>>4)&0x0f;
		high = TOHEX(high);

		unsigned char low = (digest[i])&0x0f;
		low = TOHEX(low);

		sprintf_s(out+2*i, 33-2*i, "%c%c", high, low);
	}
}

void MD5::GetDigest(unsigned char out[16])
{
	memcpy(out, digest, 16);
}