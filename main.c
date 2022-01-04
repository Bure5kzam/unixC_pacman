#include <ncurses.h>
#include <unistd.h>
#include <locale.h>

#define W 10 // 넓이
#define H 10 // 높이

//유저 변수
int pu[2] = {1,1};

//위치 관련 변수
int D[4][2] = {{-1,0}, {0, 1}, {1, 0}, {0, -1}};
int KEY_VALUE[4] = {KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT};
char map[H][W] = {
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
};
void print() {
	clear();
	for ( int y = 0; y < H; y++) {
		for ( int x = 0; x < W; x++) {
			if(y == pu[0] && x == pu[1]) {
				//유저 출력
				printw("@");
			} else {
				//맵 출력
				printw("%c", map[y][x]);
			}
		}
		printw("\n");
	}
	refresh();
}
int main()
{
	setlocale(LC_CTYPE, "ko_KR.utf8");
	initscr(); 
	keypad(stdscr, TRUE);
	while(1) {
	    print();
	    int ch = getch();
	    /*
	    if (ch == KEY_UP) {
		    // printw("상");
		    if(pu[0]+ )
		    uy--;
	    } else if (ch == KEY_RIGHT) {
		    // printw("우");
		    ux++;
	    } else if(ch == KEY_DOWN) {
		    // printw("하");
		    uy++;
	    } else if(ch == KEY_LEFT) {
		    // printw("좌");
		    ux--;
	    }
	    */
	    for(int d = 0; d < 4; d++) {
		    if(ch == KEY_VALUE[d]) {
			    pu[0] += D[d][0];
			    pu[1] += D[d][1];
		    }
	    }
    }
    getch();
    endwin();
    return 0;
}
