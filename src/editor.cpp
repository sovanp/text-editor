#include "editor.h"

#include <fstream>
#include <iostream>
#include <sstream>

// For new file
Editor::Editor()
{
    x, y = 0;
    mode = 'n';
    cmd = "";
    lower_bound = 0;
    up_status = true;
    status = "Normal Mode";
    file_name = "";

    // Initializes buffer and appends line to prevent seg. faults
    buff = new Buffer();
    buff->append_line("");
}

// For existing file
Editor::Editor(std::string fn)
{
    x, y = 0;
    mode = 'n';
    cmd = "";
    lower_bound = 0;
    up_status = true;
    status = "Normal Mode";
    file_name = fn;

    buff = new Buffer();

    // Read from file if it exists
    std::ifstream infile(fn.c_str());
    if (infile.is_open())
    {
        while (!infile.eof())
        {
            std::string temp;
            std::getline(infile, temp);
            buff->append_line(temp);
        }
    }
    else
    {
        std::cerr << "Cannot open file: '" << fn << "'\n";
        buff->append_line("");
    }

    infile.close();
}

void Editor::update_status()
{
    switch (mode)
    {
    case 'n':
        if (cmd.empty())
            status = std::string(CED_TITLE) + " " + std::string(CED_VERSION);
        else
            status = cmd;
        break;
    case 'i':
        status = "Insert Mode";
        break;
    case 'x':
        status = "Exiting";
        break;
    }
    status += "\tCOL: " + std::to_string(x) + "\tROW: " + std::to_string(lower_bound);
}

void Editor::handle_input(int c)
{
    up_status = true;
    switch (mode)
    {
    case 'n':
        switch (c)
        {
        case KEY_LEFT:
            move_left();
            break;
        case KEY_RIGHT:
            move_right();
            break;
        case KEY_UP:
            move_up();
            break;
        case KEY_DOWN:
            move_down();
            break;
        case KEY_ENTER:
        case 10:
            // Execute the command
            exec_cmd();
            break;
        case 27:
            // Escape/Alt key
            // clears command
            cmd.clear();
            break;
        case 127:
        case KEY_BACKSPACE:
        case KEY_DC:
            // Removes last character
            if (!cmd.empty())
                cmd.erase(cmd.length() - 1, 1);
            break;
        default:
            // Add character to command
            cmd += std::string(1, char(c));
            break;
        }
        break;
    case 'i':
        switch (c)
        {
        case 27:
            // Escape/Alt key
            mode = 'n';
            break;
        case 127:
        case KEY_BACKSPACE:
            // The Backspace
            if (x == 0 && y > 0)
            {
                x = buff->lines[y - 1].length();
                // Bring line down
                buff->lines[y - 1] += buff->lines[y];
                // Delete the line
                delete_line();
                move_up();
            }
            else if (x > 0)
            {
                buff->lines[y].erase(--x, 1);
            }
            break;
        case KEY_DC:
            // The Delete key
            if (x == buff->lines[y].length() && y != buff->lines.size() - 1)
            {
                // Bring line down
                buff->lines[y] += buff->lines[y + 1];
                // Delete the line
                delete_line(y + 1);
            }
            else
            {
                buff->lines[y].erase(x, 1);
            }
            break;
        case KEY_LEFT:
            move_left();
            break;
        case KEY_RIGHT:
            move_right();
            break;
        case KEY_UP:
            move_up();
            break;
        case KEY_DOWN:
            move_down();
            break;
        case KEY_ENTER:
        case 10:
            // Bring rest of line down
            if (x < buff->lines[y + lower_bound].length() - 1)
            {
                // Put rest of line on new line
                buff->insert_line(buff->lines[y + lower_bound].substr(x, buff->lines[y + lower_bound].length() - x), y + 1);
                // Remove that part of the line
                buff->lines[y + lower_bound].erase(x, buff->lines[y + lower_bound].length() - x);
            }
            else
                buff->insert_line("", y + lower_bound + 1);
            move_down();
            break;
        case KEY_BTAB:
        case KEY_CTAB:
        case KEY_STAB:
        case KEY_CATAB:
        case 9:
            // The tab
            buff->lines[y + lower_bound].insert(x, 4, ' ');
            x += 4;
            break;
        default:
            buff->lines[y + lower_bound].insert(x, 1, char(c));
            x++;
            break;
        }
        break;
    default:;
    }
}

void Editor::move_left()
{
    if (x - 1 >= 0)
    {
        x--;
        move(y, x);
    }
}

void Editor::move_right()
{
    if (x + 1 < COLS && x + 1 <= buff->lines[y].length())
    {
        x++;
        move(y, x);
    }
}

void Editor::move_up()
{
    if (y - 1 >= 0)
    {
        y--;
    }
    else if (y - 1 < 0 && lower_bound > 0)
    {
        lower_bound--;
    }
    if (x >= buff->lines[y].length())
        x = buff->lines[y].length();
    move(y, x);
}

void Editor::move_down()
{
    if (y + 1 < LINES - 1 && y + 1 < buff->lines.size())
    {
        y++;
    }
    else if (lower_bound + y < buff->lines.size())
    {
        lower_bound++;
    }
    if (x >= buff->lines[y].length())
    {
        x = buff->lines[y].length();
    }
    move(y, x);
}

void Editor::print_buffer()
{
    int line_count = 0;
    for (int i = lower_bound; line_count < LINES - 1; i++)
    {
        if (i >= buff->lines.size())
        {
        }
        else
        {
            mvprintw(line_count, 0, buff->lines[i].c_str());
        }
        clrtoeol();
        line_count++;
    }
    move(y, x);
}

void Editor::print_status_line()
{
    attron(A_REVERSE);
    mvprintw(LINES - 1, 0, status.c_str());
    clrtoeol();
    attroff(A_REVERSE);
}

void Editor::delete_line()
{
    buff->remove_line(y);
}

void Editor::delete_line(int i)
{
    buff->remove_line(i);
}

void Editor::save_file()
{
    if (file_name == "")
    {
        // Set filename to untitled
        file_name = "untitled";
    }

    std::ofstream f(file_name.c_str());
    if (f.is_open())
    {
        for (int i = 0; i < buff->lines.size(); i++)
        {
            f << buff->lines[i] << std::endl;
        }
        status = "Saved to file!";
    }
    else
    {
        status = "Error: Cannot open file for writing!";
    }
    f.close();
}

bool Editor::exec_cmd()
{
    if (cmd == "i")
        mode = 'i';
    else if (cmd == "x")
        mode = 'x';
    else if (cmd == "s")
    {
        up_status = false;
        save_file();
    }

    cmd = "";    // Reset command buffer
    return true; // Returns if command has executed successfully
}