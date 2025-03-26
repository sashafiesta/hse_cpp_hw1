#include <iostream>
#include <algorithm>
#include <limits>
#include <vector>
#include "LongNumber.h"

void LongNumber::mshl() {
    for (int i = 0; i < precision + 1; i++)
        numbers.push_back(0);
    precision = precision * 2 + 1;
}
void LongNumber::mshr() {
    size_t target = numbers.size() / 2;
    numbers.pop_back();
    std::reverse(numbers.begin(), numbers.end());
    while (numbers.size() > target)
        numbers.pop_back();
    precision = target - 1;
    std::reverse(numbers.begin(), numbers.end());
}

bool LongNumber::get_sign() const {
	return sign;
}

unsigned long long LongNumber::get_int() const {
	return numbers.back();
}

inline bool LongNumber::checkbit(size_t k) {
    unsigned long long pointer = k / (sizeof(unsigned long long) * 8);
    unsigned long long bitmask = (1ull << (k % (sizeof(unsigned long long) * 8)));
    return (numbers[pointer] & bitmask) != 0;
}
bool LongNumber::check_shl_carry() {
    return (numbers.back() & (1ull << (sizeof(unsigned long long) * 8 - 1))) != 0;
}

int LongNumber::compare(const LongNumber& a, const LongNumber& b) {
    if (a.sign && !b.sign)
        return -1;
    if (!a.sign && b.sign)
        return 1;
    LongNumber x = a;
    LongNumber y = b;
    if (a.precision != b.precision) {
        size_t target = (a.precision > b.precision) ? a.precision : b.precision;
        x.set_precision(target);
        y.set_precision(target);
    }
    if (x.numbers == y.numbers)
        return 0;
    std::reverse(x.numbers.begin(), x.numbers.end());
    std::reverse(y.numbers.begin(), y.numbers.end());
    int res = ((x.numbers > y.numbers) ? 1 : -1);
    return res * (a.sign ? -1 : 1);
}

LongNumber LongNumber::new_number(long double ld) {
    if (ld < 0)
        return -new_number(-ld);
    LongNumber result = LongNumber(1);
    result.numbers[1] = (unsigned long long)ld;
    result.numbers[0] = (unsigned long long)((ld - result.numbers[1]) * (std::numeric_limits<unsigned long long>::max() + (long double)1));
    return result;
}
LongNumber operator ""_longnum(long double number) {
    return LongNumber::new_number(number);
}
LongNumber LongNumber::abs(const LongNumber& longnumber) {
    LongNumber result = longnumber;
    result.sign = false;
    return result;
}
LongNumber LongNumber::operator>>(const bool& doShift) const {
    LongNumber result = *this;
    if (!doShift)
        return result;
    for (size_t i = 0; i < result.numbers.size() - 1; i++) {
        result.numbers[i] = result.numbers[i] >> 1 | ((result.numbers[i + 1] & 1) << (sizeof(unsigned long long) * 8 - 1));
    }
    result.numbers.back() >>= 1;
    return result;
}
LongNumber LongNumber::operator<<(const bool& doShift) const {
    LongNumber result = *this;
    if (!doShift)
        return result;
    bool carry = false;
    for (size_t i = 0; i < result.numbers.size(); i++) {
        bool new_carry = (result.numbers[i] & (1ull << (sizeof(unsigned long long) * 8 - 1))) != 0;
        result.numbers[i] = result.numbers[i] << 1 | (carry ? 1 : 0);
        carry = new_carry;
    }
    if (carry)
        throw std::overflow_error("Shift left generated an overflow");
    return result;
}

LongNumber::LongNumber(size_t precision) {
    this->precision = precision;
    numbers.assign(precision + 1, 0);
    sign = false;
}
LongNumber::LongNumber(const LongNumber& longnumber) {
    this->precision = longnumber.precision;
    this->numbers = longnumber.numbers;
    this->sign = longnumber.sign;
}
LongNumber::~LongNumber() {
    numbers.clear();
}

bool LongNumber::operator!() {
    for (auto t : numbers)
        if (t != 0)
            return false;
    return true;
}

void LongNumber::set_precision(size_t target) {
    if (precision == target)
        return;
    auto head = numbers.back();
    numbers.pop_back();
    std::reverse(numbers.begin(), numbers.end());
    while (numbers.size() > target)
        numbers.pop_back();
    while (numbers.size() < target)
        numbers.push_back(0);
    std::reverse(numbers.begin(), numbers.end());
    numbers.push_back(head);
    precision = target;
}

