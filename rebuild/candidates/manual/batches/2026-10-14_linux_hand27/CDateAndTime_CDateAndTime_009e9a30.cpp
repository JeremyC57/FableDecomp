// CDateAndTime::CDateAndTime(const _SYSTEMTIME&) -- widen the seven calendar words (year, month, day, hour, minute, second, millisecond; wDayOfWeek at +4 is skipped) into the long members
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9A30, length 54 bytes.
// Callees: none. PDB CDateAndTime (28 bytes): long Year +0, ulong Month +4, Day +8, Hour +0xc, Minute +0x10, Second +0x14, Millisecond +0x18.
// Shape: each `movzx edx, word ptr [ecx+n]` / `mov [eax+m], edx` pair is one member initialiser widening a WORD
// of the Win32 SYSTEMTIME; `this` is kept in eax from the first instruction because the constructor returns it.
struct _SYSTEMTIME {
    unsigned short wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds;
};

struct CDateAndTime {
    long          Year;            // +0x00
    unsigned long Month;           // +0x04
    unsigned long Day;             // +0x08
    unsigned long Hour;            // +0x0c
    unsigned long Minute;          // +0x10
    unsigned long Second;          // +0x14
    unsigned long Millisecond;     // +0x18
    CDateAndTime(const _SYSTEMTIME& st);
};

CDateAndTime::CDateAndTime(const _SYSTEMTIME& st)
    : Year(st.wYear), Month(st.wMonth), Day(st.wDay), Hour(st.wHour),
      Minute(st.wMinute), Second(st.wSecond), Millisecond(st.wMilliseconds)
{
}
