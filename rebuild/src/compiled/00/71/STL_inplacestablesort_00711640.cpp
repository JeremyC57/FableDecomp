// std::__inplace_stable_sort(list<CAnimSet>::const_iterator* first, list<CAnimSet>::const_iterator* last, CAnimSetItPriorityLessThan comp) -- in-place stable sort of anim-set iterators by priority: insertion sort below 15 elements, else sort both halves recursively and merge them without a buffer
// Leaf: ??$__inplace_stable_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00711640, length 99 bytes (+ int3 padding). __fastcall (first ecx, last edx,
//   the 4-byte comparator slot on the stack), `ret 4`.
// Callers: itself (0x00711672 / 0x0071167C) and std::__stable_sort_aux 0x00711B80 (landed; 0x00711BCC, the no-buffer arm).
// Callees: __insertion_sort @ 0x00711190 (landed), __merge_without_buffer @ 0x00711230 (both __fastcall, the comparator
//   slot pushed by value; __merge_without_buffer takes last, len1 = middle - first, len2 = last - middle and comp on the stack).
// Label: catalog "std::introsort_CAnimSet" is descriptive and the algorithm is not an introsort: it is STLport's
//   `__inplace_stable_sort` (the landed __stable_sort_aux 0x00711B80 carries the same text as its same-TU stand-in).
// Element: list<CAnimSet>::const_iterator (4 bytes, the node pointer); comparator CAnimSetItPriorityLessThan (empty).
// Shape: STLport 4.6 `_algo.c` verbatim: `if (__last - __first < 15) { __insertion_sort(__first, __last, __comp); return;
//   } _RandomAccessIter __middle = __first + (__last - __first) / 2; ...` (the signed halving `cdq ; sub ; sar`).
class CAnimSet {                                        // retail layout (Ego_r.pdb minus 8: 12-byte vectors)
public:
    unsigned char _before[0x4C];                        // UpdateMode, Anims, InternalAlphas, ... AnimDelay
    long Priority;                                      // +0x4C
    unsigned char _after[0x08];                         // Type, the flag bits
};

namespace std {
template <class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator_base() {}
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator() {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
};
}
typedef std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> > CAnimSetConstIterator;   // list<CAnimSet>::const_iterator

struct CAnimSetItPriorityLessThan {                     // empty functor (a 4-byte argument slot)
    bool operator()(const CAnimSetConstIterator& a, const CAnimSetConstIterator& b) const { return (*a).Priority < (*b).Priority; }
};

namespace std {
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);        // 0x00711190
template <class _BidirectionalIter, class _Distance, class _Compare>
void __fastcall __merge_without_buffer(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last,
                                       _Distance __len1, _Distance __len2, _Compare __comp);                  // 0x00711230

template <class _RandomAccessIter, class _Compare>
void __fastcall __inplace_stable_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first < 15) {
        __insertion_sort(__first, __last, __comp);
        return;
    }
    _RandomAccessIter __middle = __first + (__last - __first) / 2;
    __inplace_stable_sort(__first, __middle, __comp);
    __inplace_stable_sort(__middle, __last, __comp);
    __merge_without_buffer(__first, __middle, __last, __middle - __first, __last - __middle, __comp);
}

template void __fastcall __inplace_stable_sort<CAnimSetConstIterator*, CAnimSetItPriorityLessThan>(
    CAnimSetConstIterator*, CAnimSetConstIterator*, CAnimSetItPriorityLessThan);
}