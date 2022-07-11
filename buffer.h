#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <vector>

class Buffer {
    public:
        Buffer();

        std::vector<std::string> lines;

        // Helper functions
        void insertLine(std::string, int);
        void appendLine(std::string);
        void removeLine(int);

        // Subsitute tabs with 4 spaces
        std::string sub_tabs(std::string);
};

#endif