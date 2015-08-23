#pragma once

/**
*	@简介: Base64 编解码
*	@作者: Haust	
*	@日期: 2015/08/20
*	@注意：
*		Base64的算法为将源串中的低位移植目标串中的高位，
*	从而生成目标字符串。现代码中提供两种编码，一种是正常
*	的Base64算法，另一种则是将源串中的高位放在目标串中的
*	低位，从而演化出与原算法相反的结果，在接口中用reverse
*	表示。用二进制表示如下：
*	原算法： 0x12345678,0x23456789,0x3456789a
*			->0x00123456, 0x00782345, 0x00678934, 0x0056789a
*	reverse: 0x12345678,0x23456789,0x3456789a
*			->0x00345678, 0x00678912, 0x009a2345, 0x00345678
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

	static bool reverse_encode(const std::string& instr, std::string& outstr);
	static bool reverse_encode(const char *instr, int inlen, char *outstr, int& outlen);
	static bool reverse_decode(const std::string& instr, std::string& outstr);
	static bool reverse_decode(const char *instr, int inlen, char *outstr, int& outlen);
};

