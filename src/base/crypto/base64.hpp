#pragma once

/**
*	@���: Base64 �����
*	@����: Haust	
*	@����: 2015/08/20
*	@ע�⣺
*		Base64���㷨Ϊ��Դ���еĵ�λ��ֲĿ�괮�еĸ�λ��
*	�Ӷ�����Ŀ���ַ������ִ������ṩ���ֱ��룬һ��������
*	��Base64�㷨����һ�����ǽ�Դ���еĸ�λ����Ŀ�괮�е�
*	��λ���Ӷ��ݻ�����ԭ�㷨�෴�Ľ�����ڽӿ�����reverse
*	��ʾ���ö����Ʊ�ʾ���£�
*	ԭ�㷨�� 0x12345678,0x23456789,0x3456789a
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

