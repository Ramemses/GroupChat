#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "../../Utils/Utils.h"
#include "../../Utils/Help.h"

void startListeningAndPrintMessagesOnNewThread(int fd);

void* listenAndPrint(void* socketPT);

void readConsoleEntriesAndSendToServer(int socketFD);





	
int main() {

	char ip[MAX_IPv4_LENGTH];
	int port;
	get_connection_address(ip, &port);
	

    int socketFD = createTCPIpv4Socket();
    struct sockaddr_in *address = createIPv4Address(ip, port);

	


    int result = connect(socketFD, (struct sockaddr*)address, sizeof (*address));

    if(result == 0)
        printf("connection was successful\n");

    startListeningAndPrintMessagesOnNewThread(socketFD);

    readConsoleEntriesAndSendToServer(socketFD);


    close(socketFD);

    return 0;
}

void readConsoleEntriesAndSendToServer(int socketFD) {
    char name[MAX_NAME_LENGTH];
	get_name(name);

    char *line = NULL;
    size_t lineSize= 0;
    printf("type and we will send(type exit to leave the chat)...\n");


    char buffer[1024];

    while(true)
    {
		
		line = NULL;
        ssize_t  charCount = getline(&line,&lineSize,stdin);
        line[charCount-1]=0;

        sprintf(buffer,"%s:%s",name , line);

        if(charCount>0)
        {
            if(strcmp(line,"exit")==0)
                break;

            ssize_t amountWasSent =  send(socketFD,
                                          buffer,
                                          strlen(buffer), 0);
        }
    }
}

void startListeningAndPrintMessagesOnNewThread(int socketFD) 
{
    pthread_t id ;
    pthread_create(&id, NULL, listenAndPrint, (void*)&socketFD);

}

void* listenAndPrint(void* socketPT) {
	int socketFD = *(int*)socketPT;
    char buffer[1024];

    while (true)
    {
        ssize_t  amountReceived = recv(socketFD,buffer,1024,0);

        if(amountReceived>0)
        {
            buffer[amountReceived] = 0;
            printf("%s\n ",buffer);
        }

        if(amountReceived==0)
            break;
    }

    close(socketFD);
}
