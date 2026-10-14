#include <cstdio>
struct CDateAndTime {
    long Year; unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    bool operator==(const CDateAndTime& o) const { return Year == o.Year && Month == o.Month && Day == o.Day && Hour == o.Hour && Minute == o.Minute && Second == o.Second && Millisecond == o.Millisecond; }
    bool operator!=(const CDateAndTime& o) const { return !(*this == o); }
};

int main()
{
    CDateAndTime a = { 2005, 1, 2, 3, 4, 5, 6 }, b = a, c = a; c.Second = 9;
    if ((a != b) || !(a != c)) { std::printf("009e9b20_TEST FAIL\n"); return 1; }
    std::printf("009e9b20_TEST PASS\n"); return 0;
}