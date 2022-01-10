#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUF_SIZE 1024
#define TOKEN_SIZE 256
#define PIPENAME "./named_pipe_file"

void error_handling(char *message);
int main(int argc, char *argv[])
{
    int sock, fd;
    int nread, rc;
    char buf[BUF_SIZE];
    char token[TOKEN_SIZE];
    struct sockaddr_in serv_adr;

    if(argc!=3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock=socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));
    
    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("connect() error!");

    memset(buf, 0x00, sizeof(buf));
    read(sock, buf, BUF_SIZE);
    printf("%s", buf);
    
    memset(buf, 0x00, sizeof(buf));
    read(sock, buf, BUF_SIZE);
    printf("%s", buf);

    memset(buf, 0x00, sizeof(buf));
    read(0, buf, BUF_SIZE);
    write(sock, buf, strlen(buf));
    //2015311711
    memset(buf, 0x00, sizeof(buf));
    read(sock, buf, BUF_SIZE);
    printf("%s", buf);

    memset(buf, 0x00, sizeof(buf));
    read(sock, buf, BUF_SIZE);
    printf("%s", buf);

    memset(buf, 0x00, sizeof(buf));
    read(0, buf, BUF_SIZE);
    write(sock, buf, strlen(buf));
    //IP
    memset(buf, 0x00, sizeof(buf));
    read(sock, buf, BUF_SIZE);
    printf("%s", buf);

    memset(buf, 0x00, sizeof(buf));
    read(sock, buf, BUF_SIZE);
    printf("%s", buf);

    memset(buf, 0x00, sizeof(buf));
    read(0, buf, BUF_SIZE);
    write(sock, buf, strlen(buf));
    //PORT
    memset(buf, 0x00, sizeof(buf));
    read(sock, buf, BUF_SIZE);
    printf("%s", buf);

    memset(buf, 0x00, sizeof(buf));
    read(sock, buf, BUF_SIZE);
    printf("%s", buf);

    memset(buf, 0x00, sizeof(buf));
    read(0, buf, BUF_SIZE);
    write(sock, buf, strlen(buf));
    //Y
    memset(buf, 0x00, sizeof(buf));
    read(sock, buf, BUF_SIZE);
    printf("%s", buf);

    memset(buf, 0x00, sizeof(buf));
    read(sock, buf, BUF_SIZE);
    printf("%s", buf);

    memset(buf, 0x00, sizeof(buf));
    read(0, buf, BUF_SIZE);
    write(sock, buf, strlen(buf));
    // Y
    memset(buf, 0x00, sizeof(buf));
    read(sock, buf, BUF_SIZE);
    printf("%s", buf);

    memset(buf, 0x00, sizeof(buf));
    read(sock, buf, BUF_SIZE);
    printf("%s", buf);

    memset(buf, 0x00, sizeof(buf));
    read(0, buf, BUF_SIZE);
    write(sock, buf, strlen(buf));
    // OK

    if(access(PIPENAME,F_OK) == 0){
        unlink(PIPENAME);
    }

    if((rc = mkfifo(PIPENAME, 0666)) < 0){
        error_handling("failed to make named pipe");
    }

    if((fd = open(PIPENAME, O_RDWR)) < 0){
        error_handling("failed to open named pipe");
    }

    if((nread = read(fd, token, sizeof(token))) < 0){
        error_handling("failed to call read()");
    }

    write(sock, token, strlen(token));
    memset(buf, 0x00, sizeof(buf));
    read(sock, buf, BUF_SIZE);
    printf("%s", buf);

    memset(buf, 0x00, sizeof(buf));
    read(sock, buf, BUF_SIZE);
    printf("%s", buf);
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}