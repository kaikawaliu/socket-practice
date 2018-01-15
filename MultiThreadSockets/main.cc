#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <thread>

void *client_func(void *arg)
{
    int recv_len = 0;
    char recv_buf[BUFSIZ] = "";
    int connfd = *(int *)arg;
    char welcome_msg[BUFSIZ] = "Welcome to my multi-thread server\n";
    recv_len = send(connfd, welcome_msg, strlen(welcome_msg),0);

    while((recv_len = recv(connfd, recv_buf, sizeof(recv_buf),0))>0)
    {
        printf("recv_buf: %s\n", recv_buf);
        send(connfd, recv_buf, recv_len, 0);
    }

    printf("client closed!\n");
    close(connfd);

    return NULL;
}

int main(int argc, char *argv[])
{
    int sockfd = 0;
    int connfd = 0;
    int err_log = 0;
    struct sockaddr_in my_addr;
    unsigned short port = 8088;
    pthread_t thread_id;

    printf("TCP Server Started at port %d!\n", port);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket error");
        exit(-1);
    }

    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    printf("Binding server to port %d/n", port);

    err_log = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(err_log < 0)
    {
        perror("bind error");
        //close(sockfd);
        exit(-1);
    }

    if(listen(sockfd,5)<0)
    {
        perror("listen error");
    }

    printf("Waiting client...\n");

    while(1)
    {
        char cli_ip[INET_ADDRSTRLEN] = "";
        struct sockaddr_in client_addr;
        socklen_t cliaddr_len = sizeof(client_addr);

        connfd = accept(sockfd, (struct sockaddr*)&client_addr, &cliaddr_len);
        if(connfd < 0)
        {
            perror("accept this time");
            // continue;
            return 1;
        }

        inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
        printf("-----------------------------------------\n");
        printf("client_ip=%s, port=%d\n", cli_ip, ntohs(client_addr.sin_port));
        if(connfd > 0)
        {
            pthread_create(&thread_id, NULL, client_func, (void *)&connfd);
            pthread_detach(thread_id);
        }
    }

    close(sockfd);

    return 0;
}
