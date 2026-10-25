// std::__upper_bound(first, last, val, comp, int*) -- STLport binary search over a vector of list<CAnimSet>::const_iterator ordered by CAnimSetItPriorityLessThan: the first element whose anim-set Priority is greater than val's
// Leaf: ??$__upper_bound@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070FD30, 60 bytes, __fastcall (first ecx, last edx), `ret 0Ch` (val by reference,
//   the empty comparator by value, the null distance-type pointer).
// Identity: twin of the landed STLport __lower_bound 0x0070FCF0 in the same /Gr anim-set stable_sort TU (FableWin.pdb
//   ??$stable_sort ...CAnimSetItPriorityLessThan); the merge helpers call upper_bound for the second half. The element
//   is list<CAnimSet>::const_iterator (a node pointer) compared on CAnimSet::Priority (Ego_r.pdb +0x54; retail +0x4C with
//   12-byte STLport vectors = node +0x54). Label: the catalog's Dinkumware spelling is replaced by the STLport one.
// Shape: STLport 4.6 `__upper_bound` (`if (__comp(__val, *__middle)) __len = __half; else { __first = __middle;
//   ++__first; __len = __len - __half - 1; }`); the comparator is inline, so val's priority is hoisted into ecx ahead
//   of the loop; `__len - __half - 1` gives `or edi,-1 ; sub edi,edx ; add esi,edi`.
class CAnimSet {                                        // retail layout (12-byte STLport vectors)
public:
    unsigned char _pad00[0x4c];
    long          Priority;                             // +0x4C (Ego_r.pdb CAnimSet::Priority)
    unsigned char Type;                                 // +0x50
    unsigned char Bits;                                 // +0x51
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
};

template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_node<_Tp> _Node;
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
};

template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __upper_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp, _Distance*)
{
    _Distance __len = __last - __first;
    _Distance __half;
    _ForwardIter __middle;

    while (__len > 0) {
        __half = __len >> 1;
        __middle = __first;
        __middle += __half;
        if (__comp(__val, *__middle))
            __len = __half;
        else {
            __first = __middle;
            ++__first;
            __len = __len - __half - 1;
        }
    }
    return __first;
}

} // namespace std

typedef std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> > CAnimSetIt;

class CAnimSetItPriorityLessThan {
public:
    bool operator()(const CAnimSetIt& a, const CAnimSetIt& b) const { return (*a).Priority < (*b).Priority; }
};

template CAnimSetIt* __fastcall std::__upper_bound(CAnimSetIt*, CAnimSetIt*, const CAnimSetIt&, CAnimSetItPriorityLessThan, int*);