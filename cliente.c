#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main()
{
  int sockfd;
  int len;
  struct sockaddr_in address;
  int result;
  char op[8], ind[2], text[256], cmd[266];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  //address.sin_addr.s_addr = inet_addr("150.162.65.154");
  //address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(9734);

  len = sizeof(address);
  result = connect(sockfd, (struct sockaddr *)&address, len);
  if(result == -1) {
		perror("oops: client1");
		exit(1);
  }
	while (1) {
    printf("Type Operation: ");
    scanf("%s", &op);
    printf("%s\n", &op);
    if (strcmp(op, "add_line") == 0) {
      printf("Type Line: ");
      scanf("%s", &ind);
      printf("Type Text:\n");
      scanf("%s", &text);
      strcpy(cmd, op); strcat(cmd, ind); strcat(cmd, text);
      write(sockfd, &cmd, sizeof(cmd));
    } else {
      if (strcmp(op, "get_line") == 0) {
        printf("Type Line: ");
        scanf("%s", &ind);
        strcpy(cmd, op); strcat(cmd, ind);
        write(sockfd, &cmd, sizeof(cmd));
        read(sockfd, &text, sizeof(text));
        printf("%s\n", &text);
      } else {
        printf("Invalid Operation\n");
        break;
      }
    }  
  }
  close(sockfd);
	exit(0);
}

