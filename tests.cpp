#include "LongNumber.h"
#include <iostream>
#include <random>

constexpr double epsilon = 1e-5;
constexpr int test_count = 100;
std::mt19937_64 mtrand;

long double random_short_double() {
    constexpr long long max_gen = (1ll << 31ll);
    long long rv = mtrand();
    bool sign = rv & 1;
    rv >>= 1;
    return (sign ? -1 : 1) * static_cast<long double>(rv & (max_gen - 1)) / max_gen;
}

bool epsilon_compare(LongNumber a, LongNumber b) {
    LongNumber diff = LongNumber::abs(a - b);
    return diff < LongNumber::new_number(epsilon);
}

inline bool check_addition(double a, double b) {
    return epsilon_compare(LongNumber::new_number(a) + LongNumber::new_number(b), LongNumber::new_number(a + b));
}
inline bool check_subtraction(double a, double b) {
    return epsilon_compare(LongNumber::new_number(a) - LongNumber::new_number(b), LongNumber::new_number(a - b));
}
inline bool check_multiplication(double a, double b) {
    return epsilon_compare(LongNumber::new_number(a) * LongNumber::new_number(b), LongNumber::new_number(a * b));
}
inline bool check_division(double a, double b) {
    return epsilon_compare(LongNumber::new_number(a) / LongNumber::new_number(b), LongNumber::new_number(a / b));
}
inline bool check_comparison(double a, double b) {
    bool eq = (LongNumber::new_number(a) == LongNumber::new_number(b)) == (a == b);
    bool neq = (LongNumber::new_number(a) != LongNumber::new_number(b)) == (a != b);
    bool g = (LongNumber::new_number(a) > LongNumber::new_number(b)) == (a > b);
    bool ng = (LongNumber::new_number(a) <= LongNumber::new_number(b)) == (a <= b);
    bool nl = (LongNumber::new_number(a) >= LongNumber::new_number(b)) == (a >= b);
    bool l = (LongNumber::new_number(a) < LongNumber::new_number(b)) == (a < b);
    return eq && neq && g && ng && nl && l;
}
inline bool check_selfequality(double a) {
    LongNumber al = LongNumber::new_number(a);
    LongNumber bl = al;
    bl = LongNumber::new_number(a);
    bool res = (al == bl);
    return res;
}

int main() {
    bool correct;

    correct = true;
    for (int i = 0; i < test_count; i++) correct = correct && check_addition(random_short_double(), random_short_double());
    std::cout << "Addition test: " << (correct ? "OK" : "FAIL") << '\n';

    correct = true;
    for (int i = 0; i < test_count; i++) correct = correct && check_subtraction(random_short_double(), random_short_double());
    std::cout << "Subtraction test: " << (correct ? "OK" : "FAIL") << '\n';

    correct = true;
    for (int i = 0; i < test_count; i++) correct = correct && check_multiplication(random_short_double(), random_short_double());
    std::cout << "Multiplication test: " << (correct ? "OK" : "FAIL") << '\n';

    correct = true;
    for (int i = 0; i < test_count; i++) correct = correct && check_division(random_short_double(), random_short_double());
    std::cout << "Division test: " << (correct ? "OK" : "FAIL") << '\n';

    correct = true;
    for (int i = 0; i < test_count; i++) correct = correct && check_comparison(random_short_double(), random_short_double());
    std::cout << "Comparison test: " << (correct ? "OK" : "FAIL") << '\n';

    correct = true;
    for (int i = 0; i < test_count; i++) correct = correct && check_selfequality(random_short_double());
    std::cout << "Constructor/Destruction test: " << (correct ? "OK" : "FAIL") << '\n';
}
