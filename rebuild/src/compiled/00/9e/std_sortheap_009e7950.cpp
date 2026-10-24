// std::sort_heap(SSymbolEntry* first, SSymbolEntry* last, CCompareString comp) -- SGI sort_heap over the symbol map's (CCharString, CSymbolMap::CSymbolInfo) vector map: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E7950, length 115 bytes, `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<float,CCharString>*,CKeyPairCompareLess<float,CCharString>>` is
// propagated over the byte-identical skeleton. The callee 0x009E7060 (manifest `_Pop_heap_0<pair<CCharString,
// CSymbolMap::CSymbolInfo>*,...>`) is the SGI `__pop_heap` of that 16-byte pair, and the inline copy here is
// exactly it: Ego_r.pdb CSymbolInfo (12 bytes) = GroupName, EntryName (CCharString), Val (long); the map is
// CSymbolMap::SLONGMap, a CVectorMap<CCharString, CSymbolInfo, CCompareString<CSymbolInfo>> (empty comparator).
// Callees: __pop_heap @ 0x009E7060 (out of line, __fastcall), CCharString copy constructor @ 0x0099EC30 (x3).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline (16-byte stride: `and -16 ; cmp 16`). The value copy in the argument area is the implicit
// pair copy: the key string, then CSymbolInfo's implicit copy through its own address (`add edi,4`: two string
// copies and the Val dword).
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
struct CSymbolMap {
    struct CSymbolInfo {                    // Ego_r.pdb, 12 bytes
        CCharString GroupName;              // +0x00
        CCharString EntryName;              // +0x04
        long        Val;                    // +0x08
    };
};
struct SSymbolEntry {                       // std::pair<CCharString, CSymbolMap::CSymbolInfo> (16 bytes)
    CCharString             first;          // +0x00
    CSymbolMap::CSymbolInfo second;         // +0x04
};
struct CCompareString { };                  // CCompareString<CSymbolMap::CSymbolInfo>, empty, by value

void __fastcall __pop_heap(SSymbolEntry* first, SSymbolEntry* last, SSymbolEntry* result,
                           SSymbolEntry value, CCompareString comp, int*);   // 0x009E7060

inline void __pop_heap_aux(SSymbolEntry* first, SSymbolEntry* last, SSymbolEntry*, CCompareString comp)
{
    __pop_heap(first, last - 1, last - 1, SSymbolEntry(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(SSymbolEntry* first, SSymbolEntry* last, CCompareString comp)
{
    __pop_heap_aux(first, last, (SSymbolEntry*)0, comp);
}

void __fastcall sort_heap(SSymbolEntry* first, SSymbolEntry* last, CCompareString comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}