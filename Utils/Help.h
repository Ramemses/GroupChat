#ifndef HELP_H
#define HELP_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NAME_LENGTH 30
#define MAX_IPv4_LENGTH 50

void get_name(char *name);
void get_connection_address(char *connect_ip, int *connect_port);

#endif 
