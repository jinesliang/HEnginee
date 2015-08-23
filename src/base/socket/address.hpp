#pragma once

/* 简介：Socket Address
*  作者：Haust
*  日期：2015-08-21
*/

#include "commondefine.hpp"

#include <iostream>
#include <sstream>

using namespace std;

namespace Socket
{
	struct Address : protected sockaddr_in
	{
	public:
		Address();
		Address(Port);
		Address(Ip, Port);
		Address(struct sockaddr_in);
		Address(const Address&);

		Ip ip(void);
		Ip ip(Ip);

		Port port(void);
		Port port(Port);

		friend ostream& operator<< (ostream&, Address&);
	
	private:
		void set_address(Ip, Port);
	};
};

