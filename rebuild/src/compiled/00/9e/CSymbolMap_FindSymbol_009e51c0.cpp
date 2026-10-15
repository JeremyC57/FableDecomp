// CSymbolMap::FindSymbol(const CCharString& name) -- report whether the name is mapped in the SLONGMap vector map (lower bound, then reject when the key sorts before the found entry): non-zero when found
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E51C0, length 61 bytes.
// Callees: CVectorMap::LowerBound @ 0x009E6910, CCharString::FastCompare @ 0x009E6530 (`key < entry.first`).
// Shape: the same inline map Find as Lookup (0x009E5120: end() read once after LowerBound, kept as the default
// result), then the member end() re-read into ecx and `xor eax,eax ; cmp esi,ecx ; setne al` -- a full-width
// `long` result (a bool return drops the xor).
struct CCharStringData;
struct CCharString {
    CCharStringData* m_pStringData;
    bool FastCompare(const CCharString& other) const;   // 0x009E6530 (the CCompareString "less")
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

// CVectorMap<CCharString, CSymbolInfo, CCompareString<CSymbolInfo>>: the retail 12-byte vector first.
struct CSymbolVectorMap {
    SSymbolPair* m_pFirst;                         // +0x00
    SSymbolPair* m_pLast;                          // +0x04
    SSymbolPair* m_pEnd;                           // +0x08
    SSymbolPair* LowerBound(const CCharString& key) const;   // 0x009E6910
    SSymbolPair* end() const { return m_pLast; }
    SSymbolPair* Find(const CCharString& key) const
    {
        SSymbolPair* it = LowerBound(key);
        SSymbolPair* last = end();
        return (it == last || key.FastCompare(it->first)) ? last : it;
    }
};

struct CSymbolMap {
    void*            _padding_;                    // +0x00
    CSymbolVectorMap SLONGMap;                     // +0x04
    long FindSymbol(const CCharString& name) const;
};

long CSymbolMap::FindSymbol(const CCharString& name) const
{
    return SLONGMap.Find(name) != SLONGMap.end();
}