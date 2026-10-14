#include <cstdio>
struct CDateAndTime {
    long Year; unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    long operator==(const CDateAndTime& o) const { return Year == o.Year && Month == o.Month && Day == o.Day && Hour == o.Hour && Minute == o.Minute && Second == o.Second && Millisecond == o.Millisecond; }
};

int main()
{
    CDateAndTime a = { 2005, 1, 2, 3, 4, 5, 6 }, b = a, c = a; c.Millisecond = 7;
    if ((a == b) != 1 || (a == c) != 0) { std::printf("009e9ad0_TEST FAIL\n"); return 1; }
    c = a; c.Year = 2004; if (a == c) { std::printf("009e9ad0_TEST FAIL\n"); return 1; }
    std::printf("009e9ad0_TEST PASS\n"); return 0;
}
