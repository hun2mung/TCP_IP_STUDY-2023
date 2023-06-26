#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

char webpage[] = "HTTP/1.1 200 OK\r\n"
                 "Server: Linux Web Server\r\n"
                 "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                 "<!DOCTYPE html>\r\n"
                 "<html><head><title>My Web Page</title>\r\n"
                 "<style>body {background-color: #FFFF00 }</style></head>\r\n"
                 "<body><center><h1>Hello world!!</h1><br>\r\n"
                 "<img src=\"a.jpg\"></center></body></html>\r\n";

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    int str_len;
    char buf[BUF_SIZE];
    FILE *fp;
    long file_size, read_len;
    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);

    while (1)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
        if (clnt_sock == -1)
            error_handling("accept() error");

        // Check if the requested file is "a.jpg"
        if (strstr(webpage, "a.jpg") != NULL) {
            fp = fopen("a.jpg", "rb");
            if (fp == NULL)
                error_handling("fopen() error");

            fseek(fp, 0, SEEK_END);
            file_size = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            sprintf(buf, "HTTP/1.1 200 OK\r\n"
                         "Server: Linux Web Server\r\n"
                         "Content-Type: multipart/x-mixed-replace; boundary=end\r\n\r\n");
            write(clnt_sock, buf, strlen(buf));

            // Send the webpage
            write(clnt_sock, webpage, sizeof(webpage) - 1);

            // Send the image data
            sprintf(buf, "\r\n--end\r\n"
                         "Content-Type: image/jpeg\r\n"
                         "Content-Length: %ld\r\n\r\n", file_size);
            write(clnt_sock, buf, strlen(buf));

            while ((read_len = fread(buf, 1, BUF_SIZE, fp)) > 0)
                write(clnt_sock, buf, read_len);

            fclose(fp);
        } else {
            // Send the webpage
            write(clnt_sock, webpage, sizeof(webpage) - 1);
        }

        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
