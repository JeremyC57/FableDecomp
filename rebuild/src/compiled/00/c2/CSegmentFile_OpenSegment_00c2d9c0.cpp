// CSegmentFile::OpenSegment(const CString& name) -- look the named segment up in the segment table (+0x80) and select its byte range (first word = start, second = length) as the file's current range; 0 on success, -1 when the segment is unknown or the range does not fit
// Leaf: ?OpenSegment@CSegmentFile@@QAEJABVCString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D9C0, length 64 bytes (62 + int3 padding), __fastcall, `ret 4`.
// Callers (10): the libpk bank code 0x00C16C53 / 0x00C16C7A / 0x00C16E57 / 0x00C17078 / 0x00C170F0 (chunk 433's
//   HasBankSegments / ReadBankInfoCount / OpenBankFile "OpenSegment" calls), 0x00C30ED3, 0x00C30EFA, 0x00C310B1, 0x00C3BB96,
//   0x00C3BCE5.
// Callees: the segment table's virtual Find (slot 17, +0x44; the name reference passed straight through), CBufferedRangeReader::SetRange(first, count) @ 0x00C3AA50 (on the Reader member at +0)
//   (landed as "CWorld::SetDrawEnvironmentBounds": -1 and the range cleared when it does not fit, else 0).
// Parameter (sweep1 fix, FINDINGS chunk 437): every caller builds a CString temporary from a segment-name literal with
//   CString::CString(const char*) 0x00C0BFE0 and passes its address (0x00C16C29..0x00C16C53 "LHAudioBankLookupTable",
//   0x00C16C5C..0x00C16C7A "LHAudioBankCompData", 0x00C30EA9 "LHAudioWaveData", 0x00C30EDC "LHAudioBankSampleTable"),
//   so the argument is `const CString&`, not an id; the bytes are the same.
// Name: libpk, no PDB; descriptive (the catalog's `Accessor_GetAndSetEnvironmentBounds` is propagated).
// Shape: the miss returns `or eax,-1` with its own epilogue; the range result maps to 0 / -1 through
//   `xor ecx,ecx ; test eax,eax ; setge cl ; dec ecx`.
// Name (round 7, NF53): the range select 0x00C3AA50 is the descriptive ?SetRange@CBufferedRangeReader@@QAEJKK@Z of libpk/CSegmentFile.h
//   (it reads and writes RangeStart +0x70 / RangeLength +0x74 / ReadOffset +0x78, the reader's own fields; the reader is CSegmentFile's first
//   member, so `this` is unchanged); the table is CSegmentFile::Segments (CSegmentList +0x80, FindByKey slot 17). Was a local SSegment /
//   CString / CSegmentTable / CSegmentFile model that bound ?SetRange@CSegmentFile@@QAEJKK@Z. Descriptive libpk names (no libpk PDB):
//   one address, one name with the header.
#define FABLE_RETAIL_CSEGMENTFILE_PASS23
#define FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#include "libpk/CSegmentFile.h"

long CSegmentFile::OpenSegment(const CString& name)
{
    CSegment* segment = Segments.FindByKey(name);
    if (!segment)
        return -1;
    return Reader.SetRange(segment->Offset, segment->Size) >= 0 ? 0 : -1;
}
