#ifndef _CLIENT_H
#define _CLIENT_H


#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

class _Client
{
public:
    virtual int connect(std::string &address, int &port) = 0;
    virtual int reconnect(std::string &address, int &port) = 0;
    virtual int send_msg(std::string &msg) = 0;
    virtual int recieve() = 0;
    virtual void resize_buf(int &new_size) = 0;
    virtual std::string get_request(std::string &filename) = 0;
    virtual ~_Client() {}

};

#endif // _CLIENT_H
