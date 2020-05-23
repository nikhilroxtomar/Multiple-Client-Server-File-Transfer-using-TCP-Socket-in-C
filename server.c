#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

void send_file(FILE *fp, int sockfd){
  int n;
  char data[SIZE] = {0};

  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) < 0) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
  return;
}

int main(){
  // ip address and port
  char *ip = "127.0.0.1";
  int port  = 8888;

  // variables and structures
  int e;
  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];
  pid_t childpid;

  FILE *fp;
  char *filepath;

  // 1. creating the socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created.\n");

  // 2. writing the data in the structure
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip);
  server_addr.sin_port = port;

  // 3. binding the ip address with the port
  addr_size = sizeof(server_addr);
  e = bind(sockfd, (struct sockaddr*)&server_addr, addr_size);
  if (e < 0){
    perror("[-]Error in bind");
    exit(1);
  }
  printf("[+]Binding successfull.\n");

  // 4. listening to the clients
  e = listen(sockfd, 10);
  if (e < 0) {
    perror("[-]Error in listen");
    exit(1);
  }
  printf("[+]Listening...\n");

  // 5. accepting the client connection.

  while (1){
    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
    if (new_sock < 0){
      perror("[-]Error in accpet");
      exit(1);
    }
    printf("Connection accepted from %s:%d\n", inet_ntoa(new_addr.sin_addr), ntohs(new_addr.sin_port));

    childpid = fork();
    if (childpid == 0){
      close(sockfd);

      while(1){
        recv(new_sock, buffer, SIZE, 0);

        if (strcmp(buffer, "LIST") == 0){
          // send the list of filenames.
          bzero(buffer, SIZE);
          strcpy(buffer, "data.txt\nhello.txt");
          send(new_sock, buffer, SIZE, 0);
          bzero(buffer, SIZE);
        }

        else if (strcmp(buffer, "QUIT") == 0){
          // connection disconnected.
          printf("Connection disconnected from %s:%d\n", inet_ntoa(new_addr.sin_addr), ntohs(new_addr.sin_port));
          break;
        }

        else {
          // received the filename, send the file data.
          if (strcmp(buffer, "data.txt") == 0){
            filepath = "server_files/data.txt";
            fp = fopen(filepath, "r");
            send_file(fp, new_sock);
          }

          else if (strcmp(buffer, "hello.txt") == 0) {
            filepath = "server_files/hello.txt";
            fp = fopen(filepath, "r");
            send_file(fp, new_sock);
          }

          bzero(buffer, SIZE);
          send(new_sock, "\0", 1, 0);
          bzero(buffer, SIZE);
        }

      }
    }

  }

}
