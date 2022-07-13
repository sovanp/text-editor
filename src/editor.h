#ifndef EDITOR_H
#define EDITOR_H

#include <curses.h>
#include "buffer.h"

#define CED_TITLE "text-editor"
#define CED_VERSION "v0.1.0 test"

class Editor
{
private:
    int x, y; // Screen positions
    char mode;
    int lower_bound;
    Buffer *buff;
    std::string status, file_name, cmd;

    bool exec_cmd();

    // Cursor movement
    void move_up();
    void move_down();
    void move_left();
    void move_right();

    void delete_line();    // Deletes current line
    void delete_line(int); // Deletes line <int>

    void save_file(); // Saves buffer into file

public:
    bool up_status;

    Editor();            // Default constructor
    Editor(std::string); // Constructor accepting filename argument

    char get_mode() { return mode; }

    void handle_input(int); // Handles keyboard input
    void print_buffer();
    void print_status_line(); // Prints the status line
    void update_status();     // Updates the status line (text, not display)
};

#endif