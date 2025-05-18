#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "../../Utils/Utils.h"
#include "../../Utils/Help.h"
#include "../../Graphics/ChatBox.h"


void make_new_client(int fd);
void* get_message(void* socketPT);
void send_messages(int socketFD, char* name);


	
int main() {

//  Get server ip_addres and port to connect
	char ip[MAX_IPv4_LENGTH];
	int port;
	get_connection_address(ip, &port);
	


// Create client socket and connect to the server
    int socketFD = createTCPIpv4Socket();
    struct sockaddr_in *address = createIPv4Address(ip, port);

    int result = connect(socketFD, (struct sockaddr*)address, sizeof (*address));
    if(result == 0)
        printf("connection was successful\n");
	
// Make a new thread for a client
    make_new_client(socketFD);

// Set the client name
    char name[MAX_NAME_LENGTH];
	get_name(name);


// MAKE GUI HERE AFTER CHECKING THE CONNECTION RESULTS AND THREAD CREATING RESULTS
	
	
	

// Start a loop to send messages to server
    send_messages(socketFD, name);

    close(socketFD);

    return 0;
}

void send_messages(int socketFD, char* name) {


    char *line = NULL;
    size_t lineSize= 0;
    printf("type and we will send(type exit to leave the chat)...\n"); //Replace this information to the info-block

    char buffer[1024];

    while(true)
    {
		
		line = NULL;
        ssize_t  charCount = getline(&line,&lineSize,stdin);   //Change here
        line[charCount-1]=0;

        sprintf(buffer,"%s:%s",name , line); 
        if(charCount>0)
        {
// Conditional to leave the chat
            if(strcmp(line,"/exit")==0)
                break;
// send message to the server
            ssize_t amountWasSent =  send(socketFD,
                                          buffer,
                                          strlen(buffer), 0);
        }
    }
}


void make_new_client(int socketFD) 
{
    pthread_t id ;
    pthread_create(&id, NULL, get_message, (void*)&socketFD);

}

// Recieve messages in a loop from server broadcast
void* get_message(void* socketPT) {
	int socketFD = *(int*)socketPT;
    char buffer[1024];

    while (true)
    {
        ssize_t  amountReceived = recv(socketFD,buffer,1024,0);

// Display recieved messages at console
        if(amountReceived>0)
        {
            buffer[amountReceived] = 0;
            printf("%s\n ",buffer); // Replace console output to the gui output 
        }

        if(amountReceived==0)
            break;
    }

    close(socketFD);
}
