// _global::HasBankSegments(CSegmentFile* file) -- true when the bank file carries both its "LHAudioBankLookupTable" and "LHAudioBankCompData" segments (opening each by name; the second is only tried when the first exists)
// Shape (retry1): parked as an "LTCG register argument" gap; as a same-TU `static __declspec(noinline)` with a same-TU caller VC7.1 /O2
//   gives it retail's private convention itself (file in esi, plain `ret`). The `||` result lives in a byte local and the return is
//   `sete` of it (the two conditional CString temporaries of one full-expression: construction flags 1 / 2 in ebx).
// Leaf: HasBankSegments
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C16C10, 203 bytes, file in esi, plain `ret`, EH-framed.
// Caller: the bank loader @ 0x00C16FE2 (after ReadBankInfoCount @ 0x00C16E10). Callees: CSegmentFile::OpenSegment @ 0x00C2D9C0
//   (0 / -1), CString::CString(const char*) @ 0x00C0BFE0, CString::~CString @ 0x00C0BA40 (landed spellings).
// Label correction: the catalog's `LHAudioBank_InitializeBankData` is propagated; descriptive name.
class CString {                            // MFC-style string: one word
public:
    char* m_pchData;
    CString(const char* lpsz);             // 0x00C0BFE0
    ~CString();                            // 0x00C0BA40
};
class CSegmentFile {
public:
    long OpenSegment(const CString& name); // 0x00C2D9C0: 0 found and positioned, -1 missing
};

static __declspec(noinline) bool HasBankSegments(CSegmentFile* file)
{
    bool missing = file->OpenSegment(CString("LHAudioBankLookupTable")) < 0
                || file->OpenSegment(CString("LHAudioBankCompData")) < 0;
    return !missing;
}

// Same-TU caller (stands in for the bank loader 0x00C16F00, which keeps the file in esi). Not graded.
long ReadBankStandIn(CSegmentFile* file)
{
    if (!HasBankSegments(file))
        return 0;
    return file->OpenSegment(CString("x"));
}