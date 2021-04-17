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
#include <ctype.h>

#define MAX_DATA_SIZE 10000
#define MAX_PORT_LEN 20
#define DEFAULT_PORT 80

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: http_client http://hostname[:port][/path/to/file]\n");
        exit(1);
    }
    
    int port = DEFAULT_PORT;
    char port_str[MAX_PORT_LEN]; //getaddrinfo에서 요구되는 data type
    char hostname[MAX_DATA_SIZE] = "";
    char path[MAX_DATA_SIZE] = "/";
    char http_parsing[8]; //http 파싱 위한 스트링
    char addr_parsing[MAX_DATA_SIZE]; //hostname, port, path 파싱 위한 스트링
    char* ptr_parsing; //주소 및 content_length 파싱용 포인터
    int addr_len = strlen(argv[1]) - 7; //http:// 제외한주소 길이
    int content_length = -1;
    char header_checker[5]; //header 끝나는 지점 확인 위한 스트링
    char status[MAX_DATA_SIZE] = "";

    int sockfd, numbytes = 1;
    struct addrinfo hints, *servinfo;
    char buf[MAX_DATA_SIZE];
    char read_buf[MAX_DATA_SIZE]; //strtok 사용시 원본이 수정되므로 read 전용 따로 선언
    int rv;
    int curr_processed = 0; //실시간 파일 출력된 데이터 크기

    FILE *fp;

    //http://로 시작하는지 확인
    memcpy(http_parsing, &argv[1][0], 7);
    http_parsing[7] = '\0';
    if (strcmp(http_parsing, "http://") != 0) {
        fprintf(stderr, "usage: http_client http://hostname[:port][/path/to/file]\n");
        exit(1);
    }

    memcpy(addr_parsing, &argv[1][7], strlen(argv[1]) - 7);
    ptr_parsing = strstr(addr_parsing, ":");
    if (ptr_parsing == NULL) {
        ptr_parsing = strstr(addr_parsing, "/");
        if (ptr_parsing == NULL) {
            strcpy(hostname, addr_parsing);
        }
        else {
            int path_start = 0;
            for (int idx = 0; addr_parsing[idx] != '/'; idx++) {
                hostname[idx] = addr_parsing[idx];
                path_start = idx;
            }
            path_start++;
            for (int idx = path_start; addr_parsing[idx]; idx++) {
                path[idx - path_start] = addr_parsing[idx];
            }
        }
    }
    else {
        ptr_parsing = strtok(addr_parsing, ":");
        strcpy(hostname, ptr_parsing);
        ptr_parsing = strtok(NULL, ":");
        char* path_ptr;
        port = strtol(ptr_parsing, &path_ptr, 10);
        if (strlen(path_ptr) > 0) strcpy(path, path_ptr); //path가 명시된 경우
    }
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    sprintf(port_str, "%d", port); //port 스트링으로 저장
    if ((rv = getaddrinfo(hostname, port_str, &hints, &servinfo) != 0)) {
        fprintf(stderr, "getaddrinfo: Name or service not known\n");
        return 1;
    }

    if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype,
        servinfo->ai_protocol)) == -1) {
            perror("client: socket");
            return 2;
    }

    if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        close(sockfd);
        perror("connect");
        exit(1);
    }

    //request message
    strcpy(buf, "GET ");
    strcat(buf, path);
    strcat(buf, " HTTP/1.1\r\nHost: ");
    strcat(buf, hostname);
    strcat(buf, ":");
    strcat(buf, port_str);
    strcat(buf, "\r\n\r\n");

    if (send(sockfd, buf, strlen(buf), 0) == -1) {
        perror("send");
        close(sockfd);
        exit(1);
    }

    fp = fopen("20170172.out", "w");
    bool status_processed = false;
    bool header_processed = false;
    while (!header_processed) {
        numbytes = recv(sockfd, buf, MAX_DATA_SIZE - 1, 0);
        buf[numbytes] = '\0';
        strcpy(read_buf, buf);
        if (!status_processed) {
            int len = strlen(read_buf);
            ptr_parsing = strtok(read_buf, "\r");
            strcat(status, ptr_parsing);
            if (strlen(ptr_parsing) == len) continue; //status code가 다 안 받아짐
            printf("%s\n", status);
            status_processed = true;
        }
        int data_start = 0;
        for (int idx = 0; buf[idx]; idx++) {
            memcpy(header_checker, &buf[idx], 4);
            header_checker[4] = '\0';
            buf[idx] = tolower(buf[idx]);
            if (strcmp(header_checker, "\r\n\r\n") == 0) {
                header_processed = true;
                data_start += idx + 4; //data 시작 지점
                break;
            }
        }
        ptr_parsing = strstr(buf, "content-length");
        if (ptr_parsing != NULL) {
            ptr_parsing += 16;
            content_length = strtol(ptr_parsing, NULL, 10); //int로 변환
        }
        if (header_processed) {
            if (content_length == -1) {
                fprintf(stderr, "Content-Length not specified.\n");
                return 1;
            }
            for (int idx = data_start; buf[idx]; idx++) {
                curr_processed++;
                fputc(buf[idx], fp);
            }
        }
    }

    while (curr_processed < content_length) {
        numbytes = recv(sockfd, buf, MAX_DATA_SIZE - 1, 0);
        curr_processed += numbytes;
        buf[numbytes] = '\0';
        for (int idx = 0; buf[idx]; idx++) {
            fputc(buf[idx], fp);
        }
    }

    close(sockfd);

    printf("%d bytes written to 20170172.out\n", content_length);
    fclose(fp);
    return 0;
}
                      