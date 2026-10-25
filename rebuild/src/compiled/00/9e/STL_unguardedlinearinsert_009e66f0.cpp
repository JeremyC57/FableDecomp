// std::__unguarded_linear_insert(pair<CCharString, CSymbolMap::CSymbolInfo>* last, pair<CCharString, CSymbolMap::CSymbolInfo> val, CCompareString<CSymbolMap::CSymbolInfo> comp) -- insertion-sort inner step of the symbol map: shift the entries whose name sorts after val's up one place and assign val into the gap
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E66F0, 173 bytes, `ret 14h` (/Gr algorithm TU: last in ecx; the 16-byte
//   value and the empty comparator by value on the stack; the callee destroys the value).
// Identity: callers __unguarded_insertion_sort_aux 0x009E6D10 (landed) and the __linear_insert at 0x009E7034 of the
//   CSymbolMap entry sort: element pair<CCharString, CSymbolMap::CSymbolInfo> (Ego_r.pdb CSymbolInfo = GroupName +0,
//   EntryName +4, Val +8), comparator CCompareString<CSymbolMap::CSymbolInfo> (the manifest's _Push_heap /
//   _Adjust_heap labels of the family name it). The catalog label "insertion_sort_16byte" is descriptive.
// Callees: CCharString::FastCompare(const CCharString&) const @ 0x009E6530 (landed; the comparator inlined down to it),
//   CCharString::operator= @ 0x0099EFB0 (x3 per assignment), ~CCharString @ 0x0099EAE0 (val's three strings).
// Shape: STLport `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`
//   with the implicit pair / CSymbolInfo assignments inline (three string assignments, then the Val dword); the loop
//   keeps a second pointer at next's Val (`lea edi,[esi+0Ch]`).
class CCharString {
public:
    void* PStringData;                                  // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
};

class CSymbolMap {
public:
    class CSymbolInfo {                                 // Ego_r.pdb, 12 bytes
    public:
        CCharString GroupName;                          // +0x00
        CCharString EntryName;                          // +0x04
        long        Val;                                // +0x08
    };
};

namespace std {
template <class _T1, class _T2> struct pair {           // STLport 4.6 pair (a struct: mangled U)
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

// The CVectorMap<CCharString, T> key order: names compared with the cheap CCharString::FastCompare (empty functor).
template <class T> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, T>& a, const std::pair<CCharString, T>& b) const
    {
        return a.first.FastCompare(b.first);
    }
};
typedef std::pair<CCharString, CSymbolMap::CSymbolInfo> TSymbolEntry;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
template void __fastcall __unguarded_linear_insert<TSymbolEntry*, TSymbolEntry, CCompareString<CSymbolMap::CSymbolInfo> >(
    TSymbolEntry*, TSymbolEntry, CCompareString<CSymbolMap::CSymbolInfo>);
}