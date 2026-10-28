// std::__unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot, CKeyPairCompareLess comp) -- (pair<pair<CCharString, CCharString>, CSoundBankLH*>) STLport introsort partition step over CSoundManagerLH's (name, name) map entries
// Leaf: ??$__unguarded_partition@PAU?$pair@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@std@@U12@V?$CKeyPairCompareLess@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@@@std@@YIPAU?$pair@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@0@PAU10@0U10@V?$CKeyPairCompareLess@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A40DF0, 241 bytes, /Gr __fastcall (first ecx, last edx; the 12-byte pivot
//   by value and the empty comparator on the stack), `ret 10h`, the cut in eax.
// Identity (proven): STLport 4.6 __unguarded_partition; its caller is __introsort_loop 0x00A41EA0 (`call 0xA40DF0` with the
//   pivot copied into the argument area), the loop under std::sort 0x00A42060 that CSoundManagerLH's commit step 0x00A3F340
//   runs on the NamePairMap at +0x54 (key pair<CCharString, CCharString>, mapped CSoundBankLH*; the landed FindByNamePair
//   0x00A3EE50). Both compares are CKeyPairCompareLess's `a.first < b.first` = STLport's inline pair operator< (inlined).
// Callees: CBasicString<char>::operator< 0x00429950, std::swap 0x00A3FDC0 (iter_swap inlined to it), ~CCharString 0x0099EAE0.
// Label: catalog "Introsort_Partition_CSymbolInfo" names the wrong element (see NOTES: the swap 0x00A3FDC0 was landed as
//   swap<CSymbolMap::CSymbolInfo>, a layout twin; its only callers are this sort family).
// Shape: STLport 4.6 _algo.c __unguarded_partition verbatim; the by-value pivot dies at the end (second name, then first).
template <class _CharT> class CBasicString {
public:
    _CharT* PData;
#pragma auto_inline(off)
    // 0x00429950 (landed): the real body as a same-TU noinline stand-in (STLport-over-CCharString rule).
    __declspec(noinline) bool operator<(const CBasicString& other) const
    {
        const _CharT* b = other.PData;
        const _CharT* a = PData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return false;
            if (*a < *b)
                return true;
            if (*a > *b)
                return false;
            ++a;
            ++b;
        }
    }
#pragma auto_inline(on)
};

class CCharString {
public:
    CBasicString<char>* PStringData;                        // the shared block (CBasicString<char> first)
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return *PStringData < *other.PStringData;
    }
};

class CSoundBankLH;                                         // descriptive: the mapped object of CSoundManagerLH's maps

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2>
inline bool __cdecl operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<CCharString, CCharString> SNamePair;
typedef std::pair<SNamePair, CSoundBankLH*> TEntry;
typedef CKeyPairCompareLess<SNamePair, CSoundBankLH*> TCompare;
namespace std {
template <class _Tp> void __fastcall swap(_Tp& __a, _Tp& __b);                                 // 0x00A3FDC0

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp)
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

template TEntry* __fastcall __unguarded_partition<TEntry*, TEntry, TCompare>(TEntry*, TEntry*, TEntry, TCompare);
}