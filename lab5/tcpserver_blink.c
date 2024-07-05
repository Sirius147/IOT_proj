#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <pigpio.h>

#define PORTNUM 0x9190
#define BUFFSIZE 256
#define pin 18

int main(void){
    struct sockaddr_in serv_addr, cli_addr;
    int serv_fd, cli_fd, clilen;
    char buffer[BUFFSIZE];

    printf("[TCP server for chatting and controlling LED...]\n");

    if(gpioInitialise() == -1)
        return 1;
    
    gpioSetMode(pin, PI_OUTPUT);

    if((serv_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("ERROR OPENING SOCKET");
        exit(1);
    }
    memset((char*)&serv_addr, 0x00, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORTNUM);

    if(bind(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        perror("ERROR on binding");
        exit(1);
    }
    listen(serv_fd, 5);

loop:

    clilen = sizeof(cli_addr);
    if((cli_fd = accept(serv_fd, (struct sockaddr*)&cli_addr, &clilen)) == -1){
        perror("ERROR on accept");
        exit(1);
    }

    write(cli_fd, "Welcome to chat server ..... LED control .. ", BUFFSIZE);

    while(1){
        memset(buffer, 0x00, sizeof(buffer));
        if((read(cli_fd, buffer, BUFFSIZE)) == -1){
            perror("ERROR reading from socket");
            exit(1);
        }
        printf("[Guest] %s\n",buffer);

        if(buffer[0] == 'q'){
            close(cli_fd);
            goto loop;
        }
        else if(buffer[0] == '0')
            gpioWrite(pin, 0);
        else if(buffer[0] == '1')
            gpioWrite(pin, 1);
        else if(buffer[0] == 'b'){
            for(int n = 0; n < (buffer[1]-'0'); n++){
                gpioWrite(pin,1);
                gpioDelay(100000);
                gpioWrite(pin,0);
                gpioDelay(100000);
            }
        }
        else{
            printf("wrong input. Please enter again\n");
        }
        
        memset(buffer, 0x00, sizeof(buffer));
        printf("[Server] enter key(s):");
        fgets(buffer, BUFFSIZE, stdin);
        write(cli_fd, buffer, BUFFSIZE);
        if(buffer[0] == 'q')
            break;
    }
    close(serv_fd);
    gpioTerminate();
    return 0;
}