
#include "address.hpp"
#include "socketexception.hpp"

namespace Socket
{
    void Address::set_address(Ip ip, Port port)
    {
        this->sin_family = AF_INET;
        this->ip(ip);
        this->port(port);
    }

    Address::Address()
    {
        set_address("0.0.0.0", 0);
    }

    Address::Address(Port port)
    {
        set_address("0.0.0.0", port);
    }

    Address::Address(Ip ip, Port port)
    {
        set_address(ip, port);
    }

    Address::Address(struct sockaddr_in address)
    {
        set_address(inet_ntoa(address.sin_addr), address.sin_port);
    }

    Address::Address(const Address &address)
    {
        this->sin_family = address.sin_family;
        this->sin_addr = address.sin_addr;
        this->sin_port = address.sin_port;
    }

    Ip Address::ip(void)
    {
        return inet_ntoa(this->sin_addr);
    }

    Ip Address::ip(Ip ip)
    {
#ifdef WINDOWS
        unsigned long address = inet_addr(ip.c_str());

        if (address == INADDR_NONE)
        {
            stringstream error;
            error << "[ip] with [ip=" << ip << "] Invalid ip address provided";
            throw SocketException(error.str());
        }
        else
        {
            this->sin_addr.S_un.S_addr = address;
        }
#else
        if (inet_aton(ip.c_str(), &this->sin_addr) == 0)
        {
            stringstream error;
            error << "[ip] with [ip=" << ip << "] Invalid ip address provided";
            throw SocketException(error.str());
        }
#endif
        return this->ip();
    }

    Port Address::port(void)
    {
        return ntohs(this->sin_port);
    }

    Port Address::port(Port port)
    {
        this->sin_port = htons(port);
        return this->port();
    }

    ostream& operator<< (ostream &out, Address &address)
    {
        out << address.ip() << ":" << address.port();
        return out;
    }
}
