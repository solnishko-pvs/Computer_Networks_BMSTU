#include "client.h"

Client::Client()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket()");
    }
    buf = new char[buf_size];
}


int Client::connect(std::string &address, int &port)
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_aton(address.c_str(), &server_addr.sin_addr) == 0)
        return -1;
    if (::connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connection()");
        return -1;
    }
    return 0;
}

int Client::reconnect(std::string &address, int &port)
{
    close(sock);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket()");
        return -1;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_aton(address.c_str(), &server_addr.sin_addr) == 0)
        return -1;
    if (::connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connection()");
        return -1;
    }
    return 0;
}

int Client::send_msg(std::string &msg)
{
    if (send(sock, msg.c_str(), msg.length(), 0) < 0)
        return -1;
    return 0;
}

int Client::recieve()
{
    int recvSize = 0;
    std::string result_msg;


    while((recvSize = recv(sock, buf, buf_size, 0)) == buf_size)
    {
//        std::cout << recvSize << std::endl;
        result_msg.append(buf);
//        buf[recvSize] = '\0';
//        std::cout << std::string(buf) << std::endl;
    }
    if (recvSize < 0)
        return -1;
    std::cout << recvSize << std::endl;
    buf[recvSize] = '\0';
    result_msg.append(buf);
    std::cout << result_msg << std::endl;
    return recvSize;
}

void Client::resize_buf(int &new_size)
{
    buf_size = new_size;
    delete buf;
    buf = new char[buf_size];
}

std::string Client::get_request(std::string &filename)
{
    const std::string version = "HTTP/1.1";
    return "GET /" + filename + " " + version + "\r\n" + "User: console-pid-" + std::to_string(getpid());
}

Client::~Client()
{
    close(sock);
    delete buf;
    std::cout << "client stopped";
}


//int main(void)
//{
//    std::cout << "start" << std::endl;
//    _Client *cli = new Client();
//    std::string addr = "127.0.0.1";
//    int port = 8888;
//    if (cli->connect(addr, port) == -1)
//    {
//        std::cout << "connection failed" << std::endl;
//        return -1;
//    }
//    std::string msg;
//    std::string request;
//    int status = 0;
//    while(true)
//    {
//        std::cout << "filename: ";
//        std::cin >> msg;
//        request = cli->get_request(msg);
//        if (cli->send_msg(request) < 0)
//        {
//            std::cout << "sending failed" << std::endl;
//            return -1;
//        }

//        if ((status = cli->recieve()) < 0)
//        {
//            std::cout << "recieving failed" << std::endl;
//            return -1;
//        }

//        else if (status == 0)
//        {
//            std::cout << "server closed connection" << std::endl;
//            std::cout << "reconnecting..." << std::endl;
//            if (cli->reconnect(addr, port) == -1)
//            {
//                std::cout << "connection failed" << std::endl;
//                return -1;
//            }
//            else if (cli->send_msg(request) < 0)
//            {
//                std::cout << "sending failed" << std::endl;
//                return -1;
//            }
//            if ((status = cli->recieve()) < 0)
//            {
//                std::cout << "recieving failed" << std::endl;
//                return -1;
//            }
//        }
//    }

//    return 0;
//}
