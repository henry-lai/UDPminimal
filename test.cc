#include <stdio.h>	
#include <cstdint>
#include <string.h> 
#include <stdlib.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <netinet/ip.h>
#include <chrono>

#include "test.hpp"

#define ADDR "127.0.0.1"
#define BUFFERLEN 1024	//Max length of buffer
#define PORT 8888	//The port on which to send data

using namespace std;

//static int32_t sockfd;
double OVERHEAD = 1.1;

struct EncodeFile {
    uint64_t encoderId;
    uint16_t fileId;
    uint32_t len;
    int32_t count;
    unsigned char buf[65540];
};

//Create Bat packet
//puts values into encode file. adds bats header, send final pkt to socket
void encode(unsigned char* pkt, sockaddr_in dest_addr, int32_t sockfd)
{   
    uint16_t fileId= rand() % (MAX_FILE_ID >> 1);
    unsigned char *buf = new unsigned char[MAX_FILE_SIZE];
    //uint32_t len;
    //BatsEncoder *encoder;
    //EncodeFile *encodefile;

    struct ip *iph = (struct ip *)pkt;
    uint64_t encoderId = ((uint64_t)ntohl(iph->ip_src.s_addr) << 32) + ntohl(iph->ip_dst.s_addr);    // Set EncoderId as combination of source addr and dest addr
    int batchNum, numOfPackets, opt_pkt_size;
    //packetSizeOptimizer(encodefile->len , OVERHEAD, batchNum, numOfPackets, opt_pkt_size);
    
    numOfPackets = 10; 
    opt_pkt_size = 10;
    
    unsigned char *pkts[BATCH_SIZE];
    unsigned char *batches[BATCH_SIZE];
    //unsigned char *rawFile[numOfPackets];
    
    // for (int32_t i = 0; i < numOfPackets; i++)
    // {
    //     rawFile[i] = encodefile->buf + i * opt_pkt_size;
    // }
    
    for (int i = 0; i < BATCH_SIZE; i++)
    {   
        pkts[i] = (unsigned char*) calloc( 1, sizeof(struct BatsHeader) + (COEF_HEADER_LENGTH + PAD32(opt_pkt_size)));
        batches[i] = pkts[i] + sizeof(struct BatsHeader);
    }
    
    //encoder = BatsEncoderNoPreInit(BATCH_SIZE, numOfPackets, opt_pkt_size, rawFile);
    
    
    // for (int32_t i = 0; i < batchNum; i++)
    // {
        //BatchParam param = BatsEncoderGenBatch(encoder, batches);
        for (int32_t j = 0; j < BATCH_SIZE ; j++) 
        {
            struct BatsHeader *header = (struct BatsHeader *)pkts[j]; 
            
            header->encoderId = encoderId;
            header->fileId = htons(fileId);
            header->numOfPackets = htons(numOfPackets);

            // Requires bats lib == a lot of dependencies
            //header->param.batch_id = htons(param.batch_id); 
            //header->param.degree = htonl(param.degree);
            
            // printf("[%lX] Encode file %u, %d pkts,  - %d encoded pkt size, to %s\n",
            //      encoderId, fileId, numOfPackets,  opt_pkt_size, inet_ntoa(dest_addr.sin_addr));

            if ( sendto ( sockfd, pkts[j], sizeof(struct BatsHeader) + COEF_HEADER_LENGTH + opt_pkt_size, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) == -1)
            {
                perror(" Sendto failed: "); exit(EXIT_FAILURE);
            }
        }
    //}

    //Clean up memory 
    for (int32_t i = 0; i < BATCH_SIZE ; i++) 
    { 
        free(pkts[i]); 
    }
}

int main(void)
{
    //dest. address
    sockaddr_in destAddr;
    int sendlen = sizeof(destAddr); 
    int recv_len;
    //char buffer[BUFFERLEN];
    unsigned char recv_buf[65535];

    //Create UDP socket
    int32_t sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if ( sockfd == -1 )
    {
        perror(" Socket creation failed \n"); exit(EXIT_FAILURE);
    }

    //set this node address
    memset((char *) &destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(PORT);
    if (inet_aton(ADDR , &destAddr.sin_addr) == 0) 
	{
		fprintf(stderr, "inet_aton() failed\n"); exit(EXIT_FAILURE);
	}

    // if(bind(sockfd, (struct sockaddr *)&destAddr, sizeof(destAddr)) == -1)
    // {
    //     perror("Bind failed"); exit(EXIT_FAILURE);
    // }

    printf(" Running \n");
    int count =0;
    while( count < 10)
    {
        encode(recv_buf, destAddr, sockfd); //  encode()

        //read(sockfd, recv_buf,sizeof(recv_buf));

        if((recv_len = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&destAddr, (socklen_t *)&sendlen)) == -1)
        {
            perror(" recv()n failed: "); exit(EXIT_FAILURE);
        }
        puts( (const char *)recv_buf );
        count++;
    }
    close(sockfd);
    return 0;
}
