#ifndef _SERVER_H
#define _SERVER_H

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <csignal>
#include <fstream>

class _Server
{
public:
    virtual int start() = 0;
    virtual void resize_buf(int &new_size) = 0;
    virtual void resize_clients_num(int &new_size) = 0;

    virtual ~_Server() {}

private:
    virtual int userHandling(unsigned int fd, unsigned int client_id) = 0;
    virtual void handlingNewUser(unsigned int fd) = 0;
    virtual std::string get_ext(std::string& filename) = 0;
    virtual void log(std::string& filename, std::string& user_id) = 0;
};

#endif // _SERVER_H
