// Behaviour test for 0x009E9D40 CDateAndTime::ToFileTime: the fields land in the SYSTEMTIME words (day of week 0) handed
// to the conversion, whose FILETIME is returned (a stub conversion packs the fields so the mapping is visible).
#include <cstdio>
struct SYSTEMTIME_T { unsigned short wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds; };
struct FILETIME_T { unsigned int dwLowDateTime, dwHighDateTime; };
static int StubSystemTimeToFileTime(const SYSTEMTIME_T* st, FILETIME_T* ft)
{
    ft->dwHighDateTime = ((unsigned int)st->wYear << 16) | ((unsigned int)st->wMonth << 8) | st->wDay;
    ft->dwLowDateTime = ((unsigned int)st->wHour << 24) | ((unsigned int)st->wMinute << 16) | ((unsigned int)st->wSecond << 8)
                      | ((unsigned int)st->wDayOfWeek << 4) | (st->wMilliseconds & 0xf);
    return 1;
}
struct CDateAndTime {
    int Year; unsigned int Month, Day, Hour, Minute, Second, Millisecond;
    FILETIME_T ToFileTime() const
    {
        SYSTEMTIME_T s;
        s.wYear = (unsigned short)Year; s.wMonth = (unsigned short)Month; s.wDay = (unsigned short)Day;
        s.wHour = (unsigned short)Hour; s.wMinute = (unsigned short)Minute; s.wSecond = (unsigned short)Second;
        s.wDayOfWeek = 0; s.wMilliseconds = (unsigned short)Millisecond;
        FILETIME_T f; StubSystemTimeToFileTime(&s, &f);
        return f;
    }
};
int main()
{
    CDateAndTime t; t.Year = 2005; t.Month = 9; t.Day = 20; t.Hour = 13; t.Minute = 45; t.Second = 30; t.Millisecond = 7;
    FILETIME_T f = t.ToFileTime();
    if (f.dwHighDateTime != ((2005u << 16) | (9u << 8) | 20u) || f.dwLowDateTime != ((13u << 24) | (45u << 16) | (30u << 8) | 7u))
        { std::printf("009e9d40_TEST FAIL\n"); return 1; }
    std::printf("009e9d40_TEST PASS\n");
    return 0;
}
