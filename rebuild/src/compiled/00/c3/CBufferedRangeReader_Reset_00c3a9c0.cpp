// CBufferedRangeReader::Reset() -- clear the reader's range: RangeStart, RangeLength and ReadOffset (+0x70 / +0x74 / +0x78) to zero
// Leaf: ?Reset@CBufferedRangeReader@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3A9C0, three dword stores of zero, `ret`.
// Identity (NF59, round 7, queue q10528): libpk ships no symbols; descriptive name (NF56 q10480) kept from the landed method name
//   Reset and moved from the address-named stand-in class CReaderBase_00c4b300 onto libpk/CSegmentFile.h's CBufferedRangeReader
//   (opt-in FABLE_RETAIL_CBUFFEREDRANGEREADER_RESET: the range words RangeStart / RangeLength / ReadOffset +0x70/+0x74/+0x78 the
//   body zeroes). The landed SetRange 0x00C3AA50 already binds ?Reset@CBufferedRangeReader@@QAEXXZ.
#define FABLE_RETAIL_CSEGMENTFILE_PASS23
#define FABLE_RETAIL_CBUFFEREDRANGEREADER_RESET
#include "libpk/CSegmentFile.h"

void CBufferedRangeReader::Reset()
{
    RangeStart = 0;
    RangeLength = 0;
    ReadOffset = 0;
}
