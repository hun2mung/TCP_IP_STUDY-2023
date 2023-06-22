# TCP_IP_STUDY-2023
TCP/IP 학습

## Day01
- 네트워크 프로그래밍과 소켓의 이해
	- 소켓 : 연결된 네트워크 상 데이터 송수신에 사용할 수 있는 SW 장치 
	- TCP/IP 통신 :  소말리아(소켓, 바인딩, 리슨, 엑셉트)
	- 파일디스크립터 : 시스템으로부터 할당 받은 파일 또는 소켓에 부여된 정수
	  (0 : 표준입력, 1 : 표준출력, 2: 표준에러)
	- _t로 끝나는 자료형 : 시스템(운영체제)에서 정의하는 자료형
	- 윈속(윈도우 소켓)

## Day02
- 프로토콜 : 컴퓨터  상호간의 대화에 필요한 통신규약
- 소켓
	- int socket(int domain, int type, int protocol) : (프로토콜 체계, 데이터 전송 방식, 프로토콜 정보 전달)
	- 소켓 타입 : SOCK_STREAM(TCP방식: 연결지향형), SOCK_DGRAM(UDP방식)
	- TCP : 무손실, UDP : 대용량 전송
- IP주소 : 네트워크ID, 호스트ID(A~E클래스)
	- A클래스 : 0~127
	- B클래스 : 128~191
	- C클래스 : 192~223
- PORT번호: 운영체제의 길
- 주소표현 구조체
- TCP 서버 함수호출
	- socket() : 소켓생성, bind() : 소켓주소할당, listen() : 연결요청대기상태, accept() : 연결허용, read()/write() : 데이터 송수신, close() : 연결종료
- TCP 클라이언트 함수호출
	- socket(), connect() : 연결요청, read()/write(), close()

## Day03
- Iterative server, client
- TCP 내부 동작 : 소켓 연결(Tree-way handshaking), 데이터 송수신, 연결종료(Four-way handshaking) 
	- 데이터 송수신에서 ACK 번호 -> SEQ번호 + 전송된 바이트 크기 +1
- UDP기반 server/client -> TCP와 달리 연결 상태를 유지하지 않으므로 주소정보 요구
- TCP 기반 Half-Close
asdasd
