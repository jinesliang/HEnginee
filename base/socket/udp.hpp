#pragma once

/* 简介：UDP
*  作者：Haust
*  日期：2015-08-21
*/

#include "basesocket.hpp"
#include "address.hpp"

#include <vector>

using namespace std;

namespace Socket
{
	//Datagram， UDP报文
	template <class DataType>
	struct Datagram
	{
		Address address;
		DataType data;
		unsigned int received_bytes;
		unsigned int received_elements;

		template <class T>
		void operator= (const Datagram<T>&);
	};

	class UDP : public BaseSocket
	{
	public:
		UDP(void);
		UDP(const UDP&);

		template <class T> int send(Ip, Port, const T*, size_t);
		template <class T> int send(Address, const T*, size_t);
		template <class T> int send(Ip, Port, T);
		template <class T> int send(Address, T);
		template <class T> int send(Ip, Port, vector<T>);
		template <class T> int send(Address, vector<T>);

		template <class T> int receive(Address*, T*, size_t, unsigned int*);
		template <class T> Datagram<T*> receive(T*, size_t);
		template <class T, size_t N> Datagram<T[N]> receive(size_t);
		template <class T> Datagram<T> receive(void);
		template <class T> Datagram<vector<T> > receive(size_t);
	};
};
