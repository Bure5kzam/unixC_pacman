#include <ncurses.h>
#include <unistd.h>
#include <locale.h>
#include <pthread.h>
#include <string.h>

#define W 10 // 넓이
#define H 10 // 높이

//유저 파라미터
int pu[2] = {1,1};
int hp = 100;
int score = 0;

//몬스터 관련 변수
int pm[4][2] = {{3,3},{4,4},{5,5}};
bool pm_list[H][W] = {false, };
char MONSTER='M';


//맵 관련 변수
char WALL= '#';
char TRAP= '^';
char PRIZE='P';
char PRIZE_SCORE=30;
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
//멀티 쓰레드
pthread_t tid1, tid2;
pthread_mutex_t mtx_pm, mtx2;

//기타 변수
int D[4][2] = {{-1,0}, {0, 1}, {1, 0}, {0, -1}};
int KEY_VALUE[4] = {KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_LEFT};
bool isEnd = false;
void print_end() {
	clear();
	for(int i = 0; i < H; i++) {
		for(int j = 0; j < W; j++) {
			if(i == 0 || i == H-1 || j == 0 || j == W-1) printw("A");
			else printw(" ");
		}
		printw("\n");
	}
	mvprintw((H / 2)-1 , (W /2)-1, "R");
	mvprintw((H / 2)-1 , (W /2), "I");
	mvprintw((H / 2)-1 , (W /2)+1, "P");

	refresh();

}
void print() {
	pthread_mutex_lock(&mtx2);
	clear();
	for ( int y = 0; y < H; y++) {
		for ( int x = 0; x < W; x++) {
	pthread_mutex_lock(&mtx_pm);

			if(pm_list[y][x]) {
				printw("%c", MONSTER);
			}
			else if(y == pu[0] && x == pu[1]) {
				//유저 출력
				printw("@");
			} else {
				//맵 출력
				printw("%c", MAP[y][x]);
			}
	pthread_mutex_unlock(&mtx_pm);
		}

		printw("\n");
	}
	printw("HP : %d\n", hp);
	printw("SCORE : %d\n", score);

	refresh();

	pthread_mutex_unlock(&mtx2);

}
int moveEvent() {
	print();

	//움직였을 때 발생할 수 있는 이벤트들

	pthread_mutex_lock(&mtx_pm);
	if(pm_list[pu[0]][pu[1]]) {
		//몬스터와 부딪혔을 경우
		isEnd = true;
		pthread_mutex_unlock(&mtx_pm);
		return 0;
	}
	pthread_mutex_unlock(&mtx_pm);
	return 1;

}
void user() {
	int ret = 0;
	print();
	while(!isEnd) {
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

	    if(moved) {
			//유저가 움직인 시점에만 동작할 이벤트 구현
			if(MAP[pu[0]][pu[1]] == TRAP) {
				//트랩 이벤트
			hp -= 10;
			}
			if(MAP[pu[0]][pu[1]] == PRIZE) {
				MAP[pu[0]][pu[1]] = ' ';
				score += PRIZE_SCORE;
			}
		}
		ret = moveEvent();
		if(!ret) break;
    }

}
void enemy() {
	int seed = 0, ret = 0;
	while(!isEnd) {
		sleep(1);
		for(int m = 0 ; m <3; m++) {
			seed = rand() % 4;
			int cy = pm[m][0], cx = pm[m][1];
			int ny = cy + D[seed][0], nx = cx + D[seed][1];
			if(ny < 0 || ny >= H ) continue;
			if(nx < 0 || nx >= W ) continue;
			if(MAP[ny][nx] == WALL) continue;

			//mutex 적용
			pthread_mutex_lock(&mtx_pm);
			//pm_list에 이동 반영
			pm_list[cy][cx] = false;
			pm_list[ny][nx] = true;

			//pm에 이동 반영
			pm[m][0] = ny;
			pm[m][1] = nx;
			pthread_mutex_unlock(&mtx_pm);
			ret = moveEvent();
			if(!ret) break;
		}
	}
}
int main()
{
	setlocale(LC_CTYPE, "ko_KR.utf8");
	initscr(); 
	memset(pm_list, false, sizeof(bool)*H*W);
	
	keypad(stdscr, TRUE);
	
	
	pthread_create(&tid1, NULL, user, NULL);
	pthread_create(&tid2, NULL, enemy, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	print_end();
	sleep(2);

    getch();
    endwin();
    return 0;
}
