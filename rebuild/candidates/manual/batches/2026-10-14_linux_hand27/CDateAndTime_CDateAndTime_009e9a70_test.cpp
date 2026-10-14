#include <cstdio>
struct CDateAndTime {
    long Year; unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    CDateAndTime(long y, unsigned long mo, unsigned long d, unsigned long h, unsigned long mi, unsigned long s, unsigned long ms) : Year(y), Month(mo), Day(d), Hour(h), Minute(mi), Second(s), Millisecond(ms) { }
};

int main()
{
    CDateAndTime d(1999, 12, 31, 23, 58, 57, 56);
    if (d.Year != 1999 || d.Month != 12 || d.Day != 31 || d.Hour != 23 || d.Minute != 58 || d.Second != 57 || d.Millisecond != 56) { std::printf("009e9a70_TEST FAIL\n"); return 1; }
    std::printf("009e9a70_TEST PASS\n"); return 0;
}
