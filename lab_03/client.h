#ifndef CLIENT_H
#define CLIENT_H

#include "_client.h"


class Client:public _Client
{
public:
    Client();
    ~Client();

    int connect(std::string &address, int &port) override;
    int reconnect(std::string &address, int &port) override;
    int send_msg(std::string &msg) override;
    int recieve() override;
    void resize_buf(int &new_size) override;
    std::string get_request(std::string &filename) override;

private:
    int sock;
    int buf_size = 256;
    char* buf;
};


#endif // CLIENT_H
