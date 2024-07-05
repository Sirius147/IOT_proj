#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char* message);

int main(int argc, char* argv[]){

	int sock;
    char message[BUF_SIZE];
  	int str_len, recv_len, recv_cnt;
	struct sockaddr_in serv_adr;

	if(argc!=4){
		printf("Usage: %s <IP> <port> <file name>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM,0);
	if(sock == -1) error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) error_handling("connect() error");
    else printf("Connected........\n");

    FILE* fp = fopen(argv[3],"r");
    if( fp == NULL) {
		printf(" failed to open \'%s\'. ", argv[3]);
		return -1;
	}
    write(sock, argv[3], BUF_SIZE);

    while (fgets(message, BUF_SIZE, fp) != NULL) {
        if (strcmp(message, "EOF\n") == 0)
            break;
        write(sock, message, strlen(message));
    }
    close(sock);
    return 0;
}
void error_handling(char * message){
	
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

