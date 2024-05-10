#ifndef GENERICUTILS_H
#define GENERICUTILS_H

#include <string>
#include "../utils/constants.h"

namespace utils {
    inline std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        size_t last = str.find_last_not_of(" \t\n\r");

        // String contains only whitespace
        if (first == std::string::npos || last == std::string::npos)
            return "";

        return str.substr(first, last - first + 1);
    }

    inline std::string commToString(Command comm) {
        switch (comm) {
            case COMM_HELP: return "help";
            case COMM_CREATE_USER: return "createuser";
            default: return "Unknown";
        }
    }
}

#endif