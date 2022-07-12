#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <vector>

class Buffer {
    public:
        Buffer();

        std::vector<std::string> lines;

        // Helper functions
        void insert_line(std::string, int);
        void append_line(std::string);
        void remove_line(int);

        // Subsitute tabs with 4 spaces
        std::string sub_tabs(std::string);
};

#endif