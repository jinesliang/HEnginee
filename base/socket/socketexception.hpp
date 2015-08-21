#pragma once

/* ��飺Socket Exception ����Socket��ʹ���쳣
*  ���ߣ�Haust
*  ���ڣ�2015-08-21
*/

#include "commondefine.hpp"

#include <iostream>
#include <sstream>
#include <exception>

using namespace std;

namespace Socket
{
	class SocketException : public exception
	{
	public:
		SocketException(const string&);
		~SocketException() throw();

		virtual const char* what() const throw();
		friend ostream& operator<< (ostream &out, SocketException &e);

	private:
		string _error;
	};
};