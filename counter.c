#include <pthread.h>
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>

pthread_mutex_t mtx;

void *down() {
	int num = 0;
	while(1) {
		pthread_mutex_lock(&mtx);
		mvprintw(10, 30, "%d", num);
		refresh();
		pthread_mutex_unlock(&mtx);
		sleep(1);
		num--;
	}
}
void *up() {
	int num = 0;
	while(1) {
		
		pthread_mutex_lock(&mtx);
		mvprintw(10, 10, "%d", num);
		refresh();
		pthread_mutex_unlock(&mtx);
		sleep(1);
		num++;
	}
}
int main() {
	int num = 0; 
	
	initscr();

	pthread_t tid1, tid2;

	pthread_create(&tid1, NULL, up, NULL);
	pthread_create(&tid2, NULL, down, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	
	getch();
	endwin();
	return 0;
}
