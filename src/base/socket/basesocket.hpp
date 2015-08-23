#pragma once

/* 简介：UDP
*  作者：Haust
*  日期：2015-08-21
*/
#pragma once

/* 简介：Base Socket Socket基类
*  作者：Haust
*  日期：2015-08-21
*/

#include "commondefine.hpp"

namespace Socket{
	class BaseSocket
	{
	public:
		BaseSocket(void);
		BaseSocket(int);

		~BaseSocket(void);

		void open(void);
		void close(void);

		virtual void listen_on_port(Port);

		virtual void set_socket_opt(int level, int optname, const void *optval, socklen_t optlen);

		virtual void get_socket_opt(int level, int optname, void *optval, socklen_t *optlen);

	private:
#ifdef WINDOWS
		static unsigned int _num_sockets;
#endif
		void int_socket_environment(void);

	protected:
		SocketId _socket_id;
		int _socket_type;
		bool _opened;
		bool _binded;
	};
};
