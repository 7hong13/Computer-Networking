#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXDATASIZE 1000

int main(int argc, char *argv[]) {
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo;
    int rv;
    char s[INET_ADDRSTRLEN];

    fd_set master, read_fds;

    if (argc != 4) {
        fprintf(stderr, "usage: client host name portnum id\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo) != 0)) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype,
        servinfo->ai_protocol)) == -1) {
            perror("client: socket");
            return 2;
    }

    if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        close(sockfd);
        perror("client: connect");
        return 2;
    }    

    inet_ntop(servinfo->ai_family, &((struct sockaddr_in*)servinfo->ai_addr)->sin_addr, s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo);

    FD_ZERO(&master);
    FD_SET(0, &master);
    FD_SET(sockfd, &master);

    int quit = 0;
    while (1) {
        read_fds = master;
        if (select(sockfd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(1);
        }

        for (int i = 0; i <= sockfd + 1; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == 0) {
                    fgets(buf, MAXDATASIZE, stdin);
                    if (strcmp(buf, "quit\n") == 0) {
                        quit = 1;
                        break;
                    }
                    char fullStr[MAXDATASIZE];
                    strcpy(fullStr, argv[3]);
                    strcat(fullStr, ": ");
                    strcat(fullStr, buf);
                    if (send(sockfd, fullStr, strlen(fullStr), 0) == -1) {
                        perror("send");
                        close(sockfd);
                        exit(1);
                    }
                }
                else if (i == sockfd) {
                    //message received from server
                    if ((numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1) {
                        perror("recv");
                        close(sockfd);
                        exit(1);
                    }
                    if (numbytes == 0) {
                        printf("server disconnected.\n");
                        quit = 1;
                        break;
                    }
                     
                    buf[numbytes] = '\0';
                    printf("%s\n", buf);
                }
            }
        }
        if (quit) break;
    }

    printf("connectino closed.\n");
    close(sockfd);

    return 0;
}