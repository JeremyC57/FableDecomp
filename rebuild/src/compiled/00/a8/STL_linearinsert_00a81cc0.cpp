// std::__linear_insert(SSwitchableNode* first, SSwitchableNode* last, SSwitchableNode val, CSwitchableNodeLess comp) -- insertion-sort step over the 16-byte (unsigned __int64 node ID, switchable node pointer) entries: a value below the first entry shifts [first, last) up one slot and lands at first, anything else goes through the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A81CC0, 104 bytes, `ret 14h` (/Gr: first in ecx, last in edx; the 16-byte value and
//   the empty comparator's slot on the stack).
// Callees: __copy_backward(first, last, last + 1, random_access_iterator_tag, (int*)0) @ 0x00A7D8A0 (`ret 0Ch`; the tag is
//   homed in the comparator's argument slot, dead on that path), __unguarded_linear_insert @ 0x00A7D610 (this chunk).
// Identity: pair<unsigned __int64, CANavQuadTreeNode*> = the CNavQuadTree SwitchableNodes entries (dword copy of the pointer; 0x00A7D610 takes the same pair). Twin: 0x00A81C50 (the bool pair; only the value word copy differs). Catalog "InsertionSort_Callback_*" is descriptive.
// Shape: STLport 4.6 verbatim -- `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` with the 64-bit key compare inline (high word `ja`/`jb`, then the low
//   word); copy_backward -> __copy_backward_ptrs(__false_type) inline -> the out-of-line random-access __copy_backward;
//   the pair's trivial assignment stores all four dwords, while the by-value argument uses pair's memberwise copy ctor.
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
class __false_type { public: __false_type() { } };
class random_access_iterator_tag { public: random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*);   // 0x00A7D8A0
template <class _BI1, class _BI2>
inline _BI2 __copy_backward_ptrs(_BI1 __first, _BI1 __last, _BI2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward_ptrs(__first, __last, __result, __false_type());
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A7D610

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
template void __fastcall __linear_insert(SSwitchableNode*, SSwitchableNode*, SSwitchableNode, CSwitchableNodeLess);
}