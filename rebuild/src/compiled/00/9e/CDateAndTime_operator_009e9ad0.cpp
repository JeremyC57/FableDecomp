// CDateAndTime::operator==(const CDateAndTime&) const -- all seven calendar fields equal (long result 1/0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9AD0, length 71 bytes.
// Callees: none. PDB CDateAndTime (28 bytes): long Year +0, ulong Month +4, Day +8, Hour +0xc, Minute +0x10, Second +0x14, Millisecond +0x18.
// Shape: a short-circuit && chain, every mismatch jumping to the shared `xor eax,eax` tail; the true result is
// `mov eax,1` (a long/int result, not the `mov al,1` of a bool). The sibling operator!= @ 0x009E9B20 inlines this body.
struct CDateAndTime {
    long          Year;            // +0x00
    unsigned long Month;           // +0x04
    unsigned long Day;             // +0x08
    unsigned long Hour;            // +0x0c
    unsigned long Minute;          // +0x10
    unsigned long Second;          // +0x14
    unsigned long Millisecond;     // +0x18
    long operator==(const CDateAndTime& other) const;
};

long CDateAndTime::operator==(const CDateAndTime& other) const
{
    return Year == other.Year && Month == other.Month && Day == other.Day && Hour == other.Hour &&
           Minute == other.Minute && Second == other.Second && Millisecond == other.Millisecond;
}