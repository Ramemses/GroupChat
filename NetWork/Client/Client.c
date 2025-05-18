#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "../../Utils/Utils.h"
#include "../../Utils/Help.h"
#include "../../Graphics/ChatBox.h"


void make_client(int fd);
void* get_message(void* socketPT);
void send_messages(int socketFD, char* name);





	
int main() {

	char ip[MAX_IPv4_LENGTH];
	int port;
	get_connection_address(ip, &port);
	

    int socketFD = createTCPIpv4Socket();
    struct sockaddr_in *address = createIPv4Address(ip, port);

	


    int result = connect(socketFD, (struct sockaddr*)address, sizeof (*address));

    if(result == 0)
        printf("connection was successful\n");


    char name[MAX_NAME_LENGTH];
	get_name(name);


    make_client(socketFD);
	init_interface();
    send_messages(socketFD, name);

	
	close_chat();
    close(socketFD);

    return 0;
}

void send_messages(int socketFD, char* name) {

#if 0
    char *line = NULL;
    size_t lineSize= 0;
    printf("type and we will send(type exit to leave the chat)...\n");

    char buffer[1024];
#endif

    char buffer[1024];
	int length = 0;

    while(true)
    {
		memset(buffer, 0, sizeof(buffer));
		length = 0;
#if 0
		line = NULL;
        ssize_t charCount = getline(&line,&lineSize,stdin);
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
#endif 
		int status = read_input(buffer, &length);

		if(status != 1 )
		{
			printf("\n");
			break; 
		}
        buffer[length]= '\0';

		if (length > 0){
			char* self_string = calloc(sizeof(char), length + 10);
			strcat(self_string, "You: ");
			strcat(self_string, buffer);
	        add_message(self_string);
			free(self_string);

			char* send_string = calloc(sizeof(char), length + 10);
			strcat(send_string, name);
			strcat(send_string, ": ");
			strcat(send_string, buffer);
            ssize_t amountWasSent =  send(socketFD,
                                          send_string,
                                          strlen(send_string), 0);
			free(send_string);
		}	

    }
}

void make_client(int socketFD) 
{
    pthread_t id ;
    pthread_create(&id, NULL, get_message, (void*)&socketFD);

}

void* get_message(void* socketPT) {
	int socketFD = *(int*)socketPT;

    char buffer[1024];
    while (true)
    {
        ssize_t  amountReceived = recv(socketFD, buffer, 1024, 0);
        add_message(buffer);

        if(amountReceived>0)
        {
            buffer[amountReceived] = 0;
            add_message(buffer);

//			printf("%s\n ",buffer);
        }

        if(amountReceived==0)
            break;
    }

    close(socketFD);
}
