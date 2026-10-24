// _global::HasBankSegments(CSegmentFile_00c16c10* file) -- true when the bank file carries both its "LHAudioBankLookupTable" and "LHAudioBankCompData" segments (opening each by name; the second is only tried when the first exists)
// Grade: DIFFER(ours v 203) -- LTCG register argument (lane gap): retail takes the segment file in esi (`mov ecx, esi` before each OpenSegment, esi never set or saved); this reference body takes it in ecx (`__fastcall`). One grade, no dummy parameters.
// Leaf: HasBankSegments
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C16C10, 203 bytes, file in esi, plain `ret`, EH-framed (the two
//   conditional string temporaries of one `||` full-expression: construction flags 1 / 2 in ebx, mirrored to a
//   frame word, each destroyed only when built).
// Caller: the bank loader @ 0x00C16FE2 (after ReadBankInfoCount @ 0x00C16E10). Callees: OpenSegment @ 0x00C2D9C0
//   (0 / -1), the MFC-style string @ 0x00C0BFE0 / 0x00C0BA40.
// Shape: the `||` result lives in a byte local and the return is `sete` of it.
// Label correction: the catalog's `LHAudioBank_InitializeBankData` is propagated; descriptive name.
struct CString_00c16c10 {                  // MFC-style string: one word
    char* m_pchData;
    CString_00c16c10(const char* text);    // 0x00C0BFE0
    ~CString_00c16c10();                   // 0x00C0BA40
};

struct CSegmentFile_00c16c10 {
    int OpenSegment(const CString_00c16c10& name);   // 0x00C2D9C0: 0 found and positioned, -1 missing
};

bool __fastcall HasBankSegments(CSegmentFile_00c16c10* file)
{
    bool missing = file->OpenSegment(CString_00c16c10("LHAudioBankLookupTable")) < 0
                || file->OpenSegment(CString_00c16c10("LHAudioBankCompData")) < 0;
    return !missing;
}
