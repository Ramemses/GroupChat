#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "../../Utils/Utils.h"
#include "../../Utils/Help.h"
#include "../../Graphics/ChatBox.h"



pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

struct AcceptedSocket
{
    int acceptedSocketFD;
    struct sockaddr_in address;
    int error;
    bool acceptedSuccessfully;
};

struct AcceptedSocket * accept_connection(int serverSocketFD);

void get_data(int socketFD);

void launch_server(int serverSocketFD);

void get_new_client(struct AcceptedSocket *pSocket);

void broadcast_data(char *buffer,int socketFD);

struct AcceptedSocket acceptedSockets[10] ;
int acceptedSocketsCount = 0;


void launch_server(int serverSocketFD) {

    while(true)
    {
        struct AcceptedSocket* clientSocket  = accept_connection(serverSocketFD); // Add check
        acceptedSockets[acceptedSocketsCount++] = *clientSocket;

        get_new_client(clientSocket);
    }
}



void get_new_client(struct AcceptedSocket *pSocket) {

	pthread_mutex_lock(&client_mutex);
    pthread_t id;
    pthread_create(&id,NULL, get_data ,pSocket->acceptedSocketFD);
	pthread_mutex_unlock(&client_mutex);
}

void get_data(int socketFD) {

    char buffer[1024];
    while (true)
    {
        ssize_t  amountReceived = recv(socketFD,buffer,1024,0);

        if(amountReceived>0)
        {
            buffer[amountReceived] = 0;
            printf("%s\n",buffer);
            broadcast_data(buffer,socketFD);
        }

        if(amountReceived==0)
            break;
    }

    close(socketFD);
}

void broadcast_data(char *buffer,int socketFD) {
	pthread_mutex_lock(&client_mutex);
    for(int i = 0 ; i<acceptedSocketsCount ; i++)
        if(acceptedSockets[i].acceptedSocketFD !=socketFD)
        {
            send(acceptedSockets[i].acceptedSocketFD,buffer, strlen(buffer),0);
        }
	pthread_mutex_unlock(&client_mutex);

}

struct AcceptedSocket * accept_connection(int serverSocketFD) {

    struct sockaddr_in  clientAddress ;
    int clientAddressSize = sizeof (struct sockaddr_in);
    int clientSocketFD = accept(serverSocketFD,&clientAddress,&clientAddressSize);

    struct AcceptedSocket* acceptedSocket = malloc(sizeof (struct AcceptedSocket));
    acceptedSocket->address = clientAddress;
    acceptedSocket->acceptedSocketFD = clientSocketFD;
    acceptedSocket->acceptedSuccessfully = clientSocketFD>0;

    if(!acceptedSocket->acceptedSuccessfully)
        acceptedSocket->error = clientSocketFD;



    return acceptedSocket;
}


int main() {

// create server socketfd
    int serverSocketFD = createTCPIpv4Socket();
	
// get ip_addres and usable port of the server
	char ip[MAX_IPv4_LENGTH];
	int port;
	get_connection_address(ip, &port);

// bind server => connectable server
    struct sockaddr_in *serverAddress = createIPv4Address(ip, port);
    int result = bind(serverSocketFD, serverAddress, sizeof(*serverAddress));
    if(result == 0)
        printf("socket was bound successfully\n");

// start listenig incoming connections
    int listenResult = listen(serverSocketFD,10);

    launch_server(serverSocketFD);

    shutdown(serverSocketFD,SHUT_RDWR);

    return 0;
}
