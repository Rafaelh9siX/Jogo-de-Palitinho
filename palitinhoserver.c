#include <io.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#pragma comment(lib,"ws2_32.lib") //Biblioteca Winsock
#define PORT 8888
#define MESSAGESIZE 255
#define MAXCONECTIONS 100

SOCKET new_socket[MAXCONECTIONS];

void connection(int nroconnections)
{
    int len;
    char message[255];
    while(1)
    {
        len=recv(new_socket[nroconnections],message,MESSAGESIZE,0);
        if(len>0)
        {
            printf("\nTamanho da mensagem: %d",len);
            printf("\n%s",message);
            strcpy(message,"\nAlo Cliente. Eu recebi sua mensagem. Tchau!\n");
            send(new_socket[nroconnections] , message , strlen(message) , 0);
        }
    }

}

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server , client;
    int c, len, nroconnections=0, resp;
    char message[MESSAGESIZE];
    printf("\nInicializando biblioteca Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("\nFalha na inicializacao da biblioteca Winsock: %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("\nBiblioteca Winsock inicializada.");
    printf("\nInicializando socket...");
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("\nNao e possivel inicializar o socket: %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("\nSocket inicializado.");
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );
    printf("\nConstruindo socket...");
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("\nNao e possivel construir o socket: %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("\nConstrucao realizada.");
    listen(s,3);
    puts("\nAguardando por conexoes de entrada...");
    c = sizeof(struct sockaddr_in);

    while(1){
        new_socket[nroconnections] = accept(s, (struct sockaddr *)&client, &c);
        if (new_socket == INVALID_SOCKET){
            printf("\nConexao n�o aceita. Codigo de erro: %d" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        if(nroconnections==0){
            /**primeira mconexão*/
            resp=atoi(message);
        }
        puts("\nConex�o aceita.");
        printf("\nDados do cliente - IP: %s -  Porta: %d\n",inet_ntoa(client.sin_addr),htons(client.sin_port));
        _beginthread(connection,NULL,nroconnections);
        nroconnections++;
    }
    getchar();

    /**Finalizacao do socket*/
    closesocket(s);
    /**Finaliza��o da biblioteca*/
    WSACleanup();

    return 0;
}
