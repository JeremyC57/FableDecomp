// std::__unguarded_insertion_sort_aux(SSwitchableNode* first, SSwitchableNode* last, SSwitchableNode*, CSwitchableNodeLess comp) -- unguarded insertion sort of the 16-byte (unsigned __int64 unique ID, CANavQuadTreeNode*) entries: insert each entry of [first, last) into the sorted run before it
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7F060, 58 bytes (+ int3 padding to 64), `ret 8` (/Gr: first in ecx, last in edx; the null value-type
//   pointer and the empty comparator's slot on the stack).
// Callers: the final insertion sorts 0x00A7FD47, 0x00A8331C, 0x00A84249 (CNavQuadTree's SwitchableNodes map,
//   CVectorMap<unsigned __int64, CANavQuadTreeNode*>). Callee: __unguarded_linear_insert 0x00A7D610 (this chunk).
// Label: catalog "_Copy_impl<CNavigationPosition*,CNavigationPosition*>" is propagated; the body is STLport _algo.c
//   `__unguarded_insertion_sort_aux`.
// Shape: `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` -- the comparator slot is kept
//   in ebx and pushed first; the value is pair's explicit copy ctor built in the argument area (key as two dwords, then the
//   node pointer).
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
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

class CANavQuadTreeNode;
typedef std::pair<unsigned __int64, CANavQuadTreeNode*>          SSwitchableNode;      // 16 bytes
typedef CKeyPairCompareLess<unsigned __int64, CANavQuadTreeNode*> CSwitchableNodeLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A7D610

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template void __fastcall __unguarded_insertion_sort_aux(SSwitchableNode*, SSwitchableNode*, SSwitchableNode*, CSwitchableNodeLess);
}