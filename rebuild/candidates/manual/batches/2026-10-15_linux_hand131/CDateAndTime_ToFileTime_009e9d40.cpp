// CDateAndTime::ToFileTime() const -- convert the date and time to a Win32 FILETIME through a SYSTEMTIME (day of week 0)
// Leaf: ToFileTime
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9D40, length 109 bytes (`ret 4`: the hidden result pointer).
// Ego_r.pdb: `?ToFileTime@CDateAndTime@@QBE?AU_FILETIME@@XZ`; CDateAndTime = {long Year; ulong Month, Day, Hour, Minute,
// Second, Millisecond} (28 bytes).
// Callees: kernel32!SystemTimeToFileTime through the import slot 0x0143FD4C.
// Shape: a member function returns even an 8-byte POD struct through the hidden pointer; the SYSTEMTIME is filled field by
// field (word stores, wDayOfWeek = 0 as an immediate) in declaration order, the FILETIME local is copied out word by word.
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef int BOOL;
typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME;
typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME;
extern "C" __declspec(dllimport) BOOL __stdcall SystemTimeToFileTime(const SYSTEMTIME* lpSystemTime, FILETIME* lpFileTime);

struct CDateAndTime {
    long          Year;                     // +0x00
    unsigned long Month;                    // +0x04
    unsigned long Day;                      // +0x08
    unsigned long Hour;                     // +0x0c
    unsigned long Minute;                   // +0x10
    unsigned long Second;                   // +0x14
    unsigned long Millisecond;              // +0x18
    _FILETIME ToFileTime() const;
};

_FILETIME CDateAndTime::ToFileTime() const
{
    SYSTEMTIME systemTime;
    systemTime.wYear = (WORD)Year;
    systemTime.wMonth = (WORD)Month;
    systemTime.wDay = (WORD)Day;
    systemTime.wHour = (WORD)Hour;
    systemTime.wMinute = (WORD)Minute;
    systemTime.wSecond = (WORD)Second;
    systemTime.wDayOfWeek = 0;
    systemTime.wMilliseconds = (WORD)Millisecond;
    FILETIME fileTime;
    SystemTimeToFileTime(&systemTime, &fileTime);
    return fileTime;
}
