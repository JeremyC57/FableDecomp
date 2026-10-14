#include <cstdio>
struct _FILETIME { unsigned long dwLowDateTime, dwHighDateTime; };
struct _SYSTEMTIME { unsigned short wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds; };
static int FileTimeToSystemTime(const _FILETIME* ft, _SYSTEMTIME* st)   // stand-in: decode the test's packed fields
{
    st->wYear = (unsigned short)(ft->dwHighDateTime >> 16); st->wMonth = (unsigned short)(ft->dwHighDateTime & 0xffff);
    st->wDayOfWeek = 3; st->wDay = (unsigned short)(ft->dwLowDateTime >> 24); st->wHour = (unsigned short)((ft->dwLowDateTime >> 16) & 0xff);
    st->wMinute = (unsigned short)((ft->dwLowDateTime >> 8) & 0xff); st->wSecond = (unsigned short)(ft->dwLowDateTime & 0xff); st->wMilliseconds = 123; return 1;
}
struct CDateAndTime {
    long Year; unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    CDateAndTime(const _FILETIME& ft) { _SYSTEMTIME st; FileTimeToSystemTime(&ft, &st); Year = st.wYear; Month = st.wMonth; Day = st.wDay; Hour = st.wHour; Minute = st.wMinute; Second = st.wSecond; Millisecond = st.wMilliseconds; }
};

int main()
{
    _FILETIME ft = { (2u << 24) | (3u << 16) | (4u << 8) | 5u, (1980u << 16) | 11u };
    CDateAndTime d(ft);
    if (d.Year != 1980 || d.Month != 11 || d.Day != 2 || d.Hour != 3 || d.Minute != 4 || d.Second != 5 || d.Millisecond != 123) { std::printf("009e9db0_TEST FAIL\n"); return 1; }
    std::printf("009e9db0_TEST PASS\n"); return 0;
}