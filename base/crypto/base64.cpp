#include "base64.hpp"

const char squence[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890+/=";

int GetIndex(char c)
{
	for (int i = 0; i < sizeof(squence); ++i)
	{
		if (squence[i] == c)
		{
			return i;
		}
	}
	return -1;
}

#define GetBase64Idx(c) GetIndex(c)

Base64::Base64(void)
{
}


Base64::~Base64(void)
{
}

bool Base64::encode(const std::string& instr, std::string& outstr)
{
	int outlen = (instr.length()+2)/3*4;
	char *out = new char[outlen+1];
	bool ret = encode(instr.data(), instr.length(), out, outlen);
	if (ret)
	{
		outstr = std::string(out, outlen);
	}

	delete []out;
	return ret;
}

bool Base64::encode(const char *instr, int inlen, char *outstr, int& outlen)
{
	int len = (inlen+2)/3*4;
	if (outlen < len)
	{
		return false;
	}

#if 0
	int looptime = inlen / 3;
	int j = 0;
	int k = 0;

	for (int i = 0; i < looptime; ++i)
	{
		outstr[j] = squence[instr[k] & 0x3f];
		outstr[j+1] = squence[((instr[k+1]&0x0f)<<2)|((instr[k]>>6)&0x03)];
		outstr[j+2] = squence[((instr[k+1]&0xf0)>>4)|((instr[k+2]&0x03)<<4)];
		outstr[j+3] = squence[(instr[k+2]>>2)&0x3f];
		k += 3;
		j +=4 ;
	}

	char c = (inlen-k)==2?instr[k+1]:0;
	outstr[j] = squence[instr[k] & 0x3f];
	outstr[j+1] = squence[((c&0x0f)<<2)|((instr[k]>>6)&0x03)];
	outstr[j+2] = squence[c==0?64:((c&0xf0)>>4)];
	outstr[j+3] = squence[64];
#else
	
	for (int i = 0, j = 0; i < inlen; i+=3, j+= 4)
	{
		int left = inlen-i;
		char idx0 = instr[i] & 0x3f;
		char idx1 = (instr[i]>>6)&0x03;
		char idx2 = 64;
		char idx3 = 64;
		if (left > 1)
		{
			idx1 |= (instr[i+1]&0x0f)<<2;
			idx2 = (instr[i+1]&0xf0)>>4;
			
			if (left > 2)
			{
				idx2 |= (instr[i+2]&0x03)<<4;
				idx3 = (instr[i+2]>>2)&0x3f;
			}
		}
		
		outstr[j] = squence[idx0];
		outstr[j+1] = squence[idx1];
		outstr[j+2] = squence[idx2];
		outstr[j+3] = squence[idx3];
	}
#endif
	outlen = len;
	return true;
}

bool Base64::decode(const std::string& instr, std::string& outstr)
{
	int outlen = instr.length()/4*3;
	char *out = new char[outlen+1];
	bool ret = decode(instr.data(), instr.length(), out, outlen);
	if (ret)
	{
		outstr = std::string(out, outlen);
	}

	delete []out;
	return ret;
}

bool Base64::decode(const char *instr, int inlen, char *outstr, int& outlen)
{
	int len = inlen/4*3;
	if (outlen < len)
	{
		return false;
	}

	for (int i = 0, j = 0; i < inlen; i+=4, j+=3)
	{
		char idx0 = GetBase64Idx(instr[i]);
		char idx1 = GetBase64Idx(instr[i+1]);
		char idx2 = GetBase64Idx(instr[i+2]);
		char idx3 = GetBase64Idx(instr[i+3]);
		if (64 == idx3)
		{
			idx3 = 0;
			outlen = j+2;
		}
		if (64 == idx2)
		{
			idx2 = 0;
			outlen = j+1;
		}
		
		outstr[j] = idx0 & 0x3f | ((idx1&0x03)<<6);
		outstr[j+1] = ((idx1>>2)&0x0f)|((idx2&0x0f)<<4);
		outstr[j+2] = ((idx2&0x30)>>4)&((idx3&0x3f)<<2);
	}

	outlen = len;
	return true;
}
