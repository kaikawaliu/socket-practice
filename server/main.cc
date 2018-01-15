#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    int server_sockfd;
    int client_sockfd;
    int len;
    struct sockaddr_in my_addr; // server ip addr
    struct sockaddr_in remote_addr; // client ip addr
    unsigned int sin_size;
    char buf[BUFSIZ];
    memset(&my_addr, 0, sizeof(my_addr)); // data init, set to zero
    my_addr.sin_family = AF_INET; // ip communication
    my_addr.sin_addr.s_addr = INADDR_ANY; // server IP addr -- allow any local addr
    my_addr.sin_port = htons(8099); // server port

    /*create server socket --IPv4 protocol, connection facing, TCP*/
    if((server_sockfd = socket(PF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket error");
        return 1;
    }

    /*binding socket to the server addr*/
    if(bind(server_sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))<0)
    {
        perror("bind error");
        return 1;
    }

    /*listen connection request-- size of queue is 5*/
    if(listen(server_sockfd,5)<0)
    {
        perror("listen error");
        return 1;
    }

    sin_size = sizeof(struct sockaddr_in);

    /*waiting client request arrival*/
    if((client_sockfd=accept(server_sockfd, (struct sockaddr *)&remote_addr, &sin_size))<0)
    {
        perror("accept error");
        return 1;
    }

    printf("accept client %s\n", inet_ntoa(remote_addr.sin_addr));
    len = send(client_sockfd, "Welcome to my server\n", 21, 0);

    /* recv client data and send to client--recv return bytes received, send return bytes sent*/
    while((len=recv(client_sockfd,buf,BUFSIZ,0))>0)
    {
        buf[len]='\0';
        printf("%s\n",buf);
        if(send(client_sockfd,buf,strlen(buf),0)<0)
        {
            perror("write error");
            return 1;
        }
    }

    close(client_sockfd);
    close(server_sockfd);

    return 0;
}
