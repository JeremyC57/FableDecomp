// CDateAndTime::operator<(const CDateAndTime&) const -- lexicographic "earlier than" over year (signed), then the six unsigned fields
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9B80, length 86 bytes.
// Callees: none. PDB CDateAndTime (28 bytes): long Year +0, ulong Month +4, Day +8, Hour +0xc, Minute +0x10, Second +0x14, Millisecond +0x18.
// Shape: the year is a signed long (`setl`); every later field is unsigned, so its `<` is the carry idiom
// `sbb eax,eax ; neg eax`, and VC merges the six identical tails into one block entered by the `jne` of the
// field that differed (the `cmp` before each `jne` still holds the flags). The Millisecond compare falls into it.
struct CDateAndTime {
    long          Year;            // +0x00
    unsigned long Month;           // +0x04
    unsigned long Day;             // +0x08
    unsigned long Hour;            // +0x0c
    unsigned long Minute;          // +0x10
    unsigned long Second;          // +0x14
    unsigned long Millisecond;     // +0x18
    bool operator<(const CDateAndTime& other) const;
};

bool CDateAndTime::operator<(const CDateAndTime& other) const
{
    if (Year != other.Year)
        return Year < other.Year;
    if (Month != other.Month)
        return Month < other.Month;
    if (Day != other.Day)
        return Day < other.Day;
    if (Hour != other.Hour)
        return Hour < other.Hour;
    if (Minute != other.Minute)
        return Minute < other.Minute;
    if (Second != other.Second)
        return Second < other.Second;
    return Millisecond < other.Millisecond;
}
