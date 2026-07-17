#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <cctype>
#include <sstream>
 
#if defined(_WIN32) || defined(_WIN64)
    #include <io.h>
    #include <cstdio>
    #define IS_TTY(x) _isatty(x)
    #define STDIN_FD _fileno(stdin)
    #define CLEAR_COMMAND "cls"
#else
    #include <unistd.h>
    #define IS_TTY(x) isatty(x)
    #define STDIN_FD STDIN_FILENO
    #define CLEAR_COMMAND "clear"
#endif

#include "BookExceptions.h"

namespace Utils {

    inline bool isEmptyOrWhitespace(const std::string& str) {
        return str.empty() ||
               std::all_of(str.begin(), str.end(),
                           [](unsigned char c) { return std::isspace(c); });
    }

    inline bool isValidISBN(const std::string& isbn) {
        int digitCount = 0;
        for (char c : isbn) {
            if (std::isdigit(static_cast<unsigned char>(c))) {
                ++digitCount;
            } else if (c != '-') {
                return false;
            }
        }
        return digitCount == 10 || digitCount == 13;
    }

    inline bool isValidYear(int year) {
        const int MIN_YEAR = 1450;
        const int MAX_YEAR = 2026;
        return year >= MIN_YEAR && year <= MAX_YEAR;
    }

    inline std::string normalizeISBN(const std::string& isbn) {
        std::string result;
        result.reserve(isbn.size());
        for (char c : isbn) {
            if (c != '-') {
                result += c;
            }
        }
        return result;
    }

    inline void clearInputStream() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    inline void pauseScreen() {
        if (!IS_TTY(STDIN_FD) || std::cin.eof()) {
            return;
        }
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    inline void clearScreen() {
        if (!IS_TTY(STDIN_FD)) {
            return;
        }
        std::system(CLEAR_COMMAND);
    }

    inline std::string getNonEmptyString(const std::string& prompt,
                                          const std::string& fieldName) {
        std::string input;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, input);
            if (isEmptyOrWhitespace(input)) {
                std::cerr << "[X] Field '" << fieldName
                          << "' cannot be empty. Please try again.\n";
                continue;
            }
            return input;
        }
    }

    inline int getIntInRange(const std::string& prompt, int min, int max) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                if (value >= min && value <= max) {
                    clearInputStream();
                    return value;
                }
                std::cerr << "[X] Value must be between " << min << " and "
                          << max << ". Please try again.\n";
            } else {
                std::cerr << "[X] Invalid input. Please enter an integer.\n";
            }
            clearInputStream();
        }
    }

    inline bool getYesNo(const std::string& prompt) {
        char choice;
        while (true) {
            std::cout << prompt;
            std::cin >> choice;
            clearInputStream();
            if (choice == 'y' || choice == 'Y') return true;
            if (choice == 'n' || choice == 'N') return false;
            std::cerr << "[X] Please enter 'y' or 'n'.\n";
        }
    }

    inline std::string getValidISBN(const std::string& prompt) {
        std::string isbn;
        while (true) {
            isbn = getNonEmptyString(prompt, "ISBN");
            if (isValidISBN(isbn)) {
                return normalizeISBN(isbn);
            }
            std::cerr << "[X] Invalid ISBN. Must have 10 or 13 digits "
                         "(you can use hyphens).\n";
        }
    }

    inline int getValidYear() {
        return getIntInRange("Publication year (1450-2026): ", 1450, 2026);
    }

    inline std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return result;
    }

}

#endif
