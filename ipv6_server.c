#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

#define BUF_SIZE 256
#define PIPENAME "./named_pipe_file"

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in6 serv_adr, clnt_adr;
    
    pid_t pid;
    socklen_t adr_sz;
    int str_len;
    int i, fd, nread;
    char buf[BUF_SIZE];
    char result[BUF_SIZE];
    int fds[2];
    if(argc!=2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    serv_sock=socket(AF_INET6, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin6_family=AF_INET6;
    inet_pton(AF_INET6, "2001:0:c38c:c38c:428:3bc9:2d9a:b473", &serv_adr.sin6_addr);
    serv_adr.sin6_port=htons(atoi(argv[1]));
    serv_adr.sin6_flowinfo = 0;

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error");
    if(listen(serv_sock, 3)==-1)
        error_handling("listen() error");

    pipe(fds);
    pid=fork();
    if(pid==0)
    {
        char *s2 = malloc(sizeof(char)*30);
        read(fds[0], result, BUF_SIZE);
        result[strlen(result)-2] = '\0';
        for(i=1; i<5; i++){
            read(fds[0], s2, 30);
            s2[strlen(s2)-2] = '\0';
            strcat(result, ",");
            strcat(result, s2);
        }
        strcat(result, "\n");
        close(serv_sock);
        if((fd = open(PIPENAME, O_WRONLY)) < 0){
            error_handling("named pipe error");
        }
        if((nread = write(fd, result, sizeof(result))) < 0)
        {
            error_handling("named pipe write error");
        }
        return 0;
    }
    
    int cnt = 0;
    while(1)
    {
        adr_sz=sizeof(clnt_adr);
        clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if(clnt_sock==-1)
            continue;
        pid=fork();
        if(pid==-1)
        {
            close(clnt_sock);
            continue;
        }
        if(pid==0)
        {
            close(serv_sock);
            while((str_len=read(clnt_sock, buf, BUF_SIZE))!=0){
                write(fds[1], buf, str_len);
            }
            close(clnt_sock);
            return 0;   
        }
        else    
            close(clnt_sock);
        cnt++;
    }
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

