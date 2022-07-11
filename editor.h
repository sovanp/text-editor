#ifndef EDITOR_H
#define EDITOR_H

#include <curses.h>
#include "buffer.h"

class Editor {
    private:
        int x, y;                           // Screen positions
        char mode;
        Buffer *buffs;
        std::string status, file_name;

        // Cursor movement
        void move_up();
        void move_down();
        void move_left();
        void move_right();

        void delete_line();                 // Deletes current line
        void delete_line(int);              // Deletes line <int>

        void save_file();                   // Saves buffer into file

    public:
        Editor();                           // Default constructor
        Editor(std::string);                // Constructor accepting filename argument

        char get_mode() {return mode;}

        void handle_input(int);             // Handles keyboard input
        void print_buffer();
        void print_status_line();           // Prints the status line
        void update_status();               // Updates the status line (text, not display)
};