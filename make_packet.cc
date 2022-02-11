#include <netinet/ip.h>
#include <stdio.h>
#include <iostream>
//#include "config.hpp"

using namespace std;
#define MAX_FILE_SIZE 65535

int main()
{
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
    
    //unsigned char* data = (unsigned char *)sizeof(MAX_FILE_SIZE );
    
    //put bytes into packet
    for (int i = 0; i < MAX_FILE_SIZE; i++)
    {
        data[i] = i%256;
    }

    //data[0] = sizeof(1);

    sockaddr_in destAddr;
    destAddr.sin_family = AF_INET;
    

    //make a  socket file descriptor
    int32_t socfd = socket(AF_INET, SOCK_RAW, 0);
    if(socfd < 0)
    {
        exit(EXIT_FAILURE);
    }
    
    sendto(socfd, data, (size_t)(sizeof(struct ip)+pkt), 0, (struct sockaddr * )&destAddr, sizeof(destAddr));
    
    
}