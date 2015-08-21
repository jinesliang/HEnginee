

#include "socketexception.hpp"

namespace Socket
{
    SocketException::SocketException(const string &message)
    {
        this->_error = message;
    }

    SocketException::~SocketException() throw()
    {
    }

    const char* SocketException::what() const throw()
    {
        return this->_error.c_str();
    }

    ostream& operator<< (ostream &out, SocketException &e)
    {
        out << e.what();
        return out;
    }
}

