#include "LongNumber.h"
#include <iostream>
#include <random>
#include <string>
#include <chrono>

LongNumber compute_pi_slow(int steps) {
    constexpr size_t precision = 8;
    LongNumber k16 = LongNumber::new_number(1); k16.set_precision(precision);
    LongNumber result = LongNumber::new_number(0); result.set_precision(precision);
    LongNumber one = LongNumber::new_number(1); one.set_precision(precision);
    LongNumber two = one + one;
    LongNumber four = two + two;
    LongNumber five = four + one;
    LongNumber six = four + two;
    for (int i = 0; i < steps; i++) {
        LongNumber k = LongNumber::new_number(i); k.set_precision(precision);
        LongNumber k8 = k; k8 = k8 << true << true << true;
        LongNumber c1 = four / (k8 + one);
        LongNumber c2 = two / (k8 + four);
        LongNumber c3 = one / (k8 + five);
        LongNumber c4 = one / (k8 + six);
        LongNumber pr = (c1 - c2 - c3 - c4) * k16;
        k16 = k16 >> true >> true >> true >> true;
        result = result + pr;
    }
    return result;
}



int main(int argc, char** argv) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    LongNumber x = compute_pi_slow((argc < 2) ? 128 : 2*stoi(std::string(argv[1])));
    int decimal = (argc < 2) ? 100 : stoi(std::string(argv[1]));
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time elapsed = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "ms elapsed" << std::endl;
    if (x.get_sign()) std::cout << "-";
    LongNumber ten = LongNumber::new_number(10); ten.set_precision(x.get_precision());
    for(int i = 0; i < decimal+1; i++) {
        std::cout << x.get_int();
        if (i == 0) std::cout << ".";
	    x = x - LongNumber::new_number(x.get_int());
        x = x * ten;
    }
    std::cout << "\n";
}
