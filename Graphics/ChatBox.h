#ifndef CHAT_BOX_H
#define CHAT_BOX_H


#define MSG_CNT 20
#define MSG_LENGTH 256

void init_interface();
void update_message();
void add_message(const char* msg);
int read_input(char *buffer ,int *size);
void close_chat();



#endif
