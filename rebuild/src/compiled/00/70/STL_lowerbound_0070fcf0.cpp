// std::__lower_bound(first, last, val, comp, int*) -- STLport binary search over a vector of list<CAnimSet>::const_iterator, ordered by CAnimSetItPriorityLessThan (the anim set's Priority)
// Leaf: ??$__lower_bound@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070FCF0, 60 bytes (+ int3 padding), __fastcall (first ecx, last edx), `ret 0Ch`
//   (val, the empty comparator, the null distance-type pointer).
// Identity: STLport 4.6 `__lower_bound(__first, __last, __val, __comp, _Distance*)` (len = distance, halve, advance middle,
//   `__comp(*__middle, __val)`); the `lower_bound(first, last, val, comp)` forwarder 0x007102B0 pushes the null `int*`, the
//   comparator and val and calls it; the stable_sort merge helpers (0x0071128D, 0x00711390) use it. Twin: __upper_bound
//   0x0070FD30. The element is list<CAnimSet>::const_iterator (FableWin.pdb ??$stable_sort ...CAnimSetItPriorityLessThan),
//   compared on CAnimSet::Priority (Ego_r.pdb +0x54; retail +0x4C with 12-byte STLport vectors = node +0x54).
// Shape: `__fastcall` on the template definition (the /Gr STLport algorithm TU); the comparator is inline, so val's priority
//   is hoisted into ecx ahead of the loop; `__len = __len - __half - 1` gives `or edi,-1 ; sub edi,edx ; add esi,edi`.
class CAnimSet {                                        // retail layout (12-byte STLport vectors)
public:
    unsigned char _pad00[0x4c];
    long          Priority;                             // +0x4C (Ego_r.pdb CAnimSet::Priority)
    unsigned char Type;                                 // +0x50
    unsigned char Bits;                                 // +0x51 (Looping, Finished, Forwards, HadUpdate, OwnedByMainAnim)
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
_ForwardIter __fastcall __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp, _Distance*)
{
    _Distance __len = __last - __first;
    _Distance __half;
    _ForwardIter __middle;

    while (__len > 0) {
        __half = __len >> 1;
        __middle = __first;
        __middle += __half;
        if (__comp(*__middle, __val)) {
            __first = __middle;
            ++__first;
            __len = __len - __half - 1;
        }
        else
            __len = __half;
    }
    return __first;
}

} // namespace std

typedef std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> > CAnimSetIt;

class CAnimSetItPriorityLessThan {
public:
    bool operator()(const CAnimSetIt& a, const CAnimSetIt& b) const { return (*a).Priority < (*b).Priority; }
};

template CAnimSetIt* __fastcall std::__lower_bound(CAnimSetIt*, CAnimSetIt*, const CAnimSetIt&, CAnimSetItPriorityLessThan, int*);