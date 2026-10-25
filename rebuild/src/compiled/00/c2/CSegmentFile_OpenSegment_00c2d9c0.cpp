// CSegmentFile::OpenSegment(const CString& name) -- look the named segment up in the segment table (+0x80) and select its byte range (first word = start, second = length) as the file's current range; 0 on success, -1 when the segment is unknown or the range does not fit
// Leaf: ?OpenSegment@CSegmentFile@@QAEJABVCString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D9C0, length 64 bytes (62 + int3 padding), __fastcall, `ret 4`.
// Callers (10): the libpk bank code 0x00C16C53 / 0x00C16C7A / 0x00C16E57 / 0x00C17078 / 0x00C170F0 (chunk 433's
//   HasBankSegments / ReadBankInfoCount / OpenBankFile "OpenSegment" calls), 0x00C30ED3, 0x00C30EFA, 0x00C310B1, 0x00C3BB96,
//   0x00C3BCE5.
// Callees: the segment table's virtual Find (slot 17, +0x44; the name reference passed straight through), SetRange(first, count) @ 0x00C3AA50
//   (landed as "CWorld::SetDrawEnvironmentBounds": -1 and the range cleared when it does not fit, else 0).
// Parameter (sweep1 fix, FINDINGS chunk 437): every caller builds a CString temporary from a segment-name literal with
//   CString::CString(const char*) 0x00C0BFE0 and passes its address (0x00C16C29..0x00C16C53 "LHAudioBankLookupTable",
//   0x00C16C5C..0x00C16C7A "LHAudioBankCompData", 0x00C30EA9 "LHAudioWaveData", 0x00C30EDC "LHAudioBankSampleTable"),
//   so the argument is `const CString&`, not an id; the bytes are the same.
// Name: libpk, no PDB; descriptive (the catalog's `Accessor_GetAndSetEnvironmentBounds` is propagated).
// Shape: the miss returns `or eax,-1` with its own epilogue; the range result maps to 0 / -1 through
//   `xor ecx,ecx ; test eax,eax ; setge cl ; dec ecx`.
struct SSegment {                           // a segment table entry
    unsigned long m_first;                  // +0x00 start offset
    unsigned long m_count;                  // +0x04 length
};

class CString {                             // the libpk MFC-style string: one word
public:
    char* m_pchData;
};

struct CSegmentTable {                      // libpk keyed list (polymorphic), keyed by name
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual SSegment* Find(const CString& name);   // slot 17 (+0x44)
};

struct CSegmentFile {
    unsigned char _pad_0x00[0x80];
    CSegmentTable m_segments;               // +0x80
    long SetRange(unsigned long first, unsigned long count);   // 0x00C3AA50
    long OpenSegment(const CString& name);
};

long CSegmentFile::OpenSegment(const CString& name)
{
    SSegment* segment = m_segments.Find(name);
    if (!segment)
        return -1;
    return SetRange(segment->m_first, segment->m_count) >= 0 ? 0 : -1;
}