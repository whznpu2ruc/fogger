#include <iostream>
#include <cstdlib>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

const int riverWidth = 20;
const char frogSymbol = 'F';
const char bridgeSymbol = '=';

int frogPositionX = 0;
int frogPositionY = riverWidth / 2;
int bridgePosition = 0;

int getRandomBridgePosition() {
    return rand() % riverWidth;
}

void clearScreen() {
    cout << "\033[H\033[J"; // ANSI escape codes to clear the screen
}

void drawRiver() {
    cout << string(riverWidth, '=') << endl;
}

void drawFrog() {
    for (int i = 0; i < riverWidth; i++) {
        if (i == frogPositionX) {
            cout << frogSymbol;
        } else {
            cout << ' ';
        }
    }
    cout << endl;
}

void drawBridge() {
    cout << string(bridgePosition, ' ') << string(2, bridgeSymbol) << string(riverWidth - bridgePosition - 2, ' ') << endl;
}

int main() {
    srand(time(0));

    int original_terminal_attributes;
    struct termios new_terminal_attributes;
    tcgetattr(STDIN_FILENO, &new_terminal_attributes);
    original_terminal_attributes = new_terminal_attributes.c_lflag;
    new_terminal_attributes.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal_attributes);

    int fd = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(fd, F_SETFL, fd | O_NONBLOCK);

    char key = 0;
    bool gameover = false;

    while (!gameover) {
        clearScreen();
        drawRiver();
        drawBridge();
        drawFrog();

        char c = 0;
        read(STDIN_FILENO, &c, 1);
        key = tolower(c);

        int prevFrogPositionX = frogPositionX;
        int prevFrogPositionY = frogPositionY;

        if (key == 'a' && frogPositionX > 0) {
            frogPositionX--;
        } else if (key == 'd' && frogPositionX < riverWidth - 1) {
            frogPositionX++;
        } else if (key == 'w' && frogPositionY > 0) {
            frogPositionY--;
        } else if (key == 's' && frogPositionY < riverWidth - 1) {
            frogPositionY++;
        } else if (key == 'q') {
            gameover = true;
        }

        if (frogPositionX == bridgePosition) {
            bridgePosition = getRandomBridgePosition();
        }

        if (frogPositionX == riverWidth - 1) {
            gameover = true; // Winning condition
        }

        if (prevFrogPositionX != frogPositionX || prevFrogPositionY != frogPositionY) {
            usleep(100000); // Sleep for 100 ms
        }
    }

    clearScreen();
    if (frogPositionX == riverWidth - 1) {
        cout << "You made it across the river! Frog wins!" << endl;
    } else {
        cout << "Game over. Frog didn't make it to the other side." << endl;
    }

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal_attributes);
    fcntl(fd, F_SETFL, fd & ~O_NONBLOCK);

    return 0;
}
