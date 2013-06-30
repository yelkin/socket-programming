#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <stdlib.h>

#include <unistd.h>
#include <getopt.h>


#define EXIT_FAILURE 1


#define check_errno(res) if((res) == -1) { \
        int err=errno; \
        printf("ERROR: %s\n", strerror(err)); \
        exit(err); \
    }


void show_usage(char *program_name)
{
    fprintf(stderr, "USAGE: %s -h|--host IP_ADDRESS -p|--port PORT\n",
            program_name);
}

int main(int argc, char** argv)
{
    /* Parse command line */
    char* host;
    int port;
    int ret;

    char *short_options = "h:p:";
    static struct option long_options[] = {
        {"host", required_argument, 0, 'h'},
        {"port", required_argument, 0, 'p'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;

    while(1)
    {
        opt = getopt_long(argc, argv, short_options, long_options, &option_index);

        if(opt == -1)
        {
            break;
        }

        switch(opt)
        {
        case 'h':
            host = optarg;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        default:
            show_usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stderr, "Call arguments: host='%s' port='%d'\n", host, port);

    /* Connect to host:port */
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    check_errno(sock);


    struct sockaddr_in adr_inet;
    memset(&adr_inet, 0, sizeof adr_inet);
    adr_inet.sin_family = AF_INET;
    adr_inet.sin_port = htons(port);
    ret = inet_pton(AF_INET, host, &(adr_inet.sin_addr));
    check_errno(ret);

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
        error("setsockopt failed\n");

    if (setsockopt (sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
        error("setsockopt failed\n");

    ret = connect(sock, &adr_inet, sizeof adr_inet);
    if(ret == 0) {
        printf("Port %d is open on host %s\n", port, host);
        close(sock);
    } else if (ret == -1) {
        printf("Port %d is closed on host %s\n", port, host);
    } else {
        printf("Error connecting to %s:%d", host, port);
    }

    return 0;
}
