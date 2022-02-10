#include <netinet/ip.h>
#include <stdio.h>
#include <iostream>
#include "config.hpp"

using namespace std;


int main()
{
    // allocate memory for a packet
    unsigned char* pkt = (unsigned char*)calloc(1,sizeof(struct ip) +sizeof(MAX_PACKET_SIZE));
    struct ip *header = (struct ip*)pkt;
    unsigned char* data = pkt+sizeof(struct  ip);
    

    header->ip_v = 4;
    header->ip_hl = 5;
    header->ip_id = htons(0);
    header->ip_off = htons(0);
    header->ip_ttl = 64;
    header->ip_p = 17; // udp protocol
    
    //mke socket buffer to store data
    for (int i = 0; i < MAX_PACKET_SIZE; i++)
    {
        data[i] = i%256;
    }
    
   

    sockaddr_in destAddr;
    destAddr.sin_family = AF_INET;

    //make a udp socket file descriptor
    int32_t socfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if(socfd < 0)
    {
        exit(EXIT_FAILURE);
    }
    sendto(socfd, data, (size_t)(sizeof(struct ip)+pkt), 0, (struct sockaddr * )&destAddr, sizeof(destAddr));
    
    cout << pkt << endl;
}