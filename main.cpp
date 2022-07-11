#include <iostream>
#include <curses.h>
#include <string>


void curses_init() {
     initscr();                 // Start curses mode
     noecho();                  // Don't repeat keypresses
     cbreak();                  // Disable line buffering
     keypad(stdscr, true);      // Enable special keys to be recorded
}


int main(int argc, char *argv[]) {
    std::string file_name = ""; // Create string variable

    if (argc > 1) {
        file_name = argv[1];    // Set the file name
    }

    curses_init();              // Initialize curses

    refresh();                  // Refresh curses
    endwin();                   // End curses mode

    return 0;
}