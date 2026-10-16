// CWorld::SetDrawEnvironmentBounds(unsigned long, unsigned long) -- steering-library world: select a [first, first+count) range of its elements, clearing the selection and failing (-1) when out of range
// Retail: Fable.exe @ 0x00C3AA50, 96 bytes, `ret 8`. Manifest label "CWorld::SetDrawEnvironmentBounds" (unknown
// convention); like 0x00C1C620 this is the AI steering library's world class, not the game's CWorld.
// Callees (out of line in retail): GetCount() 0x00C4AA30 (returns +0x20; label "CWorld::DrawGetEnvironment"),
// ClearRange() 0x00C3A9C0 (zeroes +0x70/+0x74/+0x78; label "CV_MurderTwistScript::Init" is a byte twin),
// IsValidIndex(unsigned long) 0x00C4AA40 (label "PKCBufferedReader_ConvertByteOffset").
// Members: +0x70 range first, +0x74 range count, +0x78 range cursor.
// Shape: three separate `ClearRange(); return -1;` exits (the range test and the index test are separate ifs, whose
// tails VC7.1 merges -- an `||` condition puts the failure block after the success block); success stores count,
// first, then zero.
struct CWorld {
    unsigned char _pad[0x70];
    unsigned long RangeFirst;                   // +0x70
    unsigned long RangeCount;                   // +0x74
    unsigned long RangeCursor;                  // +0x78
    unsigned long GetCount() const;             // 0x00C4AA30
    void ClearRange();                          // 0x00C3A9C0
    bool IsValidIndex(unsigned long index) const;   // 0x00C4AA40
    long SetDrawEnvironmentBounds(unsigned long first, unsigned long count);
};

long CWorld::SetDrawEnvironmentBounds(unsigned long first, unsigned long count)
{
    if (first > GetCount())
    {
        ClearRange();
        return -1;
    }
    if (first + count > GetCount())
    {
        ClearRange();
        return -1;
    }
    if (!IsValidIndex(first))
    {
        ClearRange();
        return -1;
    }
    RangeCount = count;
    RangeFirst = first;
    RangeCursor = 0;
    return 0;
}
