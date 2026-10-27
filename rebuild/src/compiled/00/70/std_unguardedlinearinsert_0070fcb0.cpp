// std::__unguarded_linear_insert(list<CAnimSet>::const_iterator* last, list<CAnimSet>::const_iterator val, CAnimSetItPriorityLessThan comp) -- shift larger-priority iterators up one slot and drop val into the hole
// Leaf: ??$__unguarded_linear_insert@PAU?$_List_iterator@VCAnimSet@@U?$_Const_traits@VCAnimSet@@@std@@@std@@U12@VCAnimSetItPriorityLessThan@@@std@@YIXPAU?$_List_iterator@VCAnimSet@@U?$_Const_traits@VCAnimSet@@@std@@@0@U10@VCAnimSetItPriorityLessThan@@@Z
// Retail: Fable.exe @ 0x0070FCB0, 51 bytes, __fastcall (last in ecx), `ret 8` (the 4-byte iterator val, the empty
//   comparator).
// Identity (probable): the STLport 4.6 `__unguarded_linear_insert(last, val, comp)` loop; its caller is the out-of-line
//   `__linear_insert` at 0x00710C80.. (the `else` arm at 0x00710CC7 passes val and the comparator and calls this), and
//   the element/comparator are those of the landed neighbours __lower_bound 0x0070FCF0 / __insertion_sort 0x00711190:
//   list<CAnimSet>::const_iterator ordered by CAnimSetItPriorityLessThan (CAnimSet::Priority, node +0x54).
class CAnimSet {                                        // retail layout (12-byte STLport vectors)
public:
    unsigned char _pad00[0x4c];
    long          Priority;                             // +0x4C (Ego_r.pdb CAnimSet::Priority)
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

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

} // namespace std

typedef std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> > CAnimSetIt;

class CAnimSetItPriorityLessThan {
public:
    bool operator()(const CAnimSetIt& a, const CAnimSetIt& b) const { return (*a).Priority < (*b).Priority; }
};

template void __fastcall std::__unguarded_linear_insert(CAnimSetIt*, CAnimSetIt, CAnimSetItPriorityLessThan);