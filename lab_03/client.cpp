#include "client.h"
#include "settings.h"

Client::Client()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket()");
    }
}


int Client::connect(std::string &address, int &port){
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_aton(address.c_str(), &server_addr.sin_addr) == 0)
        return -1;
    if (::connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        return -1;
    return 0;
}

int Client::send_msg(std::string &msg)
{
    return 0;
}

int Client::recieve(char &buf)
{
    return 0;
}


