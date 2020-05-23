#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

void remove_char(char *s, int c){
  /* This function is used to remove a character from the character array. */
  int j, n = strlen(s);
  for (int i=j=0; i<n; i++)
    if (s[i] != c){
      s[j++] = s[i];
    }
    s[j] = '\0';
}

void write_file(char *filepath, int sockfd){
  int n;
  FILE *fp;
  char buffer[SIZE];

  fp = fopen(filepath, "w");
  if (fp == NULL) {
    perror("[-]Error in creating file");
    exit(1);
  }

  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    if (n == 1) {
      break;
      return;
    }

    fprintf(fp, "%s", buffer);
    fflush(fp);
    bzero(buffer, SIZE);
  }
  return;
}

int main(){
  // ip address and port
  char *ip = "127.0.0.1";
  int port = 8888;

  // variables and structures,
  int e;
  int sockfd;
  struct sockaddr_in server_addr;
  char buffer[SIZE];
  char *filepath;

  // 1. creating the socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Client socket created.\n");

  // 2. writing the data in the structure
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip);
  server_addr.sin_port = port;

  // 3. connect to the server
  e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(sockfd < 0){
    perror("[-]Error in connect");
    exit(1);
  }
  printf("[+]Connected to the server\n");

  printf("\n");
  printf("List of the commands.\n");
  printf("LIST - list all the files.\n");
  printf("LOAD - download the file.\n");
  printf("       LOAD <path>\n");
  printf("QUIT - disconnect from the server.\n");

  while(1){
    fflush(stdout);
    printf("> ");
    fgets(buffer, SIZE, stdin);

    if (strlen(buffer) > 1){
      char *token1 = strtok(buffer, " ");
      char *token2 = strtok(NULL, " ");
      remove_char(token1, '\n');

      if (strcmp(token1, "LIST") == 0) {
        // list all the file of the server.
        send(sockfd, buffer, SIZE, 0);
        recv(sockfd, buffer, SIZE, 0);
        printf("%s\n", buffer);
      }

      else if (strcmp(token1, "LOAD") == 0) {
        if (token2 == NULL) {
          printf("[-]Specify the correct filename.\n");
        } else {
          // save the data of the file received from the server.
          remove_char(token2, '\n');
          send(sockfd, token2, SIZE, 0);

          if (strcmp(token2, "data.txt") == 0){
            filepath = "client_files/data.txt";
            write_file(filepath, sockfd);
            printf("[+]File saved.\n");
          }

          else if (strcmp(token2, "hello.txt") == 0) {
            filepath = "client_files/hello.txt";
            write_file(filepath, sockfd);
            printf("[+]File saved.\n");
          }

          else {
            printf("Incorrect path\n");
          }
        }
      }

      else if (strcmp(token1, "QUIT") == 0) {
        // disconnect from the server.
        printf("[+]Disconnected from the server.\n");
        send(sockfd, token1, SIZE, 0);
        break;
      }

      else {
        printf("[-]Invalid command\n");
      }
    }

    bzero(buffer, SIZE);
  }

}
