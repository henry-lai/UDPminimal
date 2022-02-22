#include<stdio.h>	//printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include <unistd.h>
#include<stdio.h>
#include <iostream>
#include <netinet/ip.h>

#define SERVER "127.0.0.1"
#define BUFFERLEN 1024	//Max length of buffer
#define PORT 8888	//The port on which to send data

#define MAX_FILE_SIZE 65535
#define MAX_PACKET_SIZE 66000
#define IPPROTO_BATS 225
#define MTU (20 + 1480) 
#define BATCH_SIZE 4
#define COEF_HEADER_LENGTH BATCH_SIZE  // TODO: change this to BATCH_SIZE * OUTER CODE FIELD SIZE
#define ENCODED_PKT_SIZE  (MTU-sizeof(struct BatsHeader)-COEF_HEADER_LENGTH)

using namespace std;

//TODO Fn() for encoderID
uint64_t getEncoder()
{

}
//TODO Fn() for packetId

//TODO Fn() to generate UDP packet

//TODO Fn() to receive regular pkt




int main(void)
{
	sockaddr_in this_addr,peer_addr;
    int sendlen = sizeof(peer_addr), recv_len;
    char buffer[BUFFERLEN];

    //Create UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd == -1){
        perror(" Socket creation failed \recv_len");
        exit(EXIT_FAILURE);
    }

    //set this node address
    //memset(&this_addr,0, sizeof(this_addr)); // zero out structure
    this_addr.sin_family = AF_INET;
    this_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    this_addr.sin_port = htons(PORT);

    //bind socket with server address
    if(bind(sockfd, (struct sockaddr *)&this_addr, sizeof(this_addr)) == -1)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        //keep listening for data
        printf("Server Running \n");

        if((recv_len = recvfrom(sockfd, buffer, BUFFERLEN, 0, (struct sockaddr *)&this_addr, (socklen_t *)&sendlen)) == -1)
        {
            perror(" recv()n failed \n");
            exit(EXIT_FAILURE);
        }

        printf("Received pkt from %s:%d\n", inet_ntoa(this_addr.sin_addr), ntohs(this_addr.sin_port));
        printf("Client : %s \n", buffer);

        if( sendto(sockfd, buffer, recv_len, 0, (sockaddr *) &peer_addr, sendlen) == -1)
        {
            perror(" Sendto failed \n");
            exit(EXIT_FAILURE);
        }
    }

    close(sockfd);
    return 0;
}



  //allocate memory for single packet 
    //unsigned char* pkt = (unsigned char*) calloc( 1, sizeof(struct ip*) +  MAX_FILE_SIZE);

    // allocate memory for header
    // struct ip *header = (struct ip *)pkt;
    // header->ip_v = 4;
    // header->ip_hl = 4;
    // header->ip_id = htons(0);
    // header->ip_off = htons(0);
    // header->ip_ttl = 8;
    // header->ip_p = 8;

     //bats pkt send()
        // if( sendto(socfd, data, (size_t)(sizeof(struct ip)+pkt), 0, (struct sockaddr * )&destAddr, sizeof(destAddr)) == -1)
        // {
        //     perror(" Sendto() failed \n");
        //     exit(EXIT_FAILURE);
        // }
//unsigned char* data = pkt + sizeof(struct  ip*);
// for (int i = 0; i < MAX_FILE_SIZE; i++) // put bytes into packet
//     {
//         data[i] = i * 1;
//     }