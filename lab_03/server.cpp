#include "server.h"

Server::Server(int &p)
{
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        perror("socket()");
    }
    port = p;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind()");
    }

    buf = new char[buf_size];
    clients = new int[num_of_clients];
    FD_ZERO(&set);
}



int Server::start()
{
    if (listen(server_sock, 5) < 0)
    {
        return -1;
    }

    std::cout << "Server configured. Listening on port " << port << std::endl;

    while(true)
    {
        int max_file_descriptor = server_sock;

        FD_SET(server_sock, &set);

        for (int i = 0; i < num_of_clients; i++) {
            if (clients[i] > 0)
                FD_SET(clients[i], &set);
            if (clients[i] > max_file_descriptor) {
                max_file_descriptor = clients[i];
            }
//            std::cout << clients[i];
        }
//        std::cout << std::endl;
        int select_ret = select(max_file_descriptor + 1, &set, NULL, NULL, NULL);
        if (select_ret == 0) {
            std::cout << "\nTime is out. Sever closed the connection.\n\n";
            return 0;
        }
        else if (select_ret < 0) {
            perror("select()");
            return -1;
        }

        if (FD_ISSET(server_sock, &set))
            this->handlingNewUser(server_sock);

        for (int i = 0; i < num_of_clients; i++)
        {
            int fd = clients[i];
            if ((fd > 0) && FD_ISSET(fd, &set))
                this->userHandling(fd, i);
        }
    }
    return 0;
}


int Server::userHandling(unsigned int fd, unsigned int client_id)
{
    int recvSize = 0;
    std::string result_msg("");
    while((recvSize = recv(fd, buf, buf_size, 0)) == buf_size)
    {
//        std::cout << recvSize << std::endl;
        result_msg.append(buf);
//        buf[recvSize] = '\0';
//        std::cout << std::string(buf) << std::endl;
    }

    if (recvSize < 0)
    {
        FD_CLR(fd, &set);
        close(fd);
        clients[client_id] = 0;
        perror("server recv()");
        return -1;
    }
    if (recvSize == 0)
    {
        std::cout << "\nClient #" << client_id + 1 << " disconnected\n\n";
        FD_CLR(fd, &set);
        close(fd);
        clients[client_id] = 0;
        return 0;
    }

    std::cout << recvSize << std::endl;
    buf[recvSize] = '\0';
    result_msg.append(buf);

    char *cstr = new char[result_msg.length() + 1];
    strcpy(cstr, result_msg.c_str());

    std::string method = std::string(strtok(cstr, " "));
    std::string filename = std::string(strtok(NULL, " /"));
    std::string version = std::string(strtok(NULL, " \r\n"));
    std::string tag = std::string(strtok(NULL, "\r\n:"));
    std::string user;
    if (tag == "User")
        user = std::string(strtok(NULL, " \n"));
    else
        user = "Unknown";

    if (method != "GET")
        return -1;

    std::string status, status_code;
    std::string body = "";
    std::string content_type;
    std::string response(version);
    std::ifstream fin(filename);
    if (fin.is_open())
    {
        std::string ext = this->get_ext(filename);
        if (ext == "html")
        {
            this->log(filename, user);
            content_type = "text/html";
        }
        else if (ext == "pdf")
            content_type = "application/pdf";
        else if (ext == "json")
            content_type = "application/pdf";
        else if (ext == "png")
            content_type = "image/png";
        else if (ext == "jpeg")
            content_type = "image/jpeg";
        else
            content_type = "application/octet-stream";

        std::string tmp;
        while (std::getline(fin, tmp))
            body += tmp + "\n";

        status_code = "200";
        status = "OK";
    }
    else
    {
        status_code = "404";
        status = "Not Found";
        content_type = "text/html";
        body = "<!doctype html><html>\n\r<body>\n\r<center><h1>404 Not Found</h1></center><hr>\n\r</body>\n\r</html>";
    }
    response.append(" " + status_code + " " + status + "\r\n");
    response.append("Content-Type: " + content_type + "\r\n");
    response.append("Content-Length: " + std::to_string(body.length()) + "\r\n");
    response.append("\r\n");
    response.append(body);

//    std::cout << filename << std::endl << version << std::endl << tag << std::endl << user << std::endl << body << std::endl;

    std::cout << "client #" << client_id + 1 << " sended a message: " << result_msg << std::endl;

    if (send(fd, response.c_str(), response.length(), 0) < 0)
    {
        delete [] cstr;
        perror("send()");
        return -1;
    }
    delete [] cstr;
    return 0;
}

void Server::handlingNewUser(unsigned int fd)
{
    struct sockaddr_in address;
    int sizeof_address = sizeof(address);

    int incom = accept(fd, (struct sockaddr*) &address, (socklen_t*) &sizeof_address);
    if (incom < 0) {
        perror("error accept()");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    int flag = 0;
    for (; i < num_of_clients && !flag; i++){
        if (clients[i] == 0) {
            clients[i] = incom;
            flag = 1;
        }
    }

    std::cout << "\nNew connection\nClient #" << i << ": " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << "\n\n";
}

std::string Server::get_ext(std::string &filename)
{
    int pos = filename.rfind('.');
    if (pos == std::string::npos)
        return "";
    return filename.substr(pos + 1);
}

void Server::log(std::string& filename, std::string& user_id)
{
    std::ofstream fout(log_path, std::ios::app);
    if (fout.is_open())
        fout << "Page:" << filename << " User: " << user_id << std::endl;
        fout.close();
    return;
}

void Server::resize_buf(int &new_size)
{
    buf_size = new_size;
    delete buf;
    buf = new char[buf_size];
}

void Server::resize_clients_num(int &new_size)
{
    num_of_clients = new_size;
    delete clients;
    clients = new int[num_of_clients];
}

Server::~Server()
{
    for(int i = 0; i < num_of_clients; i++)
    {
        int fd = clients[i];
        FD_CLR(fd, &set);
        shutdown(fd, 2);
        close(fd);
        clients[i] = 0;
    }
    FD_CLR(server_sock, &set);
    shutdown(server_sock, 2);
    close(server_sock);
    delete buf;
    delete clients;
    std::cout << "server stopped";
}

//int main(void)
//{
//    int port = 8888;
//    _Server *serv = new Server(port);
//    serv->start();
//    return 0;
//}
