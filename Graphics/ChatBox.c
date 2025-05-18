#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#define MSG_CNT 16
#define MSG_LENGTH 256

static WINDOW* info_win;
static WINDOW* msg_win;
static WINDOW* input_win;

char messages[MSG_CNT][MSG_LENGTH];

static void init_info()
{
	info_win = newwin(5, 65, 0, 10);
	box(info_win, 0, 0);
	wrefresh(info_win);
}

static void init_msg()
{
	msg_win = newwin(20, 65, 5, 10);
	box(msg_win, 0 ,0);
	wrefresh(msg_win);
}

static void init_input()
{
	input_win = newwin(4 ,65 ,25 ,10);
	box(input_win , 0 ,0);
	wmove(input_win , 1 ,1);
	wrefresh(input_win);
}

void init_interface()
{
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	noecho();

	init_info();
	init_msg();
	init_input();

	keypad(input_win ,TRUE);
}

void update_message()
{
	wclear(msg_win);
	box(msg_win ,0 ,0);

	for (int i=0; i<MSG_CNT; i++)
	{
		mvwprintw(msg_win ,i+1 ,1 ,"%s", messages[i]);
	}
	wrefresh(msg_win );
	wmove(input_win , 1 ,1);
	wrefresh(input_win);
}

void add_message(const char* msg)
{
	for (int i=1; i<MSG_CNT; i++)
	{
		memcpy(messages[i-1], messages[i], MSG_LENGTH);
	}
	strncpy(messages[MSG_CNT -1], msg ,MSG_LENGTH -1 );
	messages[MSG_CNT -1][MSG_LENGTH -1] = '\0';
	update_message();
}

int read_input(char *buffer ,int *size)
{
	int symbol;
	while ((symbol=wgetch(input_win)) != ERR)
	{
		if (symbol == '\n')
		{
			if (strncmp(buffer, "/exit", 5) == 0){
				return 2;
			}
			
			mvwprintw(input_win ,1 ,1 ,"%*s" ,*size ,"");
			wrefresh(input_win );
			return 1;
		}
		else if (symbol == KEY_BACKSPACE || symbol ==127)
		{
			if (*size >0)
			{
				(*size)--;
				mvwprintw(input_win ,1 ,*size +1 ," ");
				wmove(input_win ,1 ,*size +1 );
				wrefresh(input_win );
				buffer[*size] = '\0';
			}
		}
		else if (*size < MSG_LENGTH -1)
		{
			buffer[*size] = (char)symbol;
			(*size)++;
			mvwaddch(input_win ,1 ,*size ,"");
			mvwaddch(input_win ,1 ,*size ,symbol );
			wrefresh(input_win );
		}
	}
	return 0;
}

void close_chat()
{
	endwin();
}


#if 0
int main(void)
{
	init_interface();

	char input[MSG_LENGTH];
	int length=0;

	while(1)
	{
		int status=read_input(input,&length);

		if(status != 1 )
		{
			printf("\n");
			break; 
		}
		
        input[length]= '\0';

        add_message( input );

        memset( input ,0,sizeof( input ));
        length=0;
		
   }

	endwin();

	return 0;
}
#endif
