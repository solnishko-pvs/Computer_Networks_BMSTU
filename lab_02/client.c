#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "header.h"

void perror_and_exit(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in server_addr;
    int sock, slen = sizeof(server_addr);
    char buf[MSG_LEN];

    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        perror_and_exit("socket()");

    memset((char *)&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_aton(SERV_IP, &server_addr.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    printf("Start client.\nTo stop server type \"stops\".\nTo stop client type \"stopc\".\n");

    while(1)
    {
        printf("Input message: ");
        scanf("%9s", buf);
	if (!strcmp(buf, "stopc"))
        {
            printf("Client stopped\n");
            close(sock);
            return 0;
        }
	
        if (sendto(sock, buf, MSG_LEN, 0, (struct sockaddr*)&server_addr, slen) == -1)
            perror_and_exit("sendto()");
    }

}
