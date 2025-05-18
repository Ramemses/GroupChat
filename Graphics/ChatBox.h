#ifndef CHAT_BOX_H
#define CHAT_BOX_H



void init_interface();
void update_message();
void add_message(const char* msg);
int read_input(char *buffer ,int *size);
void close_chat();



#endif
