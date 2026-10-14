#include <cstdio>
struct CDateAndTime {
    long Year; unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    bool operator>(const CDateAndTime& o) const {
        if (Year != o.Year) return Year > o.Year; if (Month != o.Month) return Month > o.Month; if (Day != o.Day) return Day > o.Day;
        if (Hour != o.Hour) return Hour > o.Hour; if (Minute != o.Minute) return Minute > o.Minute; if (Second != o.Second) return Second > o.Second;
        return Millisecond > o.Millisecond; }
};

int main()
{
    CDateAndTime a = { 2005, 1, 2, 3, 4, 5, 6 }, b = a;
    if (a > b || b > a) { std::printf("009e9be0_TEST FAIL\n"); return 1; }
    b.Millisecond = 7; if ((a > b) || !(b > a)) { std::printf("009e9be0_TEST FAIL\n"); return 1; }
    b = a; b.Year = -1; if (!(a > b)) { std::printf("009e9be0_TEST FAIL\n"); return 1; }
    b = a; b.Day = 1; b.Month = 2; if (!(b > a)) { std::printf("009e9be0_TEST FAIL\n"); return 1; }
    std::printf("009e9be0_TEST PASS\n"); return 0;
}
