#include <stdio.h>
#include <iostream>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;

int main()
{
    //Create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1){
        perror(" Socket creation failed \n");
        exit(EXIT_FAILURE);
    }

    //set server address
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    //inet_aton("127.0.0.1", (in_addr *)&serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = 8080;

    //bind socket with server address

    if(bind(sockfd,(struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Server Running \n");

    int len, n;
    char buffer[1024];
    sockaddr cliaddr;
    len = sizeof(cliaddr);

    n = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&serveraddr, (socklen_t *)&len);

    buffer[n] = '\0';
    printf("Client : %s \n", buffer);

    string mesg = "Hello from Server \n";
    sendto(sockfd, (char *)mesg.c_str(), strlen(mesg.c_str()), MSG_CONFIRM, (sockaddr *) &cliaddr, len);

    close(sockfd);
    return 0;
}
