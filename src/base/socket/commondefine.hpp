#pragma once

/* 简介：Socket公共类
*  作者：Haust
*  日期：2015-08-21
*/

#include <string>

#if defined _MSC_VER || defined __WIN32 || defined __WIN64
    #define WINDOWS
#endif

#ifdef WINDOWS
    #include <winsock.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
#endif

#define SOCKET_MAX_BUFFER_LEN 1024

using namespace std;

namespace Socket
{
    typedef int SocketId;
    typedef string Ip;
    typedef unsigned int Port;
#ifdef WINDOWS
    typedef unsigned int socklen_t;
#endif
}

