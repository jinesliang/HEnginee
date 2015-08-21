#pragma once

/* 简介：TCP
*  作者：Haust
*  日期：2015-08-21
*/

#include "commondefine.hpp"
#include "basesocket.hpp"
#include "address.hpp"

namespace Socket
{
	class TCP : public BaseSocket
	{
	public:
		TCP(void);
		TCP(const TCP&);

		Ip ip(void);
		Port port(void);
		Address address(void);

		void listen_on_port(Port, unsigned int);
		void connect_to(Address);

		TCP accept_client(void);

		template <class T> int send(const T*, size_t);
		template <class T> int receive(T*, size_t);

		void send_file(string);
		void receive_file(string);

	private:
		Address _address;
	};
};