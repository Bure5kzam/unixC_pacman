#include <ncurses.h>
#include <unistd.h>
#include <locale.h>

char map[10][10] = {
	"##########",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"##########",
}
int main()
{
	setlocale(LC_CTYPE, "ko_KR.utf8");
    initscr(); 
    keypad(stdscr, TRUE);

    while(1) {
	    int ch = getch();
	    if (ch == KEY_UP) {
		    printw("상");
	    } else if (ch == KEY_RIGHT) {
		    printw("우");
	    } else if(ch == KEY_DOWN) {
		    printw("하");
	    } else if(ch == KEY_LEFT) {
		    printw("좌");
	    }
    }
    getch();
    endwin();
    return 0;
}
