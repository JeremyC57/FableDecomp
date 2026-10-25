// CSegmentFile::SetRange(unsigned long first, unsigned long count) -- select the byte range [first, first+count) of the underlying buffered file as the current segment (seeking to its start); clears the range and fails (-1) when it does not fit or the seek fails, else 0
// Leaf: ?SetRange@CSegmentFile@@QAEJKK@Z
// Retail: Fable.exe @ 0x00C3AA50, 96 bytes, `ret 8`.
// Naming (sweep1, 2026-10-25): was landed as CWorld::SetDrawEnvironmentBounds (a propagated manifest label; the
//   landed source already noted it is not the game's CWorld); descriptive (no PDB covers libpk). FINDINGS chunk 436: it
//   is the segment file's SetRange, called by the landed CSegmentFile::OpenSegment 0x00C2D9C0 (0x00C2D9EB) with a
//   segment table entry's {start, length} and by 0x00C2DA91 / 0x00C2DAA5 / 0x00C2DAD0 / 0x00C2DB0C / 0x00C2DBBB. The
//   callee 0x00C4AA40 is the landed PKCBufferedReader Seek(offset) (false past the size at +0x20, the same word
//   0x00C4AA30 returns), so the bounds are the file size.
// Callees (out of line in retail): GetSize() 0x00C4AA30 (returns +0x20), ClearRange() 0x00C3A9C0 (zeroes
//   +0x70/+0x74/+0x78), Seek(unsigned long offset) 0x00C4AA40.
// Members: +0x70 range first, +0x74 range count, +0x78 range cursor.
// Shape: three separate `ClearRange(); return -1;` exits (the range test and the seek test are separate ifs, whose
// tails VC7.1 merges -- an `||` condition puts the failure block after the success block); success stores count,
// first, then zero.
struct CSegmentFile {
    unsigned char _pad[0x70];
    unsigned long RangeFirst;                   // +0x70
    unsigned long RangeCount;                   // +0x74
    unsigned long RangeCursor;                  // +0x78
    unsigned long GetSize() const;              // 0x00C4AA30
    void ClearRange();                          // 0x00C3A9C0
    bool Seek(unsigned long offset);            // 0x00C4AA40
    long SetRange(unsigned long first, unsigned long count);
};

long CSegmentFile::SetRange(unsigned long first, unsigned long count)
{
    if (first > GetSize())
    {
        ClearRange();
        return -1;
    }
    if (first + count > GetSize())
    {
        ClearRange();
        return -1;
    }
    if (!Seek(first))
    {
        ClearRange();
        return -1;
    }
    RangeCount = count;
    RangeFirst = first;
    RangeCursor = 0;
    return 0;
}
