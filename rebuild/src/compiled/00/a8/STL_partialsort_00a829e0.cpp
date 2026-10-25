// std::__partial_sort(SSwitchableNode* first, SSwitchableNode* middle, SSwitchableNode* last, SSwitchableNode*, CSwitchableNodeLess comp) -- STLport partial sort of the 16-byte (unsigned __int64 node ID, switchable node pointer) entries: heap [first, middle), pop every later entry with a smaller 64-bit key into it, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A829E0, 160 bytes, `ret 0Ch` (/Gr: first in ecx, middle in
//   edx; last, the null value-type pointer and the empty comparator's slot on the stack).
// Callees: __make_heap 0x00A80080 (this chunk; behind the inline make_heap: comp, (T*)0, (int*)0 pushed), __adjust_heap
//   0x00A7F1A0 (landed; `ret 18h`: hole 0 in edx, len, the 16-byte value, comp), sort_heap 0x00A81E80 (landed).
// Identity: a829e00 Twin: 0x00A828C0 (only the value-word copy differs). Catalog "Heap_Merge_*ByteElements" is descriptive.
// Shape: STLport 4.6 verbatim -- `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, __distance_type(first)); sort_heap(first, middle, comp);` with the
//   comparator inline (unsigned 64-bit key: high word `ja`/`jb`, then the low word) and __pop_heap INLINE here
//   (`*result = *first` = the pair's trivial four-dword assignment, then __adjust_heap(first, 0, middle - first, val,
//   comp)); the value copy keeps the key in ecx:edx and spills the second member, the pair's memberwise copy ctor
//   copies only that member (no padding) into the argument area.
namespace std {
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
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

class CANavQuadTreeNode;
typedef std::pair<unsigned __int64, CANavQuadTreeNode*>          SSwitchableNode;    // 16 bytes
typedef CKeyPairCompareLess<unsigned __int64, CANavQuadTreeNode*> CSwitchableNodeLess;

namespace std {
template <class _Tp> inline _Tp* __value_type_of(_Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type_of(_Tp*) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x00A80080
template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type_of(__first), __distance_type_of(__first));
}
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00A7F1A0
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00A81E80

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type_of(__first));
    sort_heap(__first, __middle, __comp);
}
template void __fastcall __partial_sort(SSwitchableNode*, SSwitchableNode*, SSwitchableNode*, SSwitchableNode*, CSwitchableNodeLess);
}