// CDateAndTime::operator!=(const CDateAndTime&) const -- `!(*this == other)` with the long-valued operator== inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9B20, length 87 bytes.
// Callees: none (operator== @ 0x009E9AD0 inlined). PDB CDateAndTime (28 bytes): long Year +0, ulong Month +4 .. Millisecond +0x18.
// Shape: the inlined comparison materialises its long result (`mov eax,1` / `xor eax,eax`) in both arms, then each
// arm applies the `!` as `xor edx,edx ; test al,al ; sete dl ; mov al,dl` -- a bool built from the low byte.
// The manifest label says operator==; the negation makes it operator!=.
struct CDateAndTime {
    long          Year;            // +0x00
    unsigned long Month;           // +0x04
    unsigned long Day;             // +0x08
    unsigned long Hour;            // +0x0c
    unsigned long Minute;          // +0x10
    unsigned long Second;          // +0x14
    unsigned long Millisecond;     // +0x18
    bool operator==(const CDateAndTime& other) const
    {
        return Year == other.Year && Month == other.Month && Day == other.Day && Hour == other.Hour &&
               Minute == other.Minute && Second == other.Second && Millisecond == other.Millisecond;
    }
    bool operator!=(const CDateAndTime& other) const;
};

bool CDateAndTime::operator!=(const CDateAndTime& other) const
{
    return !(*this == other);
}