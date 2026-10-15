// CSymbolMap::FindFirstSymbolWithVal(const CCharString& group, long value) -- walk the SLONGMap vector map's pairs in order and return the first symbol info (pair.second) whose GroupName equals the given group (the inlined CCharString ==: same data block, or both present with equal lengths and Compare @ 0x00411570 == 0) AND whose Val equals the value, or 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E50C0, length 95 bytes.
// Callees: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`: entry text in ecx, key text in edx).
// Shape: the end word cached in ebx (loaded before the begin word), the key's data block and the value loaded
// once AFTER the empty test (edi/ebp) and kept across the Compare call; `neg ; sbb al,al ; inc al` for
// `Compare == 0`; `lea eax,[esi+4]` (the address of pair.second) in the found tail; `ret 8`. Retail's
// loop-invariant block load across a call reproduces with the end and the key block in locals read inside the
// guard of a bottom-tested loop (a for-loop with the key local hoists that load above the guard).
// Model: PDB CSymbolMap (+4 SLONGMap, the retail 12-byte vector), pair<CCharString, CSymbolInfo>.
struct CCharStringData {
    const char*   m_pChars;                        // +0x00
    unsigned long m_length;                        // +0x04
};
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

struct CCharString {
    CCharStringData* m_pStringData;
};

static __forceinline bool SameText(CCharStringData* a, CCharStringData* b)
{
    if (a == b)
        return true;
    if (!a || !b)
        return false;
    if (a->m_length != b->m_length)
        return false;
    return Compare_00411570(a->m_pChars, b->m_pChars) == 0;
}

struct CSymbolInfo {                               // PDB CSymbolMap::CSymbolInfo, 12 bytes
    CCharString GroupName;                         // +0x00
    CCharString EntryName;                         // +0x04
    long        Val;                               // +0x08
};

struct SSymbolPair {                               // std::pair<CCharString, CSymbolInfo>, 16 bytes
    CCharString first;                             // +0x00
    CSymbolInfo second;                            // +0x04
};

struct CSymbolVectorMap {                          // the retail 12-byte vector first
    SSymbolPair* m_pFirst;                         // +0x00
    SSymbolPair* m_pLast;                          // +0x04
    SSymbolPair* m_pEnd;                           // +0x08
};

struct CSymbolMap {
    void*            _padding_;                    // +0x00
    CSymbolVectorMap SLONGMap;                     // +0x04
    const CSymbolInfo* FindFirstSymbolWithVal(const CCharString& group, long value) const;
};

const CSymbolInfo* CSymbolMap::FindFirstSymbolWithVal(const CCharString& group, long value) const
{
    const SSymbolPair* end = SLONGMap.m_pLast;
    const SSymbolPair* it = SLONGMap.m_pFirst;
    if (it != end) {
        CCharStringData* key = group.m_pStringData;
        do {
            if (SameText(it->second.GroupName.m_pStringData, key) && it->second.Val == value)
                return &it->second;
        } while (++it != end);
    }
    return 0;
}