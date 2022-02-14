#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <iostream>
//#include "config.hpp"

using namespace std;
#define MAX_FILE_SIZE 65535

int main()
{
    char *DEST_IP;
    //allocate memory for single packet 
    unsigned char* pkt = (unsigned char*) calloc( 1, sizeof(struct ip*) +  MAX_FILE_SIZE);
    
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
    sockaddr_in destAddr;
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = 8080;
    //destAddr.sin_addr.s_addr = INADDR_ANY;
    inet_aton("127.0.0.1", (in_addr *)&destAddr.sin_addr.s_addr); 

    //make a  socket file descriptor
    int32_t socfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socfd == -1)
    {
        perror("Socket create failed \n");
        exit(EXIT_FAILURE);
    }
    

    int n, len;
    char buffer[1024];
    sendto(socfd, data, (size_t)(sizeof(struct ip)+pkt), MSG_CONFIRM, (struct sockaddr * )&destAddr, sizeof(destAddr));
    printf("Client saying Hello.... \n");

    n = recvfrom(socfd,(char *) buffer, 1024, MSG_WAITALL,(struct sockaddr * )&destAddr,(socklen_t *) &len);
}