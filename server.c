#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "openssl/sha.h"
#define PORT 8080


long BUFFER_SIZE = 4096;
int server_fd, sock;
char buffer[4096] = {0};
int opt = 1;
struct sockaddr_in address;
int addrlen = sizeof(address);

void start_networking();
char* receive_message();
void send_message(char* message);

int main(int argc, char const *argv[])
{
    start_networking();
    printf("Client Said: %s\n", receive_message());


    char message[BUFFER_SIZE];
    strcpy(message, "Hey from Server\n");
    send_message(message);
    printf("Hello from server to client sent\n");
    return 0;
}

void send_message(char* message){
    send(sock , message , strlen(message) , 0 );
}

char* receive_message(){
    memset(&buffer[0], '\0', sizeof(buffer));
    read( sock , buffer, BUFFER_SIZE);
    return buffer;
}

void start_networking(){
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((sock = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }


}
