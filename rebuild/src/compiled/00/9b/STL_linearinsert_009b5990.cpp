// std::__linear_insert(pair<CCharString, long>* first, pair<CCharString, long>* last, pair<CCharString, long> val, CKeyPairCompareLess<CCharString, long> comp) -- insertion-sort step over (name, value) entries: a new minimum shifts the prefix up one slot, anything else goes through the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B5990, 137 bytes, `ret 0Ch`: `__fastcall` (/Gr algorithm TU: first in ecx, last in
// edx, the 8-byte pair value and the empty comparator on the stack). Catalog name "Sorted_Vector_InsertElement_CCharString"
// (descriptive). Caller: the __insertion_sort @ 0x009B6A0F (0x009B69E0).
// Callees: CBasicString<char>::operator< @ 0x00429950, __copy_backward<pair*, pair*, int> @ 0x009B1E20 (out of line, the
// random_access_iterator_tag temporary homed in the dead comparator slot), CCharString::operator= @ 0x0099EFB0,
// CCharString(const CCharString&) @ 0x0099EC30 (val rebuilt in the callee's argument area), __unguarded_linear_insert
// @ 0x009B47F0 (this chunk), ~CCharString @ 0x0099EAE0 (the by-value val, on both paths).
// Shape: STLport 4.6 verbatim with CCharString::operator< inlined.
struct CBasicStringChar {                           // CBasicString<char> (13 bytes packed; only compared here)
    char* PData;                                    // +0x00
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};
class CCharStringData { public:                            // shared string block (17 bytes: Data, then NoRefs at +0x0D)
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

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                      // +0x00
    _T2 second;                                     // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

struct random_access_iterator_tag {
    random_access_iterator_tag() {}
};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*);   // 0x009B1E20

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x009B47F0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}
}

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<CCharString, long> SNameEntry;
template void __fastcall std::__linear_insert<SNameEntry*, SNameEntry, CKeyPairCompareLess<CCharString, long> >(
    SNameEntry* __first, SNameEntry* __last, SNameEntry __val, CKeyPairCompareLess<CCharString, long> __comp);