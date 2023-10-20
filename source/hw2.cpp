#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>
#include <termios.h>
#include <fcntl.h>
#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>

#define ROW 10
#define COLUMN 50 
#define LOSE 3
#define WIN 2
#define QUIT 4

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int Status=1;
// 1 running
// 2 win
// 3 lose
// 4 quit
struct Node{
	int x , y; 
	Node( int _x , int _y ) : x( _x ) , y( _y ) {}; 
	Node(){} ; 
} frog ; 

char forgOrign='|';

char map[ROW+10][COLUMN] ; 

void draw_map(){
	//Print the map into screen
	std::cout << "\033[" <<ROW+1<<"A";
	for( int i = 0; i <= ROW; ++i){
		std::cout<<"\r";
		puts( map[i] );
	}
}

// Determine a keyboard is hit or not. If yes, return 1. If not, return 0. 
int kbhit(void){
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);

	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);

	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);

	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}


void* forg_control(void *t){
		while(!*(bool*)t){
		if(kbhit()){
		/*  Check keyboard hits, to change frog's position or quit the game. */
		/*  Check game's status  */
		// check the forg bounds
			char key=getchar();
			if(key=='q'){
				*(bool*)t=true;
				Status=QUIT;
			}
			else if(key=='w'){
				map[frog.x][frog.y]=forgOrign;
				forgOrign=map[--frog.x][frog.y];
				map[frog.x][frog.y]='0';
			}
			else if(key=='s'){
				map[frog.x][frog.y]=forgOrign;
				forgOrign=map[++frog.x][frog.y];
				map[frog.x][frog.y]='0';
			}
			else if(key=='a'){
				map[frog.x][frog.y]=forgOrign;
				forgOrign=map[frog.x][--frog.y];
				map[frog.x][frog.y]='0';
			}
			else if(key=='d'){
				map[frog.x][frog.y]=forgOrign;
				forgOrign=map[frog.x][++frog.y];
				map[frog.x][frog.y]='0';
			}
		
		}
		// judge the status of the game
		if(frog.y<0||frog.x>ROW||forgOrign==' '||frog.y>=COLUMN){
			*(bool*)t=true;
			Status=LOSE;
			// lose
		}else if(frog.x==0){
			// win
			*(bool*)t=true;
			Status=WIN;
		}
		draw_map();
		}
		pthread_exit(NULL);
}


void *logs_move( void *t ){

	/*  Move the logs  */

	// left -1 right 1 from bottom to top
	int directions[]={-1,1,-1,1,-1,1,-1,1,-1};

	// randomly generate the init pos
	int woodStart[ROW-1];
	for(int i=1;i<ROW;i++){
		woodStart[i]=rand()%(COLUMN-1);// 0-49
		for(int j=0;j<14;j++)
			map[i][(woodStart[i]+j)%(COLUMN-1)]='=';
	}
	draw_map();

	

	
	while(!*(bool*)t){
	

	/*  Print the map on the screen  */
	for(int i=1;i<ROW;i++){
		if(directions[i-1]>0){
			woodStart[i]++;
			map[i][(woodStart[i]-1)%(COLUMN-1)]=' ';
			map[i][(woodStart[i]+13)%(COLUMN-1)]='=';
		}else{
			woodStart[i]--;
			if(woodStart[i]<0)woodStart[i]=COLUMN-1;
			map[i][(woodStart[i])%(COLUMN-1)]='=';
			map[i][(woodStart[i]+14)%(COLUMN-1)]=' ';
		}
	}

	// notice this line must be added
	map[frog.x][frog.y]='0';
	// draw_map();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	pthread_exit(NULL);

}

int main( int argc, char *argv[] ){

	// Initialize the river map and frog's starting position
	memset( map , 0, sizeof( map ) ) ;
	int i , j ; 
	for( i = 1; i < ROW; ++i ){	
		for( j = 0; j < COLUMN - 1; ++j )	
			map[i][j] = ' ' ;  
	}	

	for( j = 0; j < COLUMN - 1; ++j )	
		map[ROW][j] = map[0][j] = '|' ;

	for( j = 0; j < COLUMN - 1; ++j )	
		map[0][j] = map[0][j] = '|' ;

	frog = Node( ROW, (COLUMN-1) / 2 ) ; 
	map[frog.x][frog.y] = '0' ; 

	// //Print the map into screen
	// for( i = 0; i <= ROW; ++i)	
	// 	puts( map[i] );
	Status=1;


	/*  Create pthreads for wood move and frog control.  */

	pthread_t thread;
	pthread_t thread2;
    int threadId = 0; // You can use a different thread ID if needed
	bool stopFlag=false;
    pthread_create(&thread, NULL, logs_move, (void *)&stopFlag);
	int result=pthread_create(&thread2,NULL,forg_control,(void*)&stopFlag);

    // Other main thread operations
	// stopFlag=true;

    // Wait for the thread to complete
    pthread_join(thread, NULL);
	pthread_join(thread2,NULL);
	
	/*  Display the output for user: win, lose or quit.  */
	if(Status==WIN){
		std::cout<<"You win the game!!"<<std::endl;
	}else if(Status==LOSE){
		std::cout<<"You lose the game!!"<<std::endl;
	}else{
		std::cout<<"You exit the game."<<std::endl;
	}
	return 0;

}
