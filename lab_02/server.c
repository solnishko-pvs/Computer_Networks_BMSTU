#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "header.h"


int algorithm(long int num, int base, char* res)
{
    char letters[36] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (base < 1 || base > 36)
        return -1;

    long int tmp = num;
    int rest = tmp % base;
    tmp /= base;
    if (tmp == 0)
    {
        res[0] = letters[rest];
        return 1;
    }
    int k = algorithm(tmp, base, res);
    res[k++] = letters[rest];
    return k;
}

int convert_number(char* s)
{
    if (strlen(s) < 1)
    {
        printf("empty string.\n\n");
        return -1;
    }


    if (s[0] < '0'  || s[0] > '9')
    {
        printf("not a number.\n\n");
        return -1;
    }

    long int num = atol(s);

    char res2[70] = {0};
    if (algorithm(num, 2, res2) == -1)
        return -1;
    printf("bin: %s\n", res2);

    char res8[20] = {0};
    if (algorithm(num, 8, res8) == -1)
        return -1;
    printf("oct: %s\n", res8);

    printf("dec: %ld\n", num);

    char res16[20] = {0};
    if (algorithm(num, 16, res16) == -1)
        return -1;
    printf("hex: %s\n\n", res16);

    return 0;
}


int main(void)
{
    struct sockaddr_in server_address, client_address;
    int sockfd;
    socklen_t cli_addr_len = sizeof(client_address);
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("socket()");
        exit(1);
    }

    memset((char *)&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);


    if (bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1)
    {
        perror("bind()");
        exit(1);
    }

    printf("Server started.\n");
    char msg[MSG_LEN];

    while(strcmp(msg, "stops"))
    {
        if (recvfrom(sockfd, msg, sizeof(msg), 0, (struct sockaddr*)&client_address, &cli_addr_len) == -1)
        {
            perror("recvfrom()");
            exit(1);
        }


        printf("msg: %s from: %s:%d\n", msg, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        convert_number(msg);

    }
    printf("stop server\n");
    close(sockfd);
    return 0;
}


        
        

