#include <ncurses.h>
#include <unistd.h>
#include <locale.h>

#define W 10 // 넓이
#define H 10 // 높이

//유저 파라미터
int pu[2] = {1,1};
int hp = 100;

//위치 관련 변수
int D[4][2] = {{-1,0}, {0, 1}, {1, 0}, {0, -1}};
int KEY_VALUE[4] = {KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT};

//형상 관련 변수
char WALL= '#';
char MAP[H][W] = {
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
				printw("%c", MAP[y][x]);
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
	    for(int d = 0; d < 4; d++) {
		    if(ch == KEY_VALUE[d]) {
			    int ny = pu[0] + D[d][0];
			    int nx = pu[1] + D[d][1];
			    if(ny < 0 || ny >= H ) break;
			    if(nx < 0 || nx >= W ) {
				    printw("zzz");
				    break;
			    }
			    if(MAP[ny][nx] == WALL) break;

			    pu[0] = ny;
			    pu[1] = nx;

			    break;
		    }
	    }
    }
    getch();
    endwin();
    return 0;
}
