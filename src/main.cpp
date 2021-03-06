#include <curses.h>
#include <string>
#include "editor.h"

void curses_init()
{
    initscr();            // Start curses mode
    noecho();             // Don't repeat keypresses
    cbreak();             // Disable line buffering
    keypad(stdscr, true); // Enable special keys to be recorded
}

std::string fn = ""; // Create string variable

int main(int argc, char *argv[])
{
    Editor ed; // Create Editor class instance

    if (argc > 1)
    {
        fn = std::string(argv[1]); // Set the file name
        ed = Editor(fn);           // Use filename constructor
    }
    else
    {
        ed = Editor(); // Use default constructor
    }

    curses_init(); // Initialize curses
    // Main loop
    while (ed.get_mode() != 'x')
    {
        if (ed.up_status)
        {
            ed.update_status();
        }
        ed.print_status_line();
        ed.print_buffer();
        int input = getch(); // Block until input
        ed.handle_input(input);
    }

    refresh(); // Refresh curses
    endwin();  // End curses mode

    return 0;
}