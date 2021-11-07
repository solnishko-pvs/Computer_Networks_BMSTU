#ifndef SERVER_H
#define SERVER_H
#include "_server.h"

class Server:public _Server
{
public:
    Server(int &port);
    ~Server();
    void resize_buf(int &new_size) override;
    void resize_clients_num(int &new_size) override;
    int start() override;


private:
    int server_sock;
    int port;
    int num_of_clients = 10;
    int *clients;
    int buf_size = 256;
    char* buf;
    fd_set set;
    std::string log_path ="log.txt";

    int userHandling(unsigned int fd, unsigned int client_id) override;
    void handlingNewUser(unsigned int fd) override;
    std::string get_ext(std::string& filename) override;
    void log(std::string& filename, std::string& user_id) override;
};
#endif // SERVER_H
