// CSymbolMap::SymbolExists(const CCharString& name) const -- report whether the name is mapped in the SLONGMap vector map (lower bound, then reject when the key sorts before the found entry)
// Leaf: ?SymbolExists@CSymbolMap@@QBE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E51C0, length 61 bytes. First landed as CSymbolMap::FindSymbol returning long.
// Name: Ego_r.pdb runs GetSymbolSLONG (0x00B36AD0) / FindSymbol (0x00B36B20, bool(const CCharString&, long&) const) /
// SymbolExists (0x00B36B80, ?SymbolExists@CSymbolMap@@QBE_NABVCCharString@@@Z); retail runs 0x009E5120 (ret 4, value) /
// 0x009E5170 (ret 8, stores the value through the second argument) / 0x009E51C0 (ret 4, found flag): this is SymbolExists.
// Callees: CVectorMap::LowerBound @ 0x009E6910, CCharString::FastCompare @ 0x009E6530 (`key < entry.first`).
// Shape: the same inline map Find as Lookup (0x009E5120: end() read once after LowerBound, kept as the default
// result), then the member end() re-read into ecx and `xor eax,eax ; cmp esi,ecx ; setne al`: VC7.1 emits the xor for the
// plain `return a != b;` bool return too (the `?:` and named-local forms drop it).
struct CCharStringData;
class CCharString {                               // a class, as Ego_r.pdb mangles it (V)
public:
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
    bool SymbolExists(const CCharString& name) const;
};

bool CSymbolMap::SymbolExists(const CCharString& name) const
{
    return SLONGMap.Find(name) != SLONGMap.end();
}
