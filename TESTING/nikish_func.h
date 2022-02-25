// Nikita Ignatov 022401 y.2020
// Fuctions that may be used in every labaratory work, or i found them useful

#pragma once // to prevent from multiple including header file
#define NULL 0

#include <iostream>

namespace custom
{
    // Fucntion, that checks if input is non-numerical
    inline int get_int() {                                      // inline - is able to be defined multiple times.
        int result = 0;
        char c_space[256];
        int err = 1;
        while (err) {
            std::cin >> c_space;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            result = atoi(c_space);		// convert char into int
            for (unsigned int i = 0; c_space[i] != '\0'; i++) {
                err = 0;
                if (i == 0 && (c_space[i] == '+'));	 // Получение знака числа;
                else if (!isdigit((unsigned char)c_space[i]) && c_space[i] != '.') {	// Получение числа с плавающей точкой и экспоненциальной формы (in progress)
                    std::cout << "Non-numerical input. Try again: ";
                    err = 1;
                    break;
                }
            }
        }
        return result;
    }

    // Simple getline function, that also can count input
    inline int getline(char* line, int max_lenght) {
        char input;
        if (max_lenght <= 0)
            return -1;
        for (int i = 0; i < max_lenght - 1 && (input = getchar()) != EOF; ++i) {
            line[i] = input;
            if (line[i] == '\n') {
                line[i] = '\0';
                return i + 1;
            }
        }
        return -1;
    }

    // strcpy functio implementation
	inline char* strcpy_p(char* destination, const char* source) {
        if (destination == NULL)
            return NULL;
        // take a pointer pointing to the beginning of destination string
        char* ptr = destination;
        // while character in not '\0' (0)
        while (*source) {
            *destination = *source;
            destination++;
            source++;
        }
        // include the terminating null character
        *destination = '\0';
        return ptr;
    }

    // strlen function implementation 
    inline size_t strlen_p(const char* str) {
        size_t length = 0;
        // while character is not '\0' (0)
        while (*str++)
            length++;
        return length;
    }
}