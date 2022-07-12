#include "editor.h"

#include <fstream>
#include <iostream>
#include <sstream>

// If no filename is given
Editor::Editor() {
    x, y = 0;
    mode = 'n';
    status = "Normal Mode";
    file_name = "untitled";

    // Initializes buffer and appends line to prevent seg. faults
    buff = new Buffer();
    buff->append_line("");
}

// If filename is given
Editor::Editor(std::string fn) {
    x, y = 0;
    mode = 'n';
    status = "Normal Mode";
    file_name = fn;

    buff = new Buffer();

    std::ifstream infile(fn.c_str());
    if (infile.is_open()) {
        while (!infile.eof()) {
            std::string temp;
            std::getline(infile, temp);
            buff->append_line(temp);
        }
    }
    else {
        std::cerr << "Cannot open file: '" << fn << "'\n";
        buff->append_line("");
    }
}

void Editor::update_status() {
    switch (mode)
    {
    case 'n' :
        //n Normal mode
        status = "Insert Mode";
        break;
    case 'i':
        // Insert mode
        status = "Insert Mode";
        break;
    case 'x':
        // Exiting
        status = "Exiting";
        break;
    }
    status += "\tCOL: " + std::to_string(x) + "\tROW: " + std::to_string(y);
}

void Editor::handle_input(int c) {
    switch (c)
    {
    case KEY_LEFT:
        move_left();
        return;
    case KEY_RIGHT:
        move_right();
        return;
    case KEY_UP:
        move_up();
        return;
    case KEY_DOWN:
        move_down();
        return;
    }

    switch(mode)
    {
    case 'n':
        switch(c)
        {
        case 'x':
            // Press 'x' to exit
            mode = 'x';
            break;
        case 'i':
            // Press 'i' to enter insert mode
            mode = 'i';
            break;
        case 's':
            // Press 's' to save the current file
            save_file();
            break;
        }
        break;
    case 'i':
        switch(c)
        {
        case 27:
            // The Escape/Alt key
            mode = 'n';
            break;
        case 127:
        case KEY_BACKSPACE:
            // The Backspace key
            if(x == 0 && y > 0)
            {
                x = buff->lines[y-1].length();
                // Bring the line down
                buff->lines[y-1] += buff->lines[y];
                // Delete the current line
                delete_line();
                move_up();
            }
            else
            {
                // Removes a character
                buff->lines[y].erase(--x, 1);
            }
            break;
        case KEY_DC:
            // The Delete key
            if(x == buff->lines[y].length() && y != buff->lines.size() - 1)
            {
                // Bring the line down
                buff->lines[y] += buff->lines[y+1];
                // Delete the line
                delete_line(y+1);
            }
            else
            {
                buff->lines[y].erase(x, 1);
            }
            break;
        case KEY_ENTER:
        case 10:
            // The Enter key
            // Bring the rest of the line down
            if(x < buff->lines[y].length())
            {
                // Put the rest of the line on a new line
                buff->insert_line(buff->lines[y].substr(x, buff->lines[y].length() - x), y + 1);
                // Remove that part of the line
                buff->lines[y].erase(x, buff->lines[y].length() - x);
            }
            else
            {
                buff->insert_line("", y+1);
            }
            x = 0;
            move_down();
            break;
        case KEY_BTAB:
        case KEY_CTAB:
        case KEY_STAB:
        case KEY_CATAB:
        case 9:
            // The Tab key
            buff->lines[y].insert(x, 4, ' ');
            x += 4;
            break;
        default:
            // Any other character
            buff->lines[y].insert(x, 1, char(c));
            x++;
            break;
        }
        break;
    }  
}

void Editor::move_left() {
    if (x-1 >= 0) {
        x--;
        move(y, x);
    }
}

void Editor::move_right() {
    if (x+1 < COLS && x+1 <= buff->lines[y].length()) {
        x++;
        move(y, x);
    }
}

void Editor::move_up() {
    if (y-1 >= 0) {
        y--;
    } 
    if (x >= buff->lines[y].length()) {
        x = buff->lines[y].length();
    }
    move(y, x);
}

void Editor::move_down() {
    if (y+1 < LINES-1 && y+1 < buff->lines.size()) {
        y++;
    }
    if (x >= buff->lines[y].length()) {
        x = buff->lines[y].length();
    }
    move(y, x);
}