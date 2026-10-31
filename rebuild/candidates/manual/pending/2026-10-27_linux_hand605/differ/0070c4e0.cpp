// CAnimateThing::GetFirstCombinationAnimFromType(long type) -- the first anim set of this type in the current animation's combination list, else in the delayed animation's combination list, else null
// Grade: DIFFER(81v81) 2 lines (unchanged in chunk 605): the shared return-0 block emits `pop edi ; xor eax,eax` where retail has `xor eax,eax ; pop edi` (pure epilogue scheduling). Tried in chunk 605: result variable (94), for/break loops (same 2), shared single `return &*it` after a nested second search (79), inverted last test (88), STLport tag-dispatched __find_if (same 2), flags /Oa /Ow /G5 /G7 /Op /Ob1 (same 2).
// Was: DIFFER(82v81) -- find_if form: the end is re-read from memory (`cmp eax,[ecx+58h]` / `[ecx+64h]`) where retail keeps it in edx / ecx (and the second loop runs in ecx/edx, not edx/edi); prologue, loops and tails otherwise match. Next: a named `end` local per list passed to find_if and compared against.
// Leaf: ?GetFirstCombinationAnimFromType@CAnimateThing@@QAEPAVCAnimSet@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070C4E0, 81 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?GetFirstCombinationAnimFromType@CAnimateThing@@QAEPAVCAnimSet@@J@Z sits beside
//   GetCombinationAnimFromTypeAndIndex (landed 0x0070C170, the same two lists with an extra index test) in the donor
//   order; the landed callers (e.g. CModeStrafe 0x008E97B0) call this address with a long type. Landed callers declare it as
//   an invented "CAnimateThing::FindCombinationAnim" (label correction, see NOTES).
// Layout: Ego_r.pdb CAnimateThing with retail STLport sizes: CurrentAnim (CCompleteAnimation, 0x60: MainAnimSet 0x54,
//   TransitionAnimSets +0x54, CombinationAnims +0x58), DelayedAnimQueue list +0x60, DelayedAnim (CDelayedAnimation:
//   CombinationAnims list) +0x64. A list node is {next, prev, CAnimSet}; CAnimSet::Type at +0x50 (node +0x58).
// Shape: each search is a for loop that breaks on a match, then `if (it != end()) return &*it;` -- the exhausted first loop
//   jumps past its own end test (`jmp`) and the second's match re-tests the end before the `add eax,8` tail.
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp, class _Traits> struct _List_iterator {
    typedef _List_node<_Tp> _Node;
    _List_node_base* _M_node;
    _List_iterator() { }
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    typename _Traits::reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    typename _Traits::pointer operator->() const { return &(operator*()); }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator==(const _List_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, _Alloc> _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
};
}

class CAnimSet {
public:
    unsigned char _pad00[0x50];
    unsigned char Type;                                     // +0x50
    unsigned char Flags;                                    // +0x51
    unsigned char _pad52[2];
};

class CAnimateThing {
public:
    class CCompleteAnimation {
    public:
        CAnimSet             MainAnimSet;                   // +0x00
        std::list<CAnimSet>  TransitionAnimSets;            // +0x54
        std::list<CAnimSet>  CombinationAnims;              // +0x58
        bool                 MainAnimCanTransitionOut;      // +0x5C
    };
    class CDelayedAnimation {
    public:
        std::list<CAnimSet>  CombinationAnims;              // +0x00
    };
    CCompleteAnimation                CurrentAnim;          // +0x00
    std::list<CDelayedAnimation>      DelayedAnimQueue;     // +0x60
    CDelayedAnimation                 DelayedAnim;          // +0x64
    CAnimSet* GetFirstCombinationAnimFromType(long type);
};

namespace std {
template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
}
}
struct CIsAnimSetOfType {
    long Type;
    CIsAnimSetOfType(long type) : Type(type) { }
    bool operator()(const CAnimSet& set) const { return set.Type == Type; }
};

CAnimSet* CAnimateThing::GetFirstCombinationAnimFromType(long type)
{
    std::list<CAnimSet>::iterator end = CurrentAnim.CombinationAnims.end();
    std::list<CAnimSet>::iterator it = std::find_if(CurrentAnim.CombinationAnims.begin(), end, CIsAnimSetOfType(type));
    if (it != end)
        return &*it;
    end = DelayedAnim.CombinationAnims.end();
    it = std::find_if(DelayedAnim.CombinationAnims.begin(), end, CIsAnimSetOfType(type));
    if (it != end)
        return &*it;
    return 0;
}
