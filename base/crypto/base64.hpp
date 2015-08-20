#pragma once

/**
*	@简介: Base64 编解码
*	@作者: Haust	
*	@日期: 2015/08/20
*/

#include <string>

class Base64
{
public:
	Base64(void);
	virtual ~Base64(void);

	static bool encode(const std::string& instr, std::string& outstr);
	static bool encode(const char *instr, int inlen, char *outstr, int& outlen);
	static bool decode(const std::string& instr, std::string& outstr);
	static bool decode(const char *instr, int inlen, char *outstr, int& outlen);
};

