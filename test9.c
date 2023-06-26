#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void* request_handler(void* arg);
void send_response(int clnt_sock, char* response);
void send_error(FILE* fp);
void error_handling(char* message);

char webpage[] = "HTTP/1.1 200 OK\r\n"
                 "Server: Linux Web Server\r\n"
                 "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                 "<!DOCTYPE html>\r\n"
                 "<html><head><title> My Web Page </title>\r\n"
                 "<style>body {background-color: #FFFF00 }</style></head>\r\n"
                 "<body><center><h1>Hello world!!</h1><br>\r\n"
                 "<img src=\"a.jpg\"></center></body></html>\r\n";

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int clnt_adr_size;
    pthread_t t_id;
    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 20) == -1)
        error_handling("listen() error");

    while (1) {
        clnt_adr_size = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_size);
        printf("Connection Request : %s:%d\n",
            inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));
        pthread_create(&t_id, NULL, request_handler, &clnt_sock);
        pthread_detach(t_id);
    }
    close(serv_sock);
    return 0;
}

void* request_handler(void* arg)
{
    int clnt_sock = *((int*)arg);
    char req_line[SMALL_BUF];
    FILE* clnt_read;
    FILE* clnt_write;

    clnt_read = fdopen(clnt_sock, "r");
    clnt_write = fdopen(dup(clnt_sock), "w");
    fgets(req_line, SMALL_BUF, clnt_read);
    if (strstr(req_line, "HTTP/") == NULL) {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }

    // 클라이언트에게 배열 전송
    send_response(clnt_sock, webpage);

    fclose(clnt_read);
    fclose(clnt_write);
    return NULL;
}

void send_response(int clnt_sock, char* response)
{
    write(clnt_sock, response, strlen(response));
}

void send_error(FILE* fp)
{
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server: Linux Web Server\r\n";
    char content_type[] = "Content-Type: text/html; charset=UTF-8\r\n\r\n";
    char content[] = "<!DOCTYPE html>\r\n"
                     "<html><head><title>Web Server Error</title></head>\r\n"
                     "<body><p>Error occurred.</p></body></html>\r\n";
    
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(content_type, fp);
    fputs(content, fp);
    fflush(fp);
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
