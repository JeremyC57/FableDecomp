// CDateAndTime::CDateAndTime(const _FILETIME&) -- convert the file time with FileTimeToSystemTime and widen the seven calendar words into the members
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9DB0, length 86 bytes.
// Callees: KERNEL32 FileTimeToSystemTime (IAT 0x0143FD50). PDB CDateAndTime (28 bytes): long Year +0, ulong Month +4 .. Millisecond +0x18.
// Shape: identical to Initialise @ 0x009E9CB0 plus the constructor's `mov eax,esi` return of `this`: a 16-byte
// SYSTEMTIME local, the import through its IAT slot, the seven `movzx` widenings (wDayOfWeek skipped) as
// member initialisers.
struct _FILETIME { unsigned long dwLowDateTime; unsigned long dwHighDateTime; };
struct _SYSTEMTIME {
    unsigned short wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds;
};
extern "C" __declspec(dllimport) int __stdcall FileTimeToSystemTime(const _FILETIME* fileTime, _SYSTEMTIME* systemTime);   // IAT 0x0143FD50

class CDateAndTime { public:
    long          Year;            // +0x00
    unsigned long Month;           // +0x04
    unsigned long Day;             // +0x08
    unsigned long Hour;            // +0x0c
    unsigned long Minute;          // +0x10
    unsigned long Second;          // +0x14
    unsigned long Millisecond;     // +0x18
    CDateAndTime(const _FILETIME& fileTime);
};

CDateAndTime::CDateAndTime(const _FILETIME& fileTime)
{
    _SYSTEMTIME st;
    FileTimeToSystemTime(&fileTime, &st);
    Year        = st.wYear;
    Month       = st.wMonth;
    Day         = st.wDay;
    Hour        = st.wHour;
    Minute      = st.wMinute;
    Second      = st.wSecond;
    Millisecond = st.wMilliseconds;
}