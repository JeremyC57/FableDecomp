// _STL::__partial_sort(SNameEntry* first, SNameEntry* middle, SNameEntry* last, SNameEntry*, CNameLess comp) -- STLport partial sort over the 8-byte (name, object) entries: heap [first, middle), pop every smaller later element into it, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe @ 0x00A41B60, 189 bytes (+ int3 padding), `ret 0Ch` with an 8-byte-aligned ebp frame. /Gr TU
//   written as explicit __fastcall: first in ecx, middle in edx; last, the null type pointer and the comparator slot
//   on the stack.
// Callees: __make_heap 0x00A41200 (behind the inline make_heap), CCharString copy ctor 0x0099EC30 / operator= 0x0099EFB0
//   / ~CCharString 0x0099EAE0, __adjust_heap 0x00A408E0 (behind the inline __pop_heap), sort_heap 0x00A41960.
// Label: catalog "Heap_PopHeap_CCharStringPair" is descriptive.
// Element: CSoundManagerLH's NameMap entry pair<CCharString, CSoundBankLH*> (8 bytes; the landed FindByName 0x00A3ED30),
//   ordered by CKeyPairCompareLess (the key string's inline operator<: same block, null left, null right, then
//   CBasicString<char>::operator< 0x00429950 -- given its landed body as a same-TU noinline function). The pair has
//   STLport's explicit copy constructor; assignment is implicit (CCharString::operator= 0x0099EFB0 + the pointer).
// Shape: STLport 4.6 _algo.c verbatim (`make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i,
//   *first)) __pop_heap(first, middle, i, _Tp(*i), comp, DISTANCE_TYPE); sort_heap(first, middle, comp);`), with
//   __pop_heap (`*result = *first; __adjust_heap(first, 0, middle - first, val, comp);`) inline around the frame copy.

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
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x00A41200
template <class _Tp> inline _Tp* __value_type_of(_Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type_of(_Tp*) { return (int*)0; }
template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type_of(__first), __distance_type_of(__first));
}
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00A408E0
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00A41960

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type_of(__first));
    sort_heap(__first, __middle, __comp);
}
}

template void __fastcall _STL::__partial_sort<SNameEntry*, SNameEntry, CNameLess>(SNameEntry*, SNameEntry*, SNameEntry*, SNameEntry*, CNameLess);