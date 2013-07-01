#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>


#include "common.h"

const int EXIT_FAILURE=1;

void check_errno(int retcode)
{
    if(retcode == -1)
    {
        int err=errno;
        perror(err);
        exit(EXIT_FAILURE);
    }
}

int get_unix_socket()
{
    int sockfd;
    struct sockaddr_un sock_un;
    
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    check_errno(sockfd);
    
    memset(&sock_un, 0, sizeof sock_un);
    sock_un.sun_family = AF_UNIX;
    
    strncpy(sock_un.sun_path, SOCKET_NAME, sizeof sock_un.sun_path-1)
            [sizeof sock_un.sun_path-1] = 0;
    
    int len = SUN_LEN(&sock_un);
    sock_un.sun_path[0]=0;
    
    check_errno(bind(sockfd, (struct sockaddr *)&sock_un, len));
    
    return sockfd;
}

int main(int argc, char** argv)
{
    int sockfd = get_unix_socket();
    system("netstat -pa --unix|grep qweasd");
    close(sockfd);
    return 0;
}
