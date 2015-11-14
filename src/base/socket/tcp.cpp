

#include "tcp.hpp"
#include "socketexception.hpp"

#include <fstream>

using namespace std;

namespace Socket
{
    TCP::TCP(void) : BaseSocket(SOCK_STREAM)
    {
    }
    
    TCP::TCP(const TCP &tcp) : BaseSocket()
    {
        this->_socket_id = tcp._socket_id;
        this->_opened = tcp._opened;
        this->_binded = tcp._binded;
    }
    
    Ip TCP::ip(void)
    {
        return this->_address.ip();
    }
    
    Port TCP::port(void)
    {
        return this->_address.port();
    }
    
    Address TCP::address(void)
    {
        return Address(this->_address);
    }
    
    void TCP::listen_on_port(Port port, unsigned int listeners = 1)
    {
        BaseSocket::listen_on_port(port);
        
        if (listen(this->_socket_id, listeners) != 0)
        {
            stringstream error;
            error << "[listen_on_port] �˿�[port=" << port << "]���ܰ󶨵�Socket[listeners=" << listeners << "]";
            throw SocketException(error.str());
        }
    }
    
    void TCP::connect_to(Address address)
    {
        if (this->_binded) throw SocketException("[connect_to] Socket�Ѿ��󶨶˿ڣ��뻻һ��Socket����");

        if (!this->_opened) this->open();
        
        if (connect(this->_socket_id, (struct sockaddr*)&address, sizeof(struct sockaddr_in)) < 0)
        {
            stringstream error;
            error << "[connect_to] �������ӵ�ָ����ַ[address=" << address << "]";
            throw SocketException(error.str());
        }
        
        this->_binded = true;
    }
    
    TCP TCP::accept_client(void)
    {
        TCP ret;
        socklen_t len = sizeof(struct sockaddr_in);
        
        ret.close();
#ifdef WINDOWS
		ret._socket_id = accept(this->_socket_id, (struct sockaddr*)&ret._address, (int*)&len);
#else
		ret._socket_id = accept(this->_socket_id, (struct sockaddr*)&ret._address, (socklen_t*)&len);
#endif
        
        ret._opened = true;
        ret._binded = true;
        
        return ret;
    }
    
    template <class T>
    int TCP::send(const T* buffer, size_t len)
    {
        if (!this->_binded) throw SocketException("[send] Socketδ��");
        if (!this->_opened) throw SocketException("[send] Socketδ��");
        
        len *= sizeof(T);
        if (len > (SOCKET_MAX_BUFFER_LEN * sizeof(T)))
        {
            stringstream error;
            error << "[send] �������ݳ���[len=" << len << "]������󻺴�(" << SOCKET_MAX_BUFFER_LEN << ")";
            throw SocketException(error.str());
        }
        
        int ret;
        if ((ret = ::send(this->_socket_id, (const char*)buffer, len, 0)) == -1) throw SocketException("[send] ����ʧ��");
        return ret;
    }
    
    template <class T>
    int TCP::receive(T* buffer, size_t len)
    {
        if (!this->_binded) throw SocketException("[receive] �˿�δ��");
        if (!this->_opened) throw SocketException("[receive] �˿�δ����");
        
        len *= sizeof(T);
        if (len > (SOCKET_MAX_BUFFER_LEN * sizeof(T)))
        {
            stringstream error;
            error << "[receive] �������ݳ���[len=" << len << "]������󻺴�(" << SOCKET_MAX_BUFFER_LEN << ")";
            throw SocketException(error.str());
        }
        
        int ret = 0;
#ifdef WINDOWS
		ret = recv(this->_socket_id, (char*)buffer, len, 0);
#else
		ret = recv(this->_socket_id, buffer, len, 0);
#endif
		if(-1 == ret) throw SocketException("[receive] ����ʧ��");
        return ret;
    }
    
    void TCP::send_file(string file_name)
    {
        unsigned long long file_size;
        char chunk[SOCKET_MAX_BUFFER_LEN];
        char sync;
        fstream fp(file_name.c_str(), ios::in | ios::binary);
        
        if (!fp.is_open())
        {
            stringstream error;
            error << "[send_file] ���ļ�[filename=" << file_name << "]ʧ��";
            throw SocketException(error.str());
        }
        
        fp.seekg(0, ifstream::end);
        file_size = fp.tellg();
        fp.seekg(0, ifstream::beg);
        this->send<unsigned long long>(&file_size, 1);
        
        for (unsigned long long i = 0; i < file_size / SOCKET_MAX_BUFFER_LEN; i++)
        {
            this->receive<char>(&sync, 1);
            fp.read(chunk, SOCKET_MAX_BUFFER_LEN);
            this->send<char>(chunk, SOCKET_MAX_BUFFER_LEN);
        }
        
        if ((file_size % SOCKET_MAX_BUFFER_LEN) != 0)
        {
            this->receive<char>(&sync, 1);
            fp.read(chunk, file_size % SOCKET_MAX_BUFFER_LEN);
            this->send<char>(chunk, file_size % SOCKET_MAX_BUFFER_LEN);
        }
        
        fp.close();
    }
    
    void TCP::receive_file(string file_name)
    {
        unsigned long long file_size;
        char chunk[SOCKET_MAX_BUFFER_LEN];
        char sync;
        fstream fp(file_name.c_str(), ios::out | ios::binary);
        
        if (!fp.is_open())
        {
            stringstream error;
            error << "[receive_file] ���ļ�[filename=" << file_name << "]ʧ��";
            throw SocketException(error.str());
        }
        
        this->receive<unsigned long long>(&file_size, 1);
        
        for(unsigned long long i = 0; i < file_size / SOCKET_MAX_BUFFER_LEN; i++)
        {
            this->send<char>(&sync, 1);
            this->receive<char>(chunk, SOCKET_MAX_BUFFER_LEN);
            fp.write(chunk, SOCKET_MAX_BUFFER_LEN);
        }
        
        if ((file_size % SOCKET_MAX_BUFFER_LEN) != 0)
        {
            this->send<char>(&sync, 1);
            this->send<char>(chunk, file_size % SOCKET_MAX_BUFFER_LEN);
            fp.write(chunk, file_size % SOCKET_MAX_BUFFER_LEN);
        }
        
        fp.close();
    }
}

