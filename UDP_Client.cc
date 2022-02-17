#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
//#include "config.hpp"

using namespace std;

#define MAX_FILE_SIZE 65535
#define SERVER "127.0.0.1"

#define BUFLEN 1024	//Max length of buffer
#define PORT 8888	//The port on which to send data


int main(void)
{
    char *DEST_IP;
    sockaddr_in destAddr;
    int n, len;
    char buffer[BUFLEN];

    //allocate memory for single packet 
    unsigned char* pkt = (unsigned char*) calloc( 1, sizeof(struct ip*) +  MAX_FILE_SIZE);
    
    //make a  socket file descriptor
    int32_t socfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socfd == -1)
    {
        perror("Socket create failed \n");
        exit(EXIT_FAILURE);
    }

    // allocate memory for header                                
        // struct ip *header = (struct ip *)pkt;
        // header->ip_v = 4;
        // header->ip_hl = 4;
        // header->ip_id = htons(0);
        // header->ip_off = htons(0);
        // header->ip_ttl = 8;
        // header->ip_p = 8; 
    
    unsigned char* data = pkt + sizeof(struct  ip*);
    
    // put bytes into packet
    for (int i = 0; i < MAX_FILE_SIZE; i++)
    {
        data[i] = i * 1;
    }
    
    //set dest addr
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = PORT;
    if (inet_aton(SERVER, &destAddr.sin_addr) == 0) //convert network addr to binary
    {
        fprintf(stderr, "inet_aton() failed\n");
        perror(" inet_aton() failed \n");
		exit(1);
    }

    while(1)
    {
        printf("Enter message : ");
		cin.getline(buffer, sizeof buffer);

        //send the message
		if (sendto(socfd, buffer, strlen(buffer) , 0 , (struct sockaddr *) &destAddr, sizeof(destAddr))==-1)
		{
			perror(" Sendto() failed \n");
            exit(EXIT_FAILURE);
		}
        memset(buffer,'\0', BUFLEN);
        
        if(recvfrom(socfd,(char *) buffer, BUFLEN, 0,(struct sockaddr * )&destAddr,(socklen_t *) &len) == -1)
        {
            perror("recvfrom() failed \n");
            exit(EXIT_FAILURE);
        }
        puts(buffer);
    }
    close(socfd);
    return 0;
    
    // sendto(socfd, data, (size_t)(sizeof(struct ip)+pkt), MSG_CONFIRM, (struct sockaddr * )&destAddr, sizeof(destAddr));
    // printf("Client saying Hello.... \n");

    // n = recvfrom(socfd,(char *) buffer, 1024, MSG_WAITALL,(struct sockaddr * )&destAddr,(socklen_t *) &len);
}