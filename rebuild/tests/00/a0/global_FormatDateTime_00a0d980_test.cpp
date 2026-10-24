// Behaviour test for 0x00A0D980 FormatDateTime: a zero file time clears both buffers; otherwise local time -> system time -> date and time text (128 chars, user locale).
#include <cstdio>
#if !defined(_MSC_VER)
#define __fastcall
#endif

struct _FILETIME { unsigned long dwLowDateTime; unsigned long dwHighDateTime; };
struct _SYSTEMTIME {
    unsigned short wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds;
};
static int g_calls = 0;
static int Stub_FileTimeToLocalFileTime(const _FILETIME* t, _FILETIME* l) { ++g_calls; l->dwLowDateTime = t->dwLowDateTime + 1; l->dwHighDateTime = t->dwHighDateTime; return 1; }
static int Stub_FileTimeToSystemTime(const _FILETIME* t, _SYSTEMTIME* s) { ++g_calls; s->wDay = (unsigned short)t->dwLowDateTime; s->wHour = 7; return 1; }
static unsigned long Stub_GetUserDefaultLCID() { return 0x0409; }
static int Stub_GetDateFormatW(unsigned long locale, unsigned long flags, const _SYSTEMTIME* d, const unsigned short* fmt, unsigned short* text, int size)
{
    ++g_calls;
    if (locale != 0x0409 || flags != 0 || fmt != 0 || size != 128) return 0;
    text[0] = (unsigned short)('A' + d->wDay); text[1] = 0;
    return 2;
}
static int Stub_GetTimeFormatW(unsigned long locale, unsigned long flags, const _SYSTEMTIME* t, const unsigned short* fmt, unsigned short* text, int size)
{
    ++g_calls;
    if (locale != 0x0409 || flags != 0 || fmt != 0 || size != 128) return 0;
    text[0] = (unsigned short)('0' + t->wHour); text[1] = 0;
    return 2;
}

void __fastcall FormatDateTime(const _FILETIME& time, unsigned short* dateText, unsigned short* timeText, long flags)
{
    if (time.dwLowDateTime == 0 && time.dwHighDateTime == 0) {
        if (dateText)
            *dateText = 0;
        if (timeText)
            *timeText = 0;
        return;
    }
    _FILETIME localTime;
    Stub_FileTimeToLocalFileTime(&time, &localTime);
    _SYSTEMTIME systemTime;
    Stub_FileTimeToSystemTime(&localTime, &systemTime);
    Stub_GetDateFormatW(Stub_GetUserDefaultLCID(), 0, &systemTime, 0, dateText, 128);
    Stub_GetTimeFormatW(Stub_GetUserDefaultLCID(), 0, &systemTime, 0, timeText, 128);
    (void)flags;
}

int main()
{
    unsigned short date[128], tm[128];
    date[0] = 'x'; tm[0] = 'y';
    _FILETIME zero = { 0, 0 };
    FormatDateTime(zero, date, tm, 0);
    if (date[0] != 0 || tm[0] != 0 || g_calls != 0) { std::printf("00a0d980_TEST FAIL\n"); return 1; }
    FormatDateTime(zero, 0, 0, 0);                         // null buffers tolerated
    _FILETIME hi = { 0, 1 };
    FormatDateTime(hi, date, tm, 0);                      // a non-zero high word alone is a real time
    if (g_calls != 4 || date[0] != 'B' || tm[0] != '7') { std::printf("00a0d980_TEST FAIL\n"); return 1; }
    _FILETIME t = { 2, 0 };
    FormatDateTime(t, date, tm, 1);
    if (g_calls != 8 || date[0] != 'D' || date[1] != 0 || tm[0] != '7') { std::printf("00a0d980_TEST FAIL\n"); return 1; }
    std::printf("00a0d980_TEST PASS\n");
    return 0;
}