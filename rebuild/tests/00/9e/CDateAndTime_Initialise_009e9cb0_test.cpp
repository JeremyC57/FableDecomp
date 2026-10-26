#include <cstdio>
struct _FILETIME { unsigned long dwLowDateTime, dwHighDateTime; };
struct _SYSTEMTIME { unsigned short wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds; };
static int FileTimeToSystemTime(const _FILETIME* ft, _SYSTEMTIME* st)   // stand-in: decode the test's packed fields
{
    st->wYear = (unsigned short)(ft->dwHighDateTime >> 16); st->wMonth = (unsigned short)(ft->dwHighDateTime & 0xffff);
    st->wDayOfWeek = 3; st->wDay = (unsigned short)(ft->dwLowDateTime >> 24); st->wHour = (unsigned short)((ft->dwLowDateTime >> 16) & 0xff);
    st->wMinute = (unsigned short)((ft->dwLowDateTime >> 8) & 0xff); st->wSecond = (unsigned short)(ft->dwLowDateTime & 0xff); st->wMilliseconds = 123; return 1;
}
class CDateAndTime { public:
    long Year; unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    void Initialise(const _FILETIME& ft) { _SYSTEMTIME st; FileTimeToSystemTime(&ft, &st); Year = st.wYear; Month = st.wMonth; Day = st.wDay; Hour = st.wHour; Minute = st.wMinute; Second = st.wSecond; Millisecond = st.wMilliseconds; }
};

int main()
{
    _FILETIME ft = { (21u << 24) | (13u << 16) | (45u << 8) | 9u, (2005u << 16) | 7u };
    CDateAndTime d = { 0, 0, 0, 0, 0, 0, 0 }; d.Initialise(ft);
    if (d.Year != 2005 || d.Month != 7 || d.Day != 21 || d.Hour != 13 || d.Minute != 45 || d.Second != 9 || d.Millisecond != 123) { std::printf("009e9cb0_TEST FAIL\n"); return 1; }
    std::printf("009e9cb0_TEST PASS\n"); return 0;
}