#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

const int numBars = 5;
const int barWidth = 12;
const int delayMilliseconds = 100;

void clearScreen() {
    cout << "\033[H\033[J"; // ANSI escape codes to clear the screen
}

void drawBar(int position, int direction) {
    for (int i = 0; i < numBars; i++) {
        for (int j = 0; j < barWidth; j++) {
            if ((j + position) % barWidth < barWidth / 2) {
                cout << '=';
            } else {
                cout << ' ';
            }
        }
        cout << endl;
    }
}

void animateBars(atomic<bool>& stopFlag) {
    int position = 0;
    int direction = 1; // 1 for right, -1 for left

    while (!stopFlag.load()) {
        clearScreen();
        drawBar(position, direction);

        // Update position and direction
        position += direction;

        // Change direction if the bar reaches the edge
        if (position == 0 || position == barWidth - 1) {
            direction *= -1;
        }

        this_thread::sleep_for(chrono::milliseconds(delayMilliseconds));
    }
}

int main() {
    atomic<bool> stopFlag(false);
    thread animationThread(animateBars, ref(stopFlag));

    // Wait for user input to stop the animation
    cout << "Press Enter to stop the animation..." << endl;
    cin.get();

    // Set the stop flag to stop the animation thread
    stopFlag.store(true);
    animationThread.join();

    return 0;
}

