// FormatDateTime(const _FILETIME& time, unsigned short* dateText, unsigned short* timeText, long flags) -- format a UTC file time as the user-locale date and time strings (128 wide characters each); a zero time clears both buffers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0D980, length 141 bytes, `ret 8`.
// Callees: KERNEL32 FileTimeToLocalFileTime (IAT 0x0143FE5C), FileTimeToSystemTime (IAT 0x0143FD50),
// GetUserDefaultLCID (IAT 0x0143FE58, called twice), GetDateFormatW (IAT 0x0143FDC4), GetTimeFormatW (IAT 0x0143FD2C).
// Callers: CTCInGameMenu::ConstructSaveGameFileDescription (0x0047F04C, landed: it declares this signature),
// CTCPhotojournalCapture::TakeQuickScreenshot (0x0055D201), CFrontEndManager::ConstructFileDescription (0x00595DD6);
// each passes CDateAndTime::ToFileTime()'s result in ecx and two 128-character buffers. Name descriptive.
// Shape: `__fastcall` (time in ecx, date buffer in edx -> esi, time buffer and an unused flags word on the stack);
// the zero test is two separate word compares (not one 64-bit compare); a clear of each buffer under its own null
// test, the time-buffer arm with its own epilogue, its null case jumping to the shared one; a 24-byte frame
// holding the local FILETIME then the SYSTEMTIME; GetUserDefaultLCID is a dllimport used twice (`mov esi,[imp] ;
// call esi`), evaluated last for each formatter call (VC pushes right to left).
// Leaf: ?FormatDateTime@@YIXABU_FILETIME@@PAG1J@Z
struct _FILETIME { unsigned long dwLowDateTime; unsigned long dwHighDateTime; };
struct _SYSTEMTIME {
    unsigned short wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds;
};
extern "C" __declspec(dllimport) int __stdcall FileTimeToLocalFileTime(const _FILETIME* fileTime, _FILETIME* localFileTime);   // IAT 0x0143FE5C
extern "C" __declspec(dllimport) int __stdcall FileTimeToSystemTime(const _FILETIME* fileTime, _SYSTEMTIME* systemTime);       // IAT 0x0143FD50
extern "C" __declspec(dllimport) unsigned long __stdcall GetUserDefaultLCID();                                                 // IAT 0x0143FE58
extern "C" __declspec(dllimport) int __stdcall GetDateFormatW(unsigned long locale, unsigned long flags, const _SYSTEMTIME* date,
                                                             const unsigned short* format, unsigned short* text, int size);  // IAT 0x0143FDC4
extern "C" __declspec(dllimport) int __stdcall GetTimeFormatW(unsigned long locale, unsigned long flags, const _SYSTEMTIME* time,
                                                             const unsigned short* format, unsigned short* text, int size);  // IAT 0x0143FD2C

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
    FileTimeToLocalFileTime(&time, &localTime);
    _SYSTEMTIME systemTime;
    FileTimeToSystemTime(&localTime, &systemTime);
    GetDateFormatW(GetUserDefaultLCID(), 0, &systemTime, 0, dateText, 128);
    GetTimeFormatW(GetUserDefaultLCID(), 0, &systemTime, 0, timeText, 128);
}