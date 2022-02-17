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
#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to send data

#define MAX_FILE_SIZE 65535

using namespace std;

int main(void)
{
	struct sockaddr_in destAddr;
	int socfd,  slen=sizeof(destAddr);
	char buf[BUFLEN];
	char message[BUFLEN];

	if ( (socfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		perror(" Socket creation failed \n");
        exit(EXIT_FAILURE);
	}

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
    for (int i = 0; i < MAX_FILE_SIZE; i++) // put bytes into packet
    {
        data[i] = i * 1;
    }

    memset((char *) &destAddr, 0, sizeof(destAddr));
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(PORT);
	
    //convert network dot addr to binary
	if (inet_aton(SERVER , &destAddr.sin_addr) == 0) 
	{
		fprintf(stderr, "inet_aton() failed\n");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		printf("Enter message : ");
		cin.getline(message, sizeof message);
		//send the message
		if (sendto(socfd, message, strlen(message) , 0 , (struct sockaddr *) &destAddr, slen)==-1)
		{
			perror(" Sendto() failed \n");
            exit(EXIT_FAILURE);
		}
		
        //bats pkt send()
        // if( sendto(socfd, data, (size_t)(sizeof(struct ip)+pkt), 0, (struct sockaddr * )&destAddr, sizeof(destAddr)) == -1)
        // {
        //     perror(" Sendto() failed \n");
        //     exit(EXIT_FAILURE);
        // }


		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		memset(buf,'\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(socfd, buf, BUFLEN, 0, (struct sockaddr *) &destAddr, ( socklen_t *)&slen) == -1)
		{
			perror(" recvfrom() failed \n");
            exit(EXIT_FAILURE);
		}
		puts(buf);
	}
	close(socfd);
	return 0;
}