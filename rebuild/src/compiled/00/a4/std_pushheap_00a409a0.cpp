// std::__push_heap(RandomAccessIter first, int holeIndex, int topIndex, T val, CKeyPairCompareLess comp) -- (pair<pair<CCharString, CCharString>, CSoundBankLH*>) STLport heap sift-up of CSoundManagerLH's (name, name) map entries
// Leaf: ??$__push_heap@PAU?$pair@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@std@@HU12@V?$CKeyPairCompareLess@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@@@std@@YIXPAU?$pair@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@0@HHU10@V?$CKeyPairCompareLess@U?$pair@VCCharString@@V1@@std@@PAVCSoundBankLH@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A409A0, 249 bytes, /Gr __fastcall (first ecx, holeIndex edx; topIndex,
//   the 12-byte value by value and the empty comparator on the stack), `ret 14h`.
// Identity (proven): STLport 4.6 __push_heap; its caller is __adjust_heap 0x00A410A0 (`call 0xA409A0` after the sift-down),
//   part of the 12-byte (stride 2AAAAAABh) sort family under std::sort 0x00A42060, which CSoundManagerLH's commit step
//   0x00A3F340 calls on the NamePairMap at +0x54 (the landed FindByNamePair 0x00A3EE50 places that map: key
//   pair<CCharString, CCharString>, mapped CSoundBankLH*). The compare is CKeyPairCompareLess's `a.first < b.first` =
//   STLport's inline pair operator< (first names, then second names), inlined here.
// Callees: CBasicString<char>::operator< 0x00429950, CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0.
// Label: catalog "Heap_AdjustDown_CCharStringTriple" is descriptive and wrong (this is the sift-up).
// Shape: STLport 4.6 _heap.c __push_heap verbatim; implicit pair assignment (two CCharString::operator= + the pointer);
//   the by-value element dies at the end (second name, then first).
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
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}

template void __fastcall __push_heap<TEntry*, int, TEntry, TCompare>(TEntry*, int, int, TEntry, TCompare);
}