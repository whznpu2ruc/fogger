#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

using namespace std;

int main() {
    const int numRows = 3;
    char messages[numRows][100] = {
        "Message 1",
        "Message 2",
        "Message 3"
    };
    char messages2[numRows][100] = {
        "Message 2",
        "Message 3",
        "Message 4"
    };
    char messages3[numRows][100] = {
        "Message 3",
        "Message 4",
        "Message 5"
    };
    // Output multiple lines of messages
    for (int i = 0; i < numRows; i++) {
        cout << messages[i] << endl;
    }

    // Sleep for a while (simulating some processing)
    // Replace this with your actual logic
    // Sleep for 2 seconds
    this_thread::sleep_for(chrono::seconds(2));

    // Use ANSI escape codes to move the cursor up to the first message and overwrite the lines
    cout << "\033[" << numRows << "A";  // Move cursor up numRows lines
    for (int i = 0; i < numRows; i++) {
        cout << "\r";  // Move cursor to the beginning of the line
        cout << messages2[i]<<endl;
    }

    // Sleep for a while again
    // Sleep for 2 seconds
    this_thread::sleep_for(chrono::seconds(2));

    // Clear the lines to ensure the entire lines are overwritten
    cout << "\033[" << numRows << "A";  // Move cursor up numRows lines
    for (int i = 0; i < numRows; i++) {
        cout << "\r";  // Move cursor to the beginning of the line
        cout << messages3[i]<<endl;
    }
    cout << "\033[" << numRows << "B";  // Move cursor down numRows lines

    return 0;
}
