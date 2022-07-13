#include "buffer.h"

Buffer::Buffer() {}

std::string Buffer::sub_tabs(std::string line)
{
    int tab = line.find("\t");
    if (tab == line.npos)
    {
        return line;
    }
    else
    {
        return sub_tabs(line.replace(tab, 1, "    "));
    }
}

void Buffer::insert_line(std::string line, int n)
{
    line = sub_tabs(line);
    lines.insert(lines.begin() + n, line);
}

void Buffer::append_line(std::string line)
{
    line = sub_tabs(line);
    lines.push_back(line);
}

void Buffer::remove_line(int n)
{
    lines.erase(lines.begin() + n);
}