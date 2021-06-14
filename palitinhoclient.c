#include<stdio.h>
#include<winsock2.h>
#include<string.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#define MESSAGESIZE 255

void receber(SOCKET s){
    int recv_size;
    char server_reply[MESSAGESIZE];
    while(1){
          /**Recebe a resposta do servidor*/
          if((recv_size = recv(s , server_reply , MESSAGESIZE , 0)) == SOCKET_ERROR) {
            puts("\nRecep��o falhou.");
          }
          if(recv_size>0){
              printf("\nServer: ");

              /**Adicionar o \0 no final antes de imprimir*/
              server_reply[recv_size] = '\0';
              puts(server_reply);
          }
    }

}

int main(int argc , char *argv[]) {
  WSADATA wsa;
  SOCKET s;
  struct sockaddr_in server;
  char message[MESSAGESIZE];
  int recv_size;

  printf("\nInicializando Winsock...");
  if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
    printf("\nFalha na inicializacao da biblioteca Winsock: %d",WSAGetLastError());
    exit(EXIT_FAILURE);
  }
  printf("Inicializado.\n");

  printf("\nInicializando Socket...");
  if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
    printf("\nNao e possivel criar o socket: %d" , WSAGetLastError());
    exit(EXIT_FAILURE);
  }
  printf("\nSocket criado.");
  _beginthread(receber,NULL,s);

  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons( 8888 );

  if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0) {
    puts("\nErro de conex�o.");
    exit(EXIT_FAILURE);
  }
  puts("\nConectado");

  printf("Mensagem: ");
  gets(message);

  system("pause");
  return 0;
}
