// std::__unguarded_partition(pair<CCharString, long>* first, pair<CCharString, long>* last, pair<CCharString, long> pivot, CKeyPairCompareLess<CCharString, long> comp) -- quicksort partition over (name, value) entries: advance first past smaller names, retreat last past larger ones, swap the pair, repeat until they cross
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B5800, 170 bytes, `ret 0Ch`: `__fastcall` (/Gr algorithm TU: first in ecx, last in
// edx, the 8-byte pivot pair and the empty comparator on the stack), returns the cut in eax. Catalog name
// "Quicksort_Partition_Pair_CCharString" (descriptive). Caller: the __introsort_loop @ 0x009B8BBA.
// Callees: CBasicString<char>::operator< @ 0x00429950, CCharString(const CCharString&) @ 0x0099EC30 / operator= @ 0x0099EFB0
// / ~CCharString @ 0x0099EAE0 (iter_swap's temporary pair in the frame, then the by-value pivot's name).
// Shape: STLport 4.6 verbatim, with iter_swap -> __iter_swap(a, b, value_type) inlined and CCharString::operator< inlined
// (the pivot's string block is re-read into esi at the top of each round).
struct CBasicStringChar {                           // CBasicString<char> (13 bytes packed; only compared here)
    char* PData;                                    // +0x00
    __forceinline long Compare(const CBasicStringChar& other) const
    {
        const char* b = other.PData;
        const char* a = PData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950 (body below, kept out of line)
};

// The real CBasicString<char>::operator< body (landed 0x00429950) in this TU, kept out of line: VC7.1 then knows the call
// only reads memory, so the pivot's string block stays in esi across it (as retail) instead of being re-read.
#pragma auto_inline(off)
__declspec(noinline) bool CBasicStringChar::operator<(const CBasicStringChar& other) const
{
    return Compare(other) < 0;
}
#pragma auto_inline(on)
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

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                      // +0x00
    _T2 second;                                     // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type(__i1));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
}

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<CCharString, long> SNameEntry;
template SNameEntry* __fastcall std::__unguarded_partition<SNameEntry*, SNameEntry, CKeyPairCompareLess<CCharString, long> >(
    SNameEntry* __first, SNameEntry* __last, SNameEntry __pivot, CKeyPairCompareLess<CCharString, long> __comp);