LongNumber& LongNumber::operator=(const LongNumber& longnumber) {
    if (&longnumber == this)
        return *this;
    this->precision = longnumber.precision;
    this->numbers = longnumber.numbers;
    this->sign = longnumber.sign;
    return *this;
}

LongNumber LongNumber::operator-() const {
    LongNumber res = *this;
    res.sign = !res.sign;
    return res;
}

bool LongNumber::operator==(const LongNumber& longnumber) const {
    return compare(*this, longnumber) == 0;
}
bool LongNumber::operator!=(const LongNumber& longnumber) const {
    return compare(*this, longnumber) != 0;
}
bool LongNumber::operator>(const LongNumber& longnumber) const {
    return compare(*this, longnumber) == 1;
}
bool LongNumber::operator>=(const LongNumber& longnumber) const {
    return compare(*this, longnumber) != -1;
}
bool LongNumber::operator<(const LongNumber& longnumber) const {
    return compare(*this, longnumber) == -1;
}
bool LongNumber::operator<=(const LongNumber& longnumber) const {
    return compare(*this, longnumber) != 1;
}

LongNumber LongNumber::operator+(const LongNumber& longnumber) const {
    if (this->sign != longnumber.sign)
        return *this - (-longnumber);
    if (this->precision < longnumber.precision)
        return longnumber + *this;
    LongNumber result = longnumber;
    if (this->precision > result.precision)
        result.set_precision(this->precision);
    bool carry = false;
    for (size_t i = 0; i < result.numbers.size(); i++) {
        auto previous = result.numbers[i];
        result.numbers[i] += (this->numbers[i] + (carry ? 1 : 0));
        carry = (result.numbers[i] < previous);
    }
    if (carry)
        throw std::overflow_error("Addition generated an overflow");
    return result;
}
LongNumber LongNumber::operator-(const LongNumber& longnumber) const {
    if (this->sign != longnumber.sign)
        return *this + (-longnumber);
    if (this->precision < longnumber.precision)
        return -longnumber + *this;
    if (LongNumber::compare(abs(*this), abs(longnumber)) < 0)
        return -(longnumber - *this);
    LongNumber result = *this;
    LongNumber nn = longnumber;
    if (this->precision > nn.precision) {
        result.set_precision(this->precision);
        nn.set_precision(this->precision);
    }
    bool carry = false;
    for (size_t i = 0; i < result.numbers.size(); i++) {
        auto previous = result.numbers[i];
        result.numbers[i] -= (nn.numbers[i] + (carry ? 1 : 0));
        carry = (result.numbers[i] > previous);
    }
    if (carry)
        throw std::overflow_error("Subtraction generated an overflow");
    if (!result)
        result.sign = false;
    return result;
}

LongNumber LongNumber::operator*(const LongNumber& longnumber) const {
    if (this->precision < longnumber.precision)
        return longnumber * *this;
    LongNumber result = LongNumber(this->precision);

    LongNumber current = longnumber;

    if (this->precision > result.precision)
        current.set_precision(this->precision);
    LongNumber mask = *this;

    size_t n = (this->precision + 1);

    mask.sign = false;
    mask.mshl();
    current.sign = false;
    current.mshl();
    result.mshl();
    for (size_t i = 0; i < n * sizeof(unsigned long long) * 8; i++) {
        if (mask.checkbit(i))
            result = result + current;
        current = current + current;
    }
    result.mshr();
    result.sign = (this->sign != longnumber.sign);
    return result;
}
size_t LongNumber::get_precision() const {
    return precision;
}
LongNumber LongNumber::operator/(const LongNumber& longnumber) const {
    LongNumber a = *this;
    LongNumber b = longnumber;
    if (this->precision != longnumber.precision) {
        size_t target = (this->precision > longnumber.precision) ? this->precision : longnumber.precision;
        a.set_precision(target);
        b.set_precision(target);
        return a / b;
    }
    bool res_s = (a.sign != b.sign);
    a.sign = false;
    b.sign = false;
    size_t n = a.precision;
    LongNumber mask = new_number(1.0);
    mask.set_precision(n);
    LongNumber result = LongNumber(n);
    while (!b.check_shl_carry() && !mask.check_shl_carry()) {
        mask = mask << true;
        b = b << true;
    }
    while (!!b) {
        if (a >= b) {
            result = result + mask;
            a = a - b;
        }
        mask = mask >> 1;
        b = b >> true;
    }
    result.sign = res_s;
    return result;
}
