#pragma once

typedef struct
{
	unsigned int count[2];
	unsigned int state[4];
	unsigned char buffer[64];
}MD5_CTX;

class MD5
{
public:
	MD5();
	~MD5();

	void MD5Init();
	void MD5Update(unsigned char *input, unsigned int inputlen);
	void MD5Final();

	void ToHexString(char out[33]);
	void GetDigest(unsigned char out[16]);

protected:
	void MD5Transform(unsigned int state[4], unsigned char block[64]);
	void MD5Encode(unsigned char *output, unsigned int *input, unsigned int len);
	void MD5Decode(unsigned int *output, unsigned char *input, unsigned int len);

private:
	MD5_CTX m_context;
	unsigned char digest[16];
};




