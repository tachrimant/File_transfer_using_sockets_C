# File Transfer using socket in c
## premiere étape:
créer un fichier send.tx et le remplir du contenu que vous voulez envoyer dans le coté client.
## Coté client:
on commence par l’ importation des bibliothèque necessaire pour le programme

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define SIZE 1024
```
le fichier .txt que le client veut l’envoyer au serveur 
la creation de la fonction send_file qui prend en parametre le fichier et le socket.
le but de cette fonction est de envoyer le fichier text au serveur.

```c
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
```


La creation de la fonction recieve_html_file qui prend en prametre le socket 
Le but de cette fonction est de recevoir le fichier .html qui viennent de la part de serveur

```c
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
```
Dans le main on définie l’addresse ip de serveur et le port et on crée soket et on affiche le message: ‘’serveer soket created successfuly.’’
```c
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
```

la création de connection avec le serveur 

```c
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr("192.168.204.130");

  e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e == -1) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Connected to Server.\n");
```

la lecture du fichier .txt avec la fonction fopen avec le mood ‘’read’’

```c
  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
  }
```

l’envoi du fiechier .txt  l’aide de la fonction send_file qu’on a déjà créer  avec l’affichage de message :‘’file data sent successfully’’

```c
  send_file(fp, sockfd);
  printf("[+]File data sent successfully.\n");
  close(sockfd);
``` 


la creation d’un socket de serveur et créer une connection avec le, et l’affichage des message de sucée .
le dernier étape est de recevoir le fichier html de puis le serveur est afficher des message de sucée et a la fin la fermenteur de la connexion  

```c
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
```

##	Cote serveur
on commence par l’ importation des bibliothèque necessaire pour le programme
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#define SIZE 1024
```

La création de la fonction write_file qui va ajouter les balises HTML au fichier text envoyer par le client 

```c
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
 return;
 }
```
La création de la fonction send_html_file qui va envoyer le nouveau fichier .html qui contient le message et les balise au client  
```c
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
```

La fonction main on crée un socket pour assurer la connexion avec le client
```c
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
```

Dans cette étape le serveur est en listening  en attend de l’envoie de fichier txt de la part de client

```c
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
```

Dans cette étape le serveur accepte la connexion avec le client

```c
  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  if (new_sock < 0) {
        printf("server does not accept connexion...\n");
        exit(0);
    }
```


Cette étape est pour le but d’écrire le contenu du fichier .txt envoyer par le client dans un autre fichier .html   

```c
  write_file(new_sock);
  printf("[+]Data written in the file successfully.\n");

```

La dernière étape pour le serveur c’est d’envoyer le fichier html au client 

```c
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  send_html_file(new_sock);
```

D 'après avoir les deux programmes client.c et server.c maintenant on va présenter l’exécution des ceux derniers  
On commence avec la compilation de serveur
 
```bash
gcc server.c -o server
./server
```

On passe pour compiler le client et l’exécuter 

```bash
gcc client.c -o client
./client
```

