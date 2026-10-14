// CDateAndTime::CDateAndTime(long year, unsigned long month, unsigned long day, unsigned long hour, unsigned long minute, unsigned long second, unsigned long millisecond) -- seven-argument member-wise constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9A70, length 51 bytes.
// Callees: none. PDB CDateAndTime (28 bytes): long Year +0, ulong Month +4, Day +8, Hour +0xc, Minute +0x10, Second +0x14, Millisecond +0x18.
// Shape: the argument loads alternate between ecx and edx (the compiler software-pipelines the copy); `ret 1Ch` = seven dwords.
struct CDateAndTime {
    long          Year;            // +0x00
    unsigned long Month;           // +0x04
    unsigned long Day;             // +0x08
    unsigned long Hour;            // +0x0c
    unsigned long Minute;          // +0x10
    unsigned long Second;          // +0x14
    unsigned long Millisecond;     // +0x18
    CDateAndTime(long year, unsigned long month, unsigned long day, unsigned long hour,
                 unsigned long minute, unsigned long second, unsigned long millisecond);
};

CDateAndTime::CDateAndTime(long year, unsigned long month, unsigned long day, unsigned long hour,
                           unsigned long minute, unsigned long second, unsigned long millisecond)
    : Year(year), Month(month), Day(day), Hour(hour), Minute(minute), Second(second), Millisecond(millisecond)
{
}