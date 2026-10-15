// CSymbolMap::Lookup(const CCharString& name) -- find the symbol in the SLONGMap vector map (lower bound, then reject when the key sorts before the found entry) and return its value (CSymbolInfo::Val), or 0 when the name is not mapped
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E5120, length 68 bytes.
// Callees: CVectorMap::LowerBound @ 0x009E6910 (`__fastcall` on the map at +4, the iterator returned in eax),
// CCharString::FastCompare @ 0x009E6530 (`key < entry.first`, `__fastcall` on the key).
// Shape: the inline map Find reads end() ONCE right after LowerBound (`mov esi,[esi+4]`) and keeps it as the
// default result (`(it == last || key < it->first) ? last : it`); the caller re-reads end() from the member
// (`cmp esi,[ebx+8]`), then `mov eax,[esi+0Ch]` (pair.second.Val) or the shared `xor eax,eax` tail, `ret 4`.
// Model: PDB CSymbolMap (+4 SLONGMap), CSymbolInfo {GroupName, EntryName, Val}, retail 12-byte vector.
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
    long Lookup(const CCharString& name) const;
};

long CSymbolMap::Lookup(const CCharString& name) const
{
    SSymbolPair* it = SLONGMap.Find(name);
    if (it != SLONGMap.end())
        return it->second.Val;
    return 0;
}