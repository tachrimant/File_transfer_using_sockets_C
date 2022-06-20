#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define SIZE 1024

void send_file(FILE *fp, int sockfd){
  int n;
  char data[SIZE] = {0};
  while(fgets(data, SIZE, fp) != NULL) {
  printf("%s\n",data);
    if (send(sockfd, data, sizeof(data), 0) <=0) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  } 
  return;
}

void recieve_html_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "recvfromserver.html";
  char buffer[SIZE];
  fp = fopen(filename, "w");
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    if (n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}

int main(){
  char *ip = "192.168.204.130";
  int port = 8080;
  int e;
  int sockfd,newsocket;
  struct sockaddr_in server_addr;
  FILE *fp;
  char *filename = "send.txt";
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr("192.168.204.130");

  e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e == -1) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Connected to Server.\n");

  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
  }

  send_file(fp, sockfd);
  printf("[+]File data sent successfully.\n");
  close(sockfd);
	
  newsocket = socket(AF_INET, SOCK_STREAM, 0);
  if(newsocket < 0) {
    perror("[-]Error in the new socket");
    exit(1);
  }
  printf("[+]Server newsocket created successfully.\n");
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr("192.168.204.130");

  e = connect(newsocket, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e == -1) {
    perror("[-]Error in connecting with new socket");
    exit(1);
  }
  printf("[+]Connected to Server with new socket.\n");

  recieve_html_file(newsocket);
  printf("[+]Html File been recieved successfully.\n");
  printf("[+]Closing the connection.\n");
  close(newsocket);
  
  return 0;
}
