

#include "udp.hpp"
#include "socketexception.hpp"

namespace Socket
{
	//Datagram 实现
	template <class DataType>
	template <class T>
	void Datagram<DataType>::operator= (const Datagram<T> &datagram)
	{
		this->address = datagram.address;
		this->data = datagram.data;
	}

	//UDP实现
    UDP::UDP(void) : CommonSocket(SOCK_DGRAM)
    {
    }

    UDP::UDP(const UDP &udp) : CommonSocket()
    {
        this->_socket_id = udp._socket_id;
        this->_opened = udp._opened;
        this->_binded = udp._binded;
    }

    template <class T>
    int UDP::send(Ip ip, Port port, const T *data, size_t len)
    {
        if (!this->_opened) this->open();
        
        len *= sizeof(T);
        if (len > (SOCKET_MAX_BUFFER_LEN * sizeof(T)))
        {
            stringstream error;
            error << "[send] 地址[" << ip << ":" << port << "] 上发送的数据长度[len=" << len <<"]大于最大缓存长度(" << SOCKET_MAX_BUFFER_LEN*sizeof(T) <<")" ;
            throw SocketException(error.str());
        }

        Address address(ip, port);
        int ret;

        if ((ret = sendto(this->_socket_id, (const char*)data, len, 0, (struct sockaddr*)&address, sizeof(struct sockaddr))) == -1)
        {
            stringstream error;
            error << "[send] 地址[" << ip << ":" << port << "] 不能发送数据[data=" << data << "] [len=" << len << "]";
            throw SocketException(error.str());
        }
        
        return ret;
    }
    
    template <class T>
    int UDP::send(Address address, const T *data, size_t len)
    {
        return this->send<T>(address.ip(), address.port(), data, len);
    }

    template <class T>
    int UDP::send(Ip ip, Port port, T data)
    {
        return this->send<T>(ip, port, &data, 1);
    }
    
    template <class T>
    int UDP::send(Address address, T data)
    {
        return this->send<T>(address.ip(), address.port(), &data, 1);
    }

    template <>
    int UDP::send<string>(Ip ip, Port port, string data)
    {
        return this->send<char>(ip, port, data.c_str(), data.length() + 1);
    }
    
    template <>
    int UDP::send<string>(Address address, string data)
    {
        return this->send<char>(address.ip(), address.port(), data.c_str(), data.length() + 1);
    }

    template <class T>
    int UDP::send(Ip ip, Port port, vector<T> data)
    {
        return this->send<T>(ip, port, data.data(), data.size());
    }
    
    template <class T>
    int UDP::send(Address address, vector<T> data)
    {
        return this->send<T>(address.ip(), address.port(), data.data(), data.size());
    }

    template <class T>
    int UDP::receive(Address *address, T *data, size_t len, unsigned int *received_elements)
    {
        if (!this->_opened) this->open();
        if (!this->_binded) throw SocketException("[receive] 端口需要先进行监听才可收取数据");
        
        len *= sizeof(T);
        if (len > (SOCKET_MAX_BUFFER_LEN * sizeof(T)))
        {
            stringstream error;
            error << "[receive] 接收数据长度[len=" << len << "]大于最大缓存(" << SOCKET_MAX_BUFFER_LEN*sizeof(T) << ")";
            throw SocketException(error.str());
        }

        int received_bytes;
        socklen_t size = sizeof(struct sockaddr);

        if ((received_bytes = recvfrom(this->_socket_id, (char*)data, len, 0, (struct sockaddr*)address, (socklen_t*)&size)) == -1)
        {
            throw SocketException("[receive] 不能接收到数据");
        }
        
        *received_elements = (unsigned int)(received_bytes / sizeof(T));
        
        return received_bytes;
    }

    template <class T>
    Datagram<T*> UDP::receive(T *buffer, size_t len = SOCKET_MAX_BUFFER_LEN)
    {
        Datagram<T*> ret;
        
        ret.received_bytes = this->receive<T>(&ret.address, buffer, len, &ret.received_elements);
        ret.data = buffer;
        
        return ret;
    }

    template <class T, size_t N>
    Datagram<T[N]> UDP::receive(size_t len = N)
    {
        Datagram<T[N]> ret;
        ret.received_bytes = this->receive<T>(&ret.address, ret.data, len, &ret.received_elements);
        return ret;
    }

    template <class T>
    Datagram<T> UDP::receive(void)
    {
        Datagram<T> ret;
        ret.received_bytes = this->receive<T>(&ret.address, &ret.data, 1, &ret.received_elements);
        return ret;
    }

    template <>
    Datagram<string> UDP::receive<string>(void)
    {
        Datagram<string> ret;
        char buffer[SOCKET_MAX_BUFFER_LEN];
        
        ret.received_bytes = this->receive<char>(&ret.address, buffer, SOCKET_MAX_BUFFER_LEN, &ret.received_elements);
        ret.data = buffer;
        
        return ret;
    }

    template <class T>
    Datagram<vector<T> > UDP::receive(size_t len)
    {
        Datagram<vector<T> > ret;
        T buffer[len];
        
        ret.received_bytes = this->receive<T>(&ret.address, buffer, len, &ret.received_elements);
        for (int i = 0; i < ret.received_elements; i++) ret.data.push_back(buffer[i]);
        
        return ret;
    }
}

