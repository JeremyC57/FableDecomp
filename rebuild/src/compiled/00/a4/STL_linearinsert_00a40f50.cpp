// _STL::__linear_insert(SNamePairEntry* first, SNamePairEntry* last, SNamePairEntry val, CNamePairLess comp) -- STLport insertion-sort step over the 12-byte ((name, name), object) entries: a new minimum shifts [first, last) up by one and lands at first, anything else goes through the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe @ 0x00A40F50, 150 bytes (+ int3 padding), `ret 10h`. /Gr TU written as explicit __fastcall: first in
//   ecx, last in edx, the by-value val (12 bytes) and the comparator slot on the stack.
// Callees: pair operator< 0x00A40250, __copy_backward 0x00A3FE60 (behind the inline copy_backward; its
//   random_access_iterator_tag temporary homed in the dead comparator slot), CCharString::operator= 0x0099EFB0 / copy
//   ctor 0x0099EC30 / ~CCharString 0x0099EAE0, __unguarded_linear_insert 0x00A40820 (val copied into the argument area).
// Label: catalog "Introsort_Quicksort_CCharStringTriple" is descriptive and wrong.
// Element: CSoundManagerLH's NamePairMap entry pair<pair<CCharString, CCharString>, CSoundBankLH*> (12 bytes; the landed
//   FindByNamePair 0x00A3EE50), ordered by CKeyPairCompareLess on the (name, name) key = STLport's pair operator<, which
//   retail calls out of line here (0x00A40250, __cdecl, two pointers). Pair copy constructors are STLport's explicit
//   ones (CCharString copy ctor 0x0099EC30); assignment is implicit (CCharString::operator= 0x0099EFB0 twice + pointer).
// Shape: STLport 4.6 _algo.c verbatim (`if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; }
//   else __unguarded_linear_insert(last, val, comp);`), val destroyed on both paths.

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

template <class T> class CBasicString {
public:
    T* m_pData;
#pragma auto_inline(off)
    // 0x00429950 (landed): the real body as a same-TU noinline function, as the STLport-over-CCharString rule asks.
    __declspec(noinline) bool operator<(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
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

namespace _STL {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2>
bool __cdecl operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y);   // 0x00A40250 (pair<CCharString, CCharString>)
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
    bool operator()(const K& a, const _STL::pair<K, V>& b) const { return a < b.first; }
    bool operator()(const _STL::pair<K, V>& a, const K& b) const { return a.first < b; }
};

typedef _STL::pair<CCharString, CCharString>      SNamePair;      // the (name, name) key
typedef _STL::pair<CCharString, CSoundBankLH*>     SNameEntry;     // NameMap entry, 8 bytes
typedef _STL::pair<SNamePair, CSoundBankLH*>       SNamePairEntry; // NamePairMap entry, 12 bytes
typedef CKeyPairCompareLess<CCharString, CSoundBankLH*> CNameLess;
typedef CKeyPairCompareLess<SNamePair, CSoundBankLH*>   CNamePairLess;

namespace _STL {
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                               const random_access_iterator_tag&, _Distance*);   // 0x00A3FE60
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A40820

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

template void __fastcall _STL::__linear_insert<SNamePairEntry*, SNamePairEntry, CNamePairLess>(SNamePairEntry*, SNamePairEntry*, SNamePairEntry, CNamePairLess);