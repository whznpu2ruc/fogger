# Frogger Game Report

## Project Overview

This project is a terminal-based Frogger game where players control a frog to cross a river while avoiding moving logs. The game's main features include frog movement control, log movement, game state evaluation, and user input handling.

## Design

### Frog Movement Control

The `forg_control` function is responsible for controlling the frog's movement. It checks for user input and updates the frog's position accordingly.

```cpp
cppCopy codevoid* forg_control(void *t){
    while(!*(bool*)t){
        if(kbhit()){
            // Check for keyboard input to change the frog's position or quit the game
            char key = getchar();
            if(key == 'q'){
                // Quit the game
                *(bool*)t = true;
                Status = QUIT;
            }
            else if(key == 'w'){
                // Move the frog upward
                // Update frog's position and game map
            }
            // ... Handle 's', 'a', 'd' keys for movement
        }
        // Check the game status
        if(frog.y < 0 || frog.x > ROW || forgOrign == ' ' || frog.y >= COLUMN){
            // Game over (lose)
            *(bool*)t = true;
            Status = LOSE;
        }
        else if(frog.x == 0){
            // Game over (win)
            *(bool*)t = true;
            Status = WIN;
        }
        draw_map();
    }
    pthread_exit(NULL);
}
```

### Log Movement

The `logs_move` function controls the movement of logs across the river. Logs move from bottom to top and may go left or right. The directions array controls the movement direction of each log.

```cpp
cppCopy codevoid *logs_move(void *t){
    int directions[] = {-1, 1, -1, 1, -1, 1, -1, 1, -1};
    // ... Initialize and display logs

    while(!*(bool*)t){
        // Update the log positions
        for(int i = 1; i < ROW; i++){
            if(directions[i-1] > 0){
                // Move log to the right
                // Update game map
            }
            else {
                // Move log to the left
                // Update game map
            }
        }
        // Check if frog collides with a log
        if(map[frog.x][frog.y] == ' '){
            // Game over (lose)
            *(bool*)t = true;
            Status = LOSE;
        }
        else {
            // Update frog's position on the map
            map[frog.x][frog.y] = '0';
        }
        // Sleep to control log movement speed
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    pthread_exit(NULL);
}
```



## main thread

### Cursor Blinking Issue

During the game, the cursor blinks continuously. This is due to the cursor not being hidden while the program is running. To address this problem, we added functions to hide and show the cursor, ensuring that the cursor remains hidden during gameplay.

### User Input Visibility Issue

By default, user input characters are visible on the screen. This is not desirable for a game where user input should remain hidden. To address this issue, we used terminal settings to set the terminal to a no-echo mode, ensuring that user input characters are not displayed on the screen.Code Analysis

```c++
int main( int argc, char *argv[] ){
	hide_cursor();
	struct termios originalTermSettings;
    tcgetattr(STDIN_FILENO, &originalTermSettings);
	struct termios termSettings = originalTermSettings;
	termSettings.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &termSettings);
	// Initialize the river map and frog's starting position
	init_map();
	std::cout << "\033[" <<ROW+1<<"A";
	for( int i = 0; i <= ROW; ++i){
		std::cout<<"\r";
		std::cout<<"                                                 ";
	} 
	std::cout<<"\r";
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
	std::cout << "\033[" <<ROW+1<<"A";
	for( int i = 0; i <= ROW; ++i){
		std::cout<<"\r";
		std::cout<<"                                                 ";
	} 
	std::cout<<"\r";

	if(Status==WIN){
		std::cout<<"You win the game!!"<<std::endl;
	}else if(Status==LOSE){
		std::cout<<"You lose the game!!"<<std::endl;
	}else{
		std::cout<<"You exit the game."<<std::endl;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &originalTermSettings);
	show_cursor();

	return 0;

}
```





## Conclusion

The Frogger game is an interesting terminal-based game where players control a frog to cross a river while avoiding moving logs. By addressing issues such as cursor blinking and visible user input, the program provides an improved gaming experience. The code analysis and explanations provided in this report help developers understand the key components of the game, including frog movement control, log movement, and game logic evaluation.