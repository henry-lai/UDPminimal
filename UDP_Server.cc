#include <stdio.h>
#include <iostream>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;

#define BUFFERLEN 1024
#define PORT 8888

int main(void)
{
    sockaddr_in serveraddr,cliaddr;
    int sendlen = sizeof(cliaddr), recv_len;
    char buffer[BUFFERLEN];

    //Create UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd == -1)
    {
        perror(" Socket creation failed \recv_len"); exit(EXIT_FAILURE);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(PORT);

    if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("Bind failed"); exit(EXIT_FAILURE);
    }

    while(1)
    {
        //keep listening for data
        printf("Server Running \n");
        if((recv_len = recvfrom(sockfd, buffer, BUFFERLEN, 0, (struct sockaddr *)&cliaddr, (socklen_t *)&sendlen)) == -1)
        {
            perror(" recv()n failed \n"); exit(EXIT_FAILURE);
        }
        printf("Received pkt from %s:%d\n", inet_ntoa(serveraddr.sin_addr), ntohs(serveraddr.sin_port));
        printf("Client : %s \n", buffer);
        printf("Sending Reply...\n");

        string mesg = "Hello from Server\n";
        if( sendto(sockfd, mesg.c_str(), strlen(mesg.c_str()), 0 , (sockaddr *) &cliaddr, sendlen) == -1)
        {
            perror(" Sendto failed \n"); exit(EXIT_FAILURE);
        }
    }
    close(sockfd);
    return 0;
}
