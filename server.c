#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#define SIZE 1024

void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "recv.html";
  char buffer[SIZE];
  fp = fopen(filename, "w");
  fprintf(fp,"<!DOCTYPE html>\n");
  fprintf(fp,"<html>\n");
  fprintf(fp,"<head>\n");
  fprintf(fp,"<title>Html File</title>\n");
  fprintf(fp,"</head>\n");
  fprintf(fp,"<body>\n");
  fprintf(fp,"<p>\n");
  fclose(fp);
  printf("file opend in mode a\n");
  fp = fopen(filename, "a"); 
   while (1) {
   n = recv(sockfd, buffer, SIZE, 0);
   printf("%s\n",buffer);
   printf("%d",n);
   fprintf(fp, "%s", buffer);
   bzero(buffer, SIZE);
    if (n<=0){break;}
    }
  fprintf(fp,"</p> \n </body> \n </html>");
  fclose(fp);
 return;}

void send_html_file(int sockfd){
       int n;
      char data[SIZE] = {0};
	FILE *fp;
  char *filename = "recv.html";
  fp = fopen(filename,"r");
  if (fp == NULL){
    printf("error in opening file\n");
    exit(1);
  }
      while(fgets(data, SIZE, fp) != NULL) {
      
      printf("%s\n",data);
      if (send(sockfd, data, sizeof(data), 0) == -1) {
          perror("[-]Error in sending file.");
          exit(1);
        }
       
        bzero(data, SIZE);
      }
return;
}

int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;
  int sockfd, new_sock,sendingsock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e < 0) {
    perror("[-]Error in bind");
    exit(1);
  }
  printf("[+]Binding successfull.\n");

  if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		perror("[-]Error in listening");
    exit(1);
	}

  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  if (new_sock < 0) {
        printf("server does not accept connexion...\n");
        exit(0);
    }
    
  write_file(new_sock);
  printf("[+]Data written in the file successfully.\n");

  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  send_html_file(new_sock);
  return 0;
}
