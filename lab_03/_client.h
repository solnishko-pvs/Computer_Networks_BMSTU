#ifndef _CLIENT_H
#define _CLIENT_H

#include <string>
#include <sys/socket.h>

class _Client
{
public:
    virtual int connect(std::string &address, int &port) = 0;
    virtual int send_msg(std::string &msg) = 0;
    virtual int recieve(char &buf) = 0;
    virtual ~_Client() {}

};

#endif // _CLIENT_H
