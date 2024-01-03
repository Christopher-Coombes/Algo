#include <random>
#include <string>
#include "helpers.h"

int max(int a, int b) {
    return a > b ? a : b;
}

int int_less(int a, int b) {
    return a - b;
}

int numwidth(int n) {
    if (n == 0) return 1;
    int k = 0;
    while (n > 0) {
        k++;
        n /= 10;
    }
    return k;
}

double rand0to1() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0, 1);
    return dis(gen);
}

int rand0toN(int n) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, n);

    return dis(gen);
}