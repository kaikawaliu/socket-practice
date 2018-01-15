#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int client_sockfd;
    int len;
    struct sockaddr_in remote_addr; // server ip addr
    char buf[BUFSIZ];
    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family=AF_INET;
    remote_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    remote_addr.sin_port=htons(8088);

    if((client_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket error");
        return 1;
    }

    if(connect(client_sockfd,(struct sockaddr *)&remote_addr, sizeof(struct sockaddr))<0)
    {
        perror("connect error");
        return 1;
    }

    printf("connected to server\n");
    len=recv(client_sockfd,buf,BUFSIZ,0); // recv server data
    buf[len]='\0';
    printf("%s",buf);

    while(1)
    {
        printf("Enter string to send:");
        scanf("%s",buf);
        if(!strcmp(buf,"quit"))
            break;
        len=send(client_sockfd,buf,strlen(buf),0);
        len=recv(client_sockfd,buf,BUFSIZ,0);
        buf[len]='\0';
        printf("received:%s\n",buf);
    }

    close(client_sockfd);

    return 0;
}
