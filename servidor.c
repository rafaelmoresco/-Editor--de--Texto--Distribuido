#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define num_lines 32
#define line_size 256
#define max_clients 2


pthread_mutex_t doc_lock;
char document[num_lines][line_size];

void* user( void *arg);

int main()
{
  int server_sockfd, client_sockfd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  pthread_t tid[max_clients+5];
  pthread_mutex_init(&doc_lock, NULL);
  
  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(9734);
  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, max_clients);
  int i = 0;
	while(1) {		
		printf("server waiting\n");
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, &client_len);
    if (pthread_create(&tid[i++], NULL, user, &client_sockfd)!=0) {
      printf("Failed to create thread\n");
    } else {
      printf("Thread %d created\n", &i);
    }
    if (i > max_clients) {
      i = 0;
      while (i < max_clients) {
        pthread_join(tid[i++], NULL);
      }
      i = 0;
    }
	}
}
void* user(void *arg){
  int client_sockfd = *((int *)arg);
  char cmd[10+line_size];
  char rtn[line_size];
  while (1)
  {
    read(client_sockfd, &cmd, sizeof(cmd));
		if (cmd[0] == 'a' && cmd[1] == 'd' && cmd[2] == 'd') {
      int line_index = (cmd[8]-'0')*10 + (cmd[9]-'0');
      pthread_mutex_lock(&doc_lock);
      for (int i = 0; i < 256; i++) {
        document[line_index][i] = cmd[9+i];
        if (cmd[9+i] == '\0') {
          break;
        }
      }
      pthread_mutex_unlock(&doc_lock);
    } else {
      if (cmd[0] == 'g' && cmd[1] == 'e' && cmd[2] == 't') {
        int line_index = (cmd[8]-'0')*10 + (cmd[9]-'0');
        pthread_mutex_lock(&doc_lock);
        for (int i = 0; i < 256; i++) {
          rtn[i] = document[line_index][i];
          if (document[line_index][i] == '\0') {
            break;
          }
        }
        pthread_mutex_unlock(&doc_lock);
        write(client_sockfd, &rtn, sizeof(rtn));
      } else {
        close(client_sockfd);
        break;
      }
    }
  }
}