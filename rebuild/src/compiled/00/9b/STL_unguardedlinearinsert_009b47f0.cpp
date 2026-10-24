// _STL::__unguarded_linear_insert(pair<CCharString, long>* last, pair<CCharString, long> val, CKeyPairCompareLess<CCharString, long> comp) -- insertion-sort inner step over (name, value) entries: shift entries with larger names up one slot until val's name fits, then assign val there
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B47F0, 89 bytes, `ret 0Ch`: `__fastcall` (/Gr algorithm TU: last in ecx, the 8-byte
// pair value and the empty comparator on the stack). Catalog name "Sorted_Vector_InsertElement" (descriptive). Callers:
// __linear_insert @ 0x009B5A06 (0x009B5990, this chunk) and the __unguarded_insertion_sort_aux @ 0x009B5A46 (0x009B5A20).
// Callees: CBasicString<char>::operator< @ 0x00429950, CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0.
// Element: a CVectorMap<CCharString, long>-style entry (key compare only; the second word is not recoverable: long).
// Shape: STLport 4.6 verbatim; CCharString::operator< inlines (same block / empty-string tests, then the CBasicString
// compare out of line), the pair assignment is operator= on the name plus a word copy, val's name is destroyed at the end.
struct CBasicStringChar {                           // CBasicString<char> (13 bytes packed; only compared here)
    char* PData;                                    // +0x00
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};
struct CCharStringData {                            // shared string block (17 bytes: Data, then NoRefs at +0x0D)
    CBasicStringChar Data;                          // +0x00
};
class CCharString {
public:
    CCharStringData* PStringData;                   // +0x00
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return PStringData->Data < other.PStringData->Data;
    }
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                      // +0x00
    _T2 second;                                     // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

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
}

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<CCharString, long> SNameEntry;
template void __fastcall _STL::__unguarded_linear_insert<SNameEntry*, SNameEntry, CKeyPairCompareLess<CCharString, long> >(
    SNameEntry* __last, SNameEntry __val, CKeyPairCompareLess<CCharString, long> __comp);