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

char hash[SHA_DIGEST_LENGTH*2] = {0};
char hash2[SHA_DIGEST_LENGTH*2] = {0};
char* removed;
char* random_string[4096];
char* generate_puzzle();
char* binary;
char pre_image[4096] = {0};



void start_networking();
char* receive_message();
void send_message(char* message);

int main(int argc, char const *argv[])
{


    int count = 0;
    struct timeval t1, t2;
    double elapsedTime;

    start_networking();
    printf("Client Said: %s\n", receive_message());


    char message[BUFFER_SIZE];
    strcpy(message, "Hey from Server\n");
    send_message(message);
    printf("Hello from server to client sent\n");


    gettimeofday(&t1, NULL);
    while( count < 1000){
	memset(hash, '\0', sizeof(hash));
        memset(hash2, '\0', sizeof(hash2));
        memset(pre_image, '\0', sizeof(pre_image));
	generate_puzzle();
	send_message(hash);
      	receive_message();
	printf("Client Said: %s\n", receive_message());
	send_message(removed);
      	receive_message();
	send_message("Received your request\n");
      	receive_message();
	count++;
    }
    


    gettimeofday(&t2, NULL);
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
    elapsedTime = elapsedTime/1000;
    printf("TIME ELAPSED: %f\n", elapsedTime);
    send_message("1");



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








int random_number(){
  return rand() % 9000 + 1000;
}

void number_to_string(int num) {
  char temp[12];
  sprintf(temp, "%d", num);
  strcpy(random_string, temp);
}


void hash_string(char* s) {
  memset(&hash[0], '\0', sizeof(hash));
  unsigned char temp[SHA_DIGEST_LENGTH] = {'0'};
  SHA1((unsigned char*)s, strlen(s), temp);
  for(int i=0; i<SHA_DIGEST_LENGTH; i++){
    sprintf((char*)&(hash[i*2]), "%02x", temp[i]);
  }
}

char* stringToBinary(char* s) {
  if(s == NULL) return 0;
    size_t len = strlen(s);
    char *binary = malloc(len*8 + 1);
    binary[0] = '\0';
    for(size_t i = 0; i < len; ++i) {
        char ch = s[i];
        for(int j = 7; j >= 0; --j){
            if(ch & (1 << j)) {
                strcat(binary,"1");
            } else {
                strcat(binary,"0");
            }
        }
    }
    return binary;
}


char* remove_bits(int k, char* s){
  char* temp = s;
  temp[strlen(s)-k] = 0;
  return temp;
}

char* generate_puzzle(){
  char message[12];
  int num = random_number();
  number_to_string(num);
  hash_string(random_string);
  strcpy(hash2, hash);
  binary = stringToBinary(hash2);
  hash_string(binary);
  removed = remove_bits(16, binary);
}

