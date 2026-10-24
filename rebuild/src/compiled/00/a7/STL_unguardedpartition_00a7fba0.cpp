// _STL::__unguarded_partition(SUniqueIDOpen* first, SUniqueIDOpen* last, SUniqueIDOpen pivot, CUniqueIDOpenLess comp) -- quicksort partition step over the 16-byte (unsigned __int64 unique ID, bool open) entries: swap entries across the pivot key until the scans meet, return the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7FBA0, 131 bytes, `ret 14h` (/Gr: first in ecx, last in edx; the 16-byte pivot
//   and the empty comparator's slot on the stack; the cut in eax).
// Element: pair<unsigned __int64, bool> (CNavSwitchableLeafNode's UniqueIDsOpen map): the swap temporary copies the key
//   dwords and ONE byte (pair's explicit copy ctor), the assignments copy all 16 bytes. Byte-identical twin modulo that
//   byte copy: 0x00A7FCB0 (pair<unsigned __int64, CANavQuadTreeNode*>).
// Label: catalog "Heap_AdjustDown_32ByteElements" is descriptive; the body is STLport _algo.c `__unguarded_partition`.
// Shape: STLport verbatim -- `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }` with the 64-bit key compare inline (high
//   word, then low word) and iter_swap's `_Tp tmp = *a; *a = *b; *b = tmp;` inline (the temporary in the frame).
namespace _STL {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<unsigned __int64, bool>          SUniqueIDOpen;      // 16 bytes
typedef CKeyPairCompareLess<unsigned __int64, bool> CUniqueIDOpenLess;

namespace _STL {
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}
template <class _Tp> inline _Tp* __value_type_of(_Tp*) { return (_Tp*)0; }
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type_of(__i1));
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
template SUniqueIDOpen* __fastcall __unguarded_partition(SUniqueIDOpen*, SUniqueIDOpen*, SUniqueIDOpen, CUniqueIDOpenLess);
}