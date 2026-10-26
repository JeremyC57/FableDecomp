// CSymbolMap::FindFirstSymbolWithVal(long value) -- walk the SLONGMap vector map's pairs in order and return the first symbol info (pair.second) whose Val equals the argument, or 0 when none matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E5090, length 39 bytes.
// Callees: none. Shape: begin/end into eax/ecx, the for-loop top test, the argument in edx before the aligned
// loop head (`mov edi,edi`), `cmp [eax+0Ch],edx` (second.Val), `add eax,10h` stride, `add eax,4` in the found
// tail (the address of pair.second), `ret 4`.
// Model: PDB CSymbolMap (+4 SLONGMap = CVectorMap<CCharString, CSymbolInfo>; the retail vector is 12 bytes so
// its first/last words sit at +4/+8), pair<CCharString, CSymbolInfo {GroupName, EntryName, Val}> = 16 bytes.
class CCharStringData;
class CCharString { public:
    CCharStringData* m_pStringData;
};

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

class CSymbolMap { public:
    void*            _padding_;                    // +0x00
    CSymbolVectorMap SLONGMap;                     // +0x04
    const CSymbolInfo* FindFirstSymbolWithVal(long value) const;
};

const CSymbolInfo* CSymbolMap::FindFirstSymbolWithVal(long value) const
{
    for (const SSymbolPair* it = SLONGMap.m_pFirst; it != SLONGMap.m_pLast; ++it) {
        if (it->second.Val == value)
            return &it->second;
    }
    return 0;
}