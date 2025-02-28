#include <iostream>
#include <vector>
class LongNumber {
private:
    std::vector<unsigned long long> numbers;
    size_t precision;
    bool sign;
    void mshr();
    void mshl();
    inline bool checkbit(size_t k);
    bool check_shl_carry();
    static int compare(const LongNumber& a, const LongNumber& b);
public:
    bool get_sign() const;
    size_t get_precision() const;
    unsigned long long get_int() const;
    LongNumber operator>>(const bool& doShift) const;
    LongNumber operator<<(const bool& doShift) const;
    static LongNumber abs(const LongNumber& longnumber);
    static LongNumber new_number(long double ld);

    LongNumber(size_t precision);
    LongNumber(const LongNumber& longnumber);
    ~LongNumber();

    bool operator!();

    void set_precision(size_t target);
    LongNumber& operator=(const LongNumber& longnumber);

    LongNumber operator-() const;
    bool operator==(const LongNumber& longnumber) const;
    bool operator!=(const LongNumber& longnumber) const;
    bool operator>(const LongNumber& longnumber) const;
    bool operator>=(const LongNumber& longnumber) const;
    bool operator<(const LongNumber& longnumber) const;
    bool operator<=(const LongNumber& longnumber) const;

    LongNumber operator+(const LongNumber& longnumber) const;
    LongNumber operator-(const LongNumber& longnumber) const;
    LongNumber operator*(const LongNumber& longnumber) const;
    LongNumber operator/(const LongNumber& longnumber) const;
};

LongNumber operator ""_longnum(long double);
