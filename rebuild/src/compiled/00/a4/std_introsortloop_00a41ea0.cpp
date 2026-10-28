// std::__introsort_loop(RandomAccessIter first, RandomAccessIter last, T*, int depth_limit, CKeyPairCompareLess comp) -- (pair<pair<CCharString, CCharString>, CSoundBankLH*>) STLport introsort driver of CSoundManagerLH's (name, name) map: median-of-three partitions down to 16-entry runs, partial_sort once the depth limit runs out
// Leaf: ??$__introsort_loop@PAU?$pair@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@std@@U12@HV?$CKeyPairCompareLess@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@@@std@@YIXPAU?$pair@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@0@00HV?$CKeyPairCompareLess@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A41EA0, 401 bytes, /Gr __fastcall (first ecx, last edx; (T*)0, depth_limit
//   and the empty comparator on the stack), `ret 0Ch`.
// Identity (proven): STLport 4.6 __introsort_loop over 12-byte entries (2AAAAAABh / sar 1); callers are std::sort 0x00A42060
//   (`__introsort_loop(first, last, (T*)0, __lg(n) * 2, comp)`) and itself. sort 0x00A42060 is called by CSoundManagerLH's
//   commit step 0x00A3F340 on the NamePairMap at +0x54 (key pair<CCharString, CCharString>, mapped CSoundBankLH*; the
//   landed FindByNamePair 0x00A3EE50, push_back 0x00A41840 and _M_insert_overflow 0x00A41390 carry the same element).
// Callees: __unguarded_partition 0x00A40DF0 and __partial_sort 0x00A41CD0, STLport pair operator< 0x00A40250 (__cdecl, the
//   out-of-line instance of the inline template: VC7.1 inlines the first __median compare and calls the rest),
//   CBasicString<char>::operator< 0x00429950, CCharString copy ctor 0x0099EC30 (the pivot in the argument area), itself.
// Label: catalog "Introsort_Quicksort_CCharStringPair" is descriptive.
// Shape: STLport 4.6 _algo.c verbatim, as the 8-byte sibling 0x00A41D50 and the C3DHelperPoint loop 0x00A34B00.
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
const int __stl_threshold = 16;

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);                            // 0x00A40DF0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                               _Tp*, _Compare __comp);                                          // 0x00A41CD0
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                         _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __fastcall __introsort_loop<TEntry*, TEntry, int, TCompare>(TEntry*, TEntry*, TEntry*, int, TCompare);
}