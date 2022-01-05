#include <ncurses.h>
#include <unistd.h>
#include <locale.h>
#include <pthread.h>

#define W 10 // 넓이
#define H 10 // 높이

//유저 파라미터
int pu[2] = {1,1};
int hp = 100;
int score = 0;

//몬스터 파라미터
// int pe[2] = {}


//맵 관련 변수
char WALL= '#';
char TRAP= '^';
char PRIZE='P';
char PRIZE_SCORE=30;

//기타 변수
int D[4][2] = {{-1,0}, {0, 1}, {1, 0}, {0, -1}};
int KEY_VALUE[4] = {KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT};

char MAP[H][W] = {
	"##########",
	"#        #",
	"#        #",
	"#   ^    #",
	"#   ^    #",
	"#        #",
	"#      P #",
	"#      P #",
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
	printw("HP : %d\n", hp);
	printw("SCORE : %d\n", score);
	refresh();
}
void user() {
	while(1) {
	    print();
	    int ch = getch();
	    bool moved = false;
	    for(int d = 0; d < 4; d++) {
		    if(ch == KEY_VALUE[d]) {
			    int ny = pu[0] + D[d][0];
			    int nx = pu[1] + D[d][1];
			    if(ny < 0 || ny >= H ) break;
			    if(nx < 0 || nx >= W ) break;
			    if(MAP[ny][nx] == WALL) break;

			    pu[0] = ny;
			    pu[1] = nx;
			    moved = true;

			    break;
		    }
	    }

	    if(!moved) continue;
	    //움직인 후 이벤트 구현
	    if(MAP[pu[0]][pu[1]] == TRAP) {
	    	//트랩 이벤트
		hp -= 10;
	    }
	    if(MAP[pu[0]][pu[1]] == PRIZE) {
		    MAP[pu[0]][pu[1]] = ' ';
		    score += PRIZE_SCORE;
	    }
    }

}
void enemy() {
}
int main()
{
	setlocale(LC_CTYPE, "ko_KR.utf8");
	initscr(); 
	keypad(stdscr, TRUE);
	
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, user, NULL);
	pthread_join(tid1, NULL);
    getch();
    endwin();
    return 0;
}
