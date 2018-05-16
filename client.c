#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <string.h>
#define PORT 8080


long BUFFER_SIZE = 4096;
int sock = 0;
struct sockaddr_in address,serv_addr;


char hash[SHA_DIGEST_LENGTH*2] = {0};
char hash2[SHA_DIGEST_LENGTH*2] = {0};
char* itob(int i);



void start_networking();
char* receive_message();
void send_message(char* message);

int main(int argc, char const *argv[])
{
    start_networking();
    char *message[BUFFER_SIZE];
    strcpy(message, "Hello from client");
    send_message(message);
    printf("Hello message sent\n");
    printf("Server Said: %s\n", receive_message());


    while(1){
      memset(&hash[0], '\0', sizeof(hash));
      strcpy(hash, receive_message());
      if(strcmp(hash, "1") == 0){
        break;
      }
      send_message("Got the hash value\n");

      for(int i=0; i<65536; i++){
        if(check_hashes() == 1){
          send_message(itob(i));
          receive_message();
          send_message("Thank you");
          break;
        } else if(i == 65535){
          printf("Can't solve the puzzle\n\n");
          send_message("can't solve");
          receive_message();
          send_message("Thank you");
        }
      }


    }


    return 0;
}

void send_message(char* message){
    send(sock, message , strlen(message) , 0 );
}

char* receive_message(){
    char* buffer = malloc(sizeof(char)*4096);
    memset(&buffer[0], '\0', sizeof(buffer));
    read( sock , buffer, BUFFER_SIZE);
    return buffer;
}

void start_networking(){
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }


}





int check_hashes(){
  if(strcmp(hash, hash2) == 0){
    return 1;
  } else {
    return 0;
  }
}

char* itob(int i) {
   static char bits[16] = {'0'};
   for(int i=0; i<16; i++){
     bits[i] = '0';
   }
   int bits_index = 15;
   while ( i > 0 ) {
      bits[bits_index--] = (i & 1) + '0';
      i = ( i >> 1);
   }
   return bits;
}










