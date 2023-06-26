#include <stdio.h>	// 표준 입출력 헤더
#include <stdlib.h>	// 유틸리티 함수 모아놓은 헤더
#include <unistd.h>	// 리눅스에서 사용하는 c 컴파일러 헤더
#include <string.h>	// C형식 문자열 다루는 함수 헤더
#include <arpa/inet.h>	// 주소변환 기능 사용 헤더
#include <sys/socket.h>	// 소켓 관련 핵심 기능 헤더
#include <fcntl.h>	// 이미지 파일 헤더

#define BUF_SIZE 1024
#define MAX_BUFFER_SIZE 1024

char webpage[] = "HTTP/1.1 200 OK\r\n"
								"Server:Linux Web Server\r\n"
								"Content-Type: text/html; charset=UTF-8\r\n\r\n"
								"<!DOCTYPE html>\r\n"
								"<html><head><title> My Web Page </title>\r\n"
								"<style>body {background-color: #FFFF00 }</style></head>\r\n"
								"<body><center><h1>Hello world!</h1><br>\r\n"
								"<img src=\"test.jpg\"></center></body></html>\r\n";


void error_handling(char *message);

void handle_client_request(int client_socket);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock; // 서버 소켓, 클라이언트 소켓
	struct sockaddr_in serv_addr, clnt_addr;	// 서버, 클라이언트 구조체 -> IP, 통신 프로토콜 초기화 및 확인 위해서
	int clnt_addr_size;	// accept 할 때 클라이언트에서 받는 구조체 크기?
	char buf[BUF_SIZE];
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);	// 소켓 생성
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));	// 서버 구조체 초기화(서버 주소)
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)	// 바인딩(주소 할당)
		error_handling("bind() error");

	if(listen(serv_sock, 5) == -1)	// 리슨 (연결대기큐)
		error_handling("listen() error");

	clnt_addr_size=sizeof(clnt_addr);	// 클라이언트 주소 사이즈
	//  엑셉트(클라이언트 통신 요청 수락. 주소 받아옴)
	while(1)
	{
		clnt_sock=accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
		if(clnt_sock==-1)
			error_handling("accept() error");

		read(clnt_sock, buf, BUF_SIZE);	// 클라이언트가 보낸 값 읽기
		printf("Connect Client IP: %s\n", inet_ntoa(clnt_addr.sin_addr));	// 클라이언트 주소 확인
		printf(buf); // 클라이언트에서 보낸 요청 내용 확인

		// Handle client request
    handle_client_request(clnt_sock);

/*		if(send(clnt_sock, web, strlen(web), 0) == -1)
		{
			perror("error");
			exit(1);
		}*/
		
/*	  FILE* fp = fopen("test.jpg", "r");
    	
		if(fp == NULL)
			error_handling("img error");
			
		char img_buf[BUF_SIZE];
		ssize_t bytes_read;
		while((bytes_read = fread(img_buf, 1, sizeof(img_buf), fp)) >0)
		{
			if(send(clnt_sock, img_buf, bytes_read, 0) == -1){
				perror("Error");
				exit(1);
			}
		}

		fclose(fp);*/
	}
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


void handle_client_request(int clnt_sock) {

		 char http_response[] = "HTTP/1.1 200 OK\r\n"
                           "Server: Linux Web Server\r\n"
                           "Content-Type: text/html\r\n\r\n"
                           "<!DOCTYPE html>\r\n"
                           "<html><head><title>My Web Page</title>\r\n"
                           "<style>body {background-color: #FFFF00 }</style></head>\r\n"
                           "<body><center><h1>Hello world!!</h1><br>\r\n"
                           "<img src=\"a.jpg\"></center></body></html>\r\n";
		
    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_read;
    FILE *image_file = fopen("test.jpg", "rb");

    if (image_file == NULL) {
        perror("Error opening image file");
        exit(1);
    }

    if (send(clnt_sock, http_response, strlen(http_response), 0) == -1) {
        perror("Error sending HTTP response headers");
        exit(1);
    }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), image_file)) > 0) {
        if (send(clnt_sock, buffer, bytes_read, 0) == -1) {
            perror("Error sending image data");
            exit(1);
        }
    }

    fclose(image_file);
}
