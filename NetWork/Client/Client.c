#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "../../Utils/Utils.h"
#include "../../Utils/Help.h"
#include "../../Graphics/ChatBox.h"

#define MAX_NAME_LENGTH 30
#define MAX_IPV4_LENGTH 30

int send_message(int socketFD, char* name);
int get_message(int socketFD, fd_set* read_fds);
void messanger(int socketFD, char* name);

void connect_to_server(int clientfd, struct sockaddr_in* server_address)
{
    int length = sizeof(*server_address);
    int status = connect(clientfd, (struct sockaddr*)server_address, (socklen_t)length);

    if (status != 0){
        perror("Connection error...");
        exit(1);
    }else{
        printf("You've been connected to the server\n");
    }
}

int main(void)
{
    // get client name
    char name[MAX_NAME_LENGTH];
    get_name(name);
    
    // get ip_address and port to connect server
    char ip_address[MAX_IPV4_LENGTH];
    int port;
    get_connection_address(ip_address, &port);
    
    // create client socket 
    int clientfd = createTCPIpv4Socket();
    // create struct of server_addr
    struct sockaddr_in* server_addr = createIPv4Address(ip_address, port);

    // wrapper of 'connect' to work with issues
    connect_to_server(clientfd, server_addr);

	init_interface();
    messanger(clientfd, name);

	close_chat();
    return 0;
}

void messanger(int socketFD, char* name)
{
    while(1){
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(socketFD, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        int max_fd = (socketFD > STDIN_FILENO) ? socketFD : STDIN_FILENO;

        if(select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            break;
        }
        
		if(FD_ISSET(STDIN_FILENO, &read_fds)) {
            if(send_message(socketFD, name) == -1) break;
        }

        if(FD_ISSET(socketFD, &read_fds)) {
            if(get_message(socketFD, &read_fds) == -1) break;
        }
    }
    close(socketFD);
}

int send_message(int socketFD, char* name)
{   
    char buffer[1024];
	char send_buffer[1024];
	char self_buffer[1024];

	int length = 0;
    memset(buffer, 0, sizeof(buffer));
   // char *line = NULL;
   // size_t lineSize= 0;
    //
	int result = read_input(buffer, &length);
   // ssize_t charCount = getline(&line, &lineSize, stdin);
    if(length <= 0) {
   //     free(line);
        return -1;
    }
    
    //line[charCount-1] = 0; 
	if(strcmp(buffer, "/exit") == 0) {
        return -1;
    }
    
    snprintf(self_buffer, sizeof(self_buffer), "You: %s", buffer);
	add_message(self_buffer);

    snprintf(send_buffer, sizeof(send_buffer), "%s: %s", name, buffer);
	


//	free(line);
    

    ssize_t totalSent = 0;
    while(totalSent < strlen(send_buffer)) {
        ssize_t amountWasSent = send(socketFD, send_buffer + totalSent, strlen(send_buffer) - totalSent, 0);
        if(amountWasSent <= 0) {
            perror("Send failed");
            return -1;
        }
		
        totalSent += amountWasSent;
    }
    
    return 0;
}

int get_message(int socketFD, fd_set* read_fds) {
    char buffer[1024];
    ssize_t amountReceived = recv(socketFD, buffer, sizeof(buffer)-1, 0);
    
    if(amountReceived > 0) {
        buffer[amountReceived] = 0;
		add_message(buffer);

//        printf("%s\n ", buffer); 
        fflush(stdout);
    }
    else if(amountReceived == 0) {
        printf("\rServer disconnected\n");
        return -1;
    }
    else {
        perror("recv failed");
        return -1;
    }
    
    return 0;
}

