#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define num_lines 32
#define line_size 256
#define max_clients 2

pthread_t tid[max_clients];
pthread_mutex_t cmd_lock;
pthread_mutex_t doc_lock;
pthread_mutex_t rtn_lock;

void* user();

int main()
{
  int server_sockfd, client_sockfd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  char document[num_lines][line_size];
  char cmd[10+line_size];
  char rtn[line_size];
  pthread_mutex_init(&cmd_lock, NULL);
  pthread_mutex_init(&doc_lock, NULL);
  pthread_mutex_init(&rtn_lock, NULL);
  
  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(9734);
  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, 5);
	while(1) {		
		printf("server waiting\n");
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, &client_len);
		pthread_mutex_lock(&cmd_lock);
    read(client_sockfd, &cmd, sizeof(cmd));
		if (cmd[0] == 'a') {
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
      if (cmd[0] == 'g') {
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
      }
    }
    pthread_mutex_unlock(&cmd_lock);
	}
}
void* user(){

}