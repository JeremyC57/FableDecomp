// CBufferedRangeReader::SetRange(unsigned long first, unsigned long count) -- select the byte range [first, first+count) of the underlying buffered file as the current range (seeking to its start); clears the range and fails (-1) when it does not fit or the seek fails.
// Leaf: ?SetRange@CBufferedRangeReader@@QAEJKK@Z
// Retail: Fable.exe @ 0x00C3AA50, 96 bytes, `ret 8`.
// Identity (NF56, round 7, queue q10480): libpk/CSegmentFile.h declares ?SetRange@CBufferedRangeReader@@QAEJKK@Z, and the members this
//   body writes (+0x70 / +0x74 / +0x78) are CBufferedRangeReader's RangeStart / RangeLength / ReadOffset (sizeof 0x7C, the first member
//   of CSegmentFile, so the OpenSegment caller's `this` is the same pointer); the landed caller 0x00C2DA20 already binds this spelling.
//   Was CSegmentFile::SetRange over a local CSegmentFile model (a class with a shared header). Descriptive libpk names (no libpk PDB):
//   the header's. Callees: PKCBufferedReader::GetSize 0x00C4AA30 (returns +0x20) and Seek(unsigned long) 0x00C4AA40 on the Reader member
//   (overlay opt-in FABLE_RETAIL_PKCBUFFEREDREADER_GETSIZE_SEEK, the landed binders' spellings), and the range Reset 0x00C3A9C0 (overlay
//   opt-in FABLE_RETAIL_CBUFFEREDRANGEREADER_RESET; the landed self row's name).
// Shape: three separate `Reset(); return -1;` exits (the range test and the seek test are separate ifs, whose tails VC7.1 merges -- an
//   `||` condition puts the failure block after the success block); success stores count, first, then zero.
#define FABLE_RETAIL_CSEGMENTFILE_PASS23
#define FABLE_RETAIL_PKCBUFFEREDREADER_GETSIZE_SEEK
#define FABLE_RETAIL_CBUFFEREDRANGEREADER_RESET
#include "libpk/CSegmentFile.h"

long CBufferedRangeReader::SetRange(unsigned long first, unsigned long count)
{
    if (first > Reader.GetSize())
    {
        Reset();
        return -1;
    }
    if (first + count > Reader.GetSize())
    {
        Reset();
        return -1;
    }
    if (!Reader.Seek(first))
    {
        Reset();
        return -1;
    }
    RangeLength = count;
    RangeStart = first;
    ReadOffset = 0;
    return 0;
}
