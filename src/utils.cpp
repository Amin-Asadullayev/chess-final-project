#include "utils.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

std::string generateGameFilename() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S");
    return ss.str();
}

template <typename T>
T max(T a, T b) {
    return a > b ? a : b;
}

template <typename T>
T min(T a, T b) {
    return a < b ? a : b;
}

template <typename T>
T abs(T a) {
    return a >= 0 ? a : -a;
}

int convert(char a) {
    int b;
    switch (a) {
        case 'a': b = 0; break;
        case 'b': b = 1; break;
        case 'c': b = 2; break;
        case 'd': b = 3; break;
        case 'e': b = 4; break;
        case 'f': b = 5; break;
        case 'g': b = 6; break;
        case 'h': b = 7; break;
        default: b = 8; break;
    }
    return b;
}
