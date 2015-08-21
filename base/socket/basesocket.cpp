#include "basesocket.hpp"
#include "socketexception.hpp"
#include "address.hpp"

namespace Socket
{
#ifdef WINDOWS
    unsigned int BaseSocket::_num_sockets = 0;
#endif

    void BaseSocket::int_socket_environment(void)
    {
#ifdef WINDOWS
        this->_num_sockets++;
        if (this->_num_sockets == 1)
        {
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
                throw SocketException("[int_socket_environment] Socket环境初始化失败");
        }
#endif
    }

    BaseSocket::BaseSocket(void)
    {
        this->int_socket_environment();
    }

    BaseSocket::BaseSocket(int socket_type)
    {
        this->int_socket_environment();

        this->_socket_type = socket_type;
        this->_opened = false;
        this->_binded = false;
        this->open();
    }

    BaseSocket::~BaseSocket(void)
    {
#ifdef WINDOWS
        this->_num_sockets--;
        if (this->_num_sockets == 0)
            WSACleanup();
#endif
    }

    void BaseSocket::open(void)
    {
        if (!this->_opened)
        {
            if ((this->_socket_id = socket(AF_INET, this->_socket_type, 0)) == -1)
                throw SocketException("[open] 创建Socket失败");
            this->_opened = true;
            this->_binded = false;
        }
    }

    void BaseSocket::close(void)
    {
        if (this->_opened)
#ifdef WINDOWS
            closesocket(this->_socket_id);
#else
            shutdown(this->_socket_id, SHUT_RDWR);
#endif

        this->_opened = false;
        this->_binded = false;
    }
    
    void BaseSocket::listen_on_port(Port port)
    {
        if (this->_binded) throw SocketException("[listen_on_port] Socket已经绑定，请关闭后重新绑定!");

        if (!this->_opened) this->open();

        Address address(port);

        if (bind(this->_socket_id, (struct sockaddr*)&address, sizeof(struct sockaddr)) == -1)
        {
            stringstream error;
            error << "[listen_on_port] 端口[" << port << "] 不能绑定Socket";
            throw SocketException(error.str());
        }

        this->_binded = true;
    }
	
	void BaseSocket::set_socket_opt(int level, int optname, const void *optval, socklen_t optlen)
	{
#ifdef WINDOWS
		if(setsockopt(this->_socket_id, level, optname, (const char *)optval, (int)optlen) == -1)
#else
		if(setsockopt(this->_socket_id, level, optname, optval, optlen) == -1)
#endif	
		{
			stringstream error;
			error << "[set_socket_opt] 设置Socket属性失败";
			throw SocketException(error.str());
		}
	}

	void BaseSocket::get_socket_opt(int level, int optname, void *optval, socklen_t *optlen)
	{
#ifdef WINDOWS
		if (getsockopt(this->_socket_id, level, optname, (char *)optval, (int *)optlen) == -1)
#else
		if (getsockopt(this->_socket_id, level, optname, optval, optlen) == -1)
#endif // WINDOWS
		{
			stringstream error;
			error << "[set_socket_opt] 获取Socket属性失败";
			throw SocketException(error.str());
		}
	}
}
