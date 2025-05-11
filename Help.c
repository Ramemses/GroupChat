#include "Help.h"

void get_name(char *name)
{
    memset(name, 0, MAX_NAME_LENGTH);
    printf("Enter your name: ");
    char * check = fgets(name, MAX_NAME_LENGTH, stdin);
    if (check == NULL || strlen(name) == 1)
    {
    	perror("Not valid name");
		exit(1);
	}
    check = strchr(name, '\n');
    if (check) *check = '\0';
}


void get_connection_address(char *connect_ip, int *connect_port)
{
    char port[10];
    memset(connect_ip, 0, MAX_IPv4_LENGTH);
    puts("Enter ip address to connect: ");
    char * check = fgets(connect_ip, MAX_IPv4_LENGTH, stdin);
    if (check == NULL || strlen(connect_ip) == 1)
    {
    	perror("Not valid ip address");
		exit(1);
    }
    puts("Enter port: ");
    fgets(port, 10, stdin);
    *connect_port = atoi(port); 
    if (*connect_port == 0)
    {
        perror("Incorrect connection port");
     	exit(1);
	}
        check = strchr(connect_ip, '\n');
        if (check) *check = '\0';
}
