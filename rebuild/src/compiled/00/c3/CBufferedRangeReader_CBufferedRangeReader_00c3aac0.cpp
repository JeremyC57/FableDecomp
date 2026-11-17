// CBufferedRangeReader::CBufferedRangeReader(void* source, unsigned long size) -- construct the buffered reader member (0x00C4B300) on
//   the source with a {size, 0, 2} parameter record, then reset the range words (Reset 0x00C3A9C0)
// Leaf: ??0CBufferedRangeReader@@QAE@PAXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3AAC0, length 63 bytes, `ret 8`, `this` returned in eax.
// Identity (NF59, round 7, queue q10528): libpk ships no symbols; libpk/CSegmentFile.h names this constructor (descriptive,
//   "native C3AAC0 constructs PKCBufferedReader at 0") and its landed caller 0x00C2DA20 binds ??0CBufferedRangeReader@@QAE@PAXK@Z.
//   Re-authored on that header: the landed row was the address-named stand-in CBufferedReader_00c3aac0 deriving a local
//   CReaderBase_00c4b300 (= the header's PKCBufferedReader member Reader, ctor 0x00C4B300 (source, const PKCBufferedReaderParams&))
//   and bound the reset as ?Reset@CReaderBase_00c4b300@@QAEXXZ.
// Shape: a 12-byte frame for the parameter record (filled after its address and the source are pushed).
#define FABLE_RETAIL_CSEGMENTFILE_PASS23
#define FABLE_RETAIL_CBUFFEREDRANGEREADER_RESET
#include "libpk/CSegmentFile.h"

CBufferedRangeReader::CBufferedRangeReader(void* source, unsigned long size)
    : Reader(source, PKCBufferedReaderParams(size, 0, 2))
{
    Reset();
}
