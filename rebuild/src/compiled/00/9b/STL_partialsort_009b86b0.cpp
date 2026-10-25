// std::__partial_sort(pair<CCharString,unsigned long>* first, pair<CCharString,unsigned long>* middle, pair<CCharString,unsigned long>* last, pair<CCharString,unsigned long>*, CKeyPairCompareLess<CCharString,unsigned long> comp) -- STLport partial sort of (name, value) pairs by name: heapify [first, middle), pop every smaller element of [middle, last) into the heap, then sort the heap
// Flags: /Gr
// Leaf: ??$__partial_sort@PAU?$pair@VCCharString@@K@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B86B0, 189 bytes (+ int3 padding), `ret 0Ch` (/Gr TU: first in ecx,
//   middle in edx; last, the unused T* and the empty comparator on the stack; `and esp,-8` frame for the 8-byte pair
//   local inside the loop).
// Identity: the CVectorMap<CCharString, unsigned long> sort family (__make_heap row 0x009B6D20, __adjust_heap 0x009B5C70,
//   sort_heap 0x009B8170 landed); callers 0x009B89EC / 0x009B8BEF.  Catalogued as `Heapsort_CCharString_Pair`.
// Callees: CBasicString<char>::operator< @ 0x00429950 (its landed body is in the TU, noinline, as the retail TUs carry the
//   template; the inlined CCharString operator<: equal data -> false, null this -> true, null other -> false, the other
//   string's word read first), CCharString(const CCharString&) @ 0x0099EC30 (x2), CCharString::operator= @ 0x0099EFB0,
//   ~CCharString @ 0x0099EAE0.
// Shape: as 0x009B6A40 with CKeyPairCompareLess comparing `first`; make_heap and __pop_heap inlined.
template <class T> class CBasicString {
public:
    T* m_pData;
    __forceinline long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
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
    __declspec(noinline) bool operator<(const CBasicString& other) const { return Compare(other) < 0; }   // 0x00429950
};

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
private:
    CBasicString<char>* PStringData;
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
__forceinline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                              _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Compare>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);

template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}

template void __partial_sort(pair<CCharString, unsigned long>*, pair<CCharString, unsigned long>*,
                             pair<CCharString, unsigned long>*, pair<CCharString, unsigned long>*,
                             CKeyPairCompareLess<CCharString, unsigned long>);
}