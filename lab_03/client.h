#ifndef CLIENT_H
#define CLIENT_H

#include "_client.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

class Client:public _Client
{
public:
    Client();
    ~Client() {}

    int connect(std::string &address, int &port) override;
    int send_msg(std::string &msg) override;
    int recieve(char &buf) override;

private:
    int sock;
};


#endif // CLIENT_H
