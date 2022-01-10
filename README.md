## 1) ipv4_client.c
Ipv4 server에 연결하기 위한 전체적인 흐름은 다음과 같다.

socket() -> connect() -> read()/write() -> close()

socket() 함수를 통해 소켓을 생성했고, 그 후에 sockaddr_in 구조체 변수 serv_adr의 IP, PORT 정보를 초기화했다. 그 다음 connect()함수 호출을 통해 서버로 연결 요청을 했다. 그 후 read를 통해 server로부터 정보를 읽어오고, write를 통해 유저로부터 입력받은 학번, IP, PORT 정보 등의 정보들을 server에 전달했다. 
그 후 ipv6 server로부터 넘어오는 token을 받기 위해, named pipe를 사용했다. Named pipe는 프로세스 간 통신 방법 중 하나로, pipe 개념을 확장한 것으로 볼 수 있다. mkfifo() 함수를 통해 named pipe를 생성하고, open() 함수로 named pipe를 열었다. 그 다음 read()를 통해 token을 읽었다. 
이 토큰을 write()함수를 이용해 ipv4 server로 보냈고, 결과를 read()함수를 통해 전달받았다.

## 2) ipv6_server.c
Ipv6_server 코드의 전체적인 흐름은 다음과 같다. 

socket() -> bind() -> listen() -> accept() -> read() / write() -> close()

socket() 함수 호출을 통해 소켓을 생성하고, 소켓 주소할당을 위해 serv_adr 구조체를 초기화하였다. 그 다음 bind() 함수를 통해 소켓에 주소를 할당했다. 
그 다음 listen() 함수 호출을 통해, 연결요청 대기 상태로 진입했다. 연결 요청 대기 큐의 크기는 3으로 설정했다. 
Concurrent한 Server를 구현하기 위해, 멀티 프로세스 기반의 코드를 작성했다. fork() 함수를 통해 생성된 자식 프로세스가 client로부터 온 token 정보를 read 하도록 했다. 그런데 여러 개의 자식 프로세스가 각자 받은 token 정보를 하나의 문자열로 취합해야 했다. 그래서 부모 자식 프로세스 간 통신에 사용되는 pipe를 사용했다. 각 자식 프로세스는 개별 token을 파이프의 한 종단에 write 한다. 
각 자식 프로세스가 보낸 token 정보를 취합하여, ipv4 클라이언트로 보내는 역할은 또 하나의 fork() 함수 호출로 생성된 자식 프로세스가 담당한다. Pipe의 또 다른 종단에서 반복문을 통해 token 정보를 받아, strcat 함수를 이용해 하나의 문자열로 만든다. 이 문자열을, named pipe를 이용해 ipv4_client에게 전달했다.  
