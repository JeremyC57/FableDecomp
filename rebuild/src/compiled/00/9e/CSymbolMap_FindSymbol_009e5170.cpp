// CSymbolMap::FindSymbol(const CCharString& name, long& value) -- look the name up in the SLONGMap vector map (lower bound, then reject when the key sorts before the found entry); on a hit store the symbol's value and return true
// Leaf: ?FindSymbol@CSymbolMap@@QBE_NABVCCharString@@AAJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E5170, length 76 bytes. PDB: ?FindSymbol@CSymbolMap@@QBE_NABVCCharString@@AAJ@Z.
// Label correction: the catalog's "SymbolMap_Lookup" is CSymbolMap::FindSymbol(name, value&) (`ret 8`, bool in al).
// Callees: CVectorMap<CCharString,CSymbolInfo,CCompareString>::LowerBound @ 0x009E6910 (landed), CCharString::FastCompare
//   @ 0x009E6530 (`key < entry.first`).
// Model: the landed sibling GetSymbolSLONG (0x009E5120, landed as "CSymbolMap::Lookup"): PDB CSymbolMap {_padding_ +0,
//   SLONGMap +4}, CSymbolInfo {GroupName, EntryName, Val}, the retail 12-byte vector under the vector map.
// Shape: the inline map Find reads end() once after LowerBound and keeps it as the default result; the caller re-reads
//   end() from the member (`cmp esi,[ebx+8]`); the hit block stores `pair.second.Val` (+0xC) through the reference and
//   returns `mov al,1` on the fall-through, the miss returns the separate `xor al,al` tail.
class CCharStringData;
class CCharString {
public:
    CCharStringData* m_pStringData;
    bool FastCompare(const CCharString& other) const;   // 0x009E6530 (the CCompareString "less")
};

class CSymbolInfo {                                // PDB CSymbolMap::CSymbolInfo, 12 bytes
public:
    CCharString GroupName;                         // +0x00
    CCharString EntryName;                         // +0x04
    long        Val;                               // +0x08
};

struct SSymbolPair {                               // std::pair<CCharString, CSymbolInfo>, 16 bytes
    CCharString first;                             // +0x00
    CSymbolInfo second;                            // +0x04
};

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

class CSymbolMap {
public:
    void*            _padding_;                    // +0x00
    CSymbolVectorMap SLONGMap;                     // +0x04
    bool FindSymbol(const CCharString& name, long& value) const;
};

bool CSymbolMap::FindSymbol(const CCharString& name, long& value) const
{
    SSymbolPair* it = SLONGMap.Find(name);
    if (it != SLONGMap.end()) {
        value = it->second.Val;
        return true;
    }
    return false;
}