#include <cstdio>
struct _SYSTEMTIME { unsigned short wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds; };
class CDateAndTime { public:
    long Year; unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    CDateAndTime(const _SYSTEMTIME& st) : Year(st.wYear), Month(st.wMonth), Day(st.wDay), Hour(st.wHour), Minute(st.wMinute), Second(st.wSecond), Millisecond(st.wMilliseconds) { }
};

int main()
{
    _SYSTEMTIME st = { 2005, 9, 3, 14, 23, 59, 7, 999 };
    CDateAndTime d(st);
    if (d.Year != 2005 || d.Month != 9 || d.Day != 14 || d.Hour != 23 || d.Minute != 59 || d.Second != 7 || d.Millisecond != 999) { std::printf("009e9a30_TEST FAIL\n"); return 1; }
    std::printf("009e9a30_TEST PASS\n"); return 0;
}