#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char* message);

int main(int argc, char* argv[])
{
	int serv_sock;	// 서버소켓의 파일디스크립터 저장
	int clnt_sock;	// 클라이언트 소켓의 파일디스크립터 저장

	struct sockaddr_in serv_addr;	// 서버의 주소정보 저장
	struct sockaddr_in clnt_addr;	// 클라이언트의 주소정보 저장
	socklen_t clnt_addr_size;	// 클라이언트 주소 사이즈

	char message[] = "Lee Dong Hun!";

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));	// 구조체멤버 0으로 초기화
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	// 바인딩하기 위해 형변환
		error_handling("bind() error");

	if(listen(serv_sock, 5)==-1)	// 연결요청대기큐(5)
		error_handling("listen() error");

	clnt_addr_size=sizeof(clnt_addr);
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if(clnt_sock==-1)
		error_handling("accept() error");

	write(clnt_sock, message, sizeof(message));
	close(clnt_sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
