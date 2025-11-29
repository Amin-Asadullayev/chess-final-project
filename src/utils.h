#ifndef UTILS_H
#define UTILS_H
#include <string>
std::string generateGameFilename();
template <typename T>
T maxstd(T a, T b) {
    return (a > b) ? a : b;
}

template <typename T>
T minstd(T a, T b) {
    return (a < b) ? a : b;
}

template <typename T>
T absstd(T a) {
    return (a >= 0) ? a : -a;
}
int convert(char a);

#endif
