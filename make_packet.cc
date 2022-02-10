#include <netinet/ip.h>
#include <stdio.h>
#include <iostream>
#include "config.hpp"

using namespace std;


int main()
{
    // allocate memory for a packet
    unsigned char* pkt = (unsigned char*)calloc(1,sizeof(MAX_PACKET_SIZE));

    //allocating memory in pkt for ip header
    struct ip *header = (struct ip*)pkt;
    header->ip_v = 4;
    header->ip_hl = 5;
    header->ip_id = htons(0);
    header->ip_off = htons(0);
    header->ip_ttl = 64;
    header->ip_p = 17; // udp protocol
    
    sockaddr_in destAddr;
    destAddr.sin_family = AF_INET;

    
    cout << pkt << endl;
}