// std::_List_iterator<CVsyncCallbackTimer::CPendingQuery, std::_Nonconst_traits<CVsyncCallbackTimer::CPendingQuery> >::
//   _List_iterator(const iterator&) -- STLport's user copy constructor of the list's mutable iterator: hand the source's node
//   to the shared _List_iterator_base constructor.
// Leaf: ??0?$_List_iterator@UCPendingQuery@CVsyncCallbackTimer@@U?$_Nonconst_traits@UCPendingQuery@CVsyncCallbackTimer@@@std@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F7760, length 30 bytes (+ int3 padding). First landed as
// CPersistTraits<CReactionFrequencyTraitsArray>::~CPersistTraits (a propagated label): a destructor takes no argument, this
// takes one by reference (`ret 4`) and returns `this`, the shape of a constructor.
// Evidence: the callee 0x0041C780 is the non-template _List_iterator_base(_List_node_base*) constructor that begin()/end()
// call in list<CPendingQuery>::size (0x009F7DC0) and CVsyncCallbackTimer::OnPostSwapScreens (0x009F73C0); STLport 4.6 spells
// `_List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}` (the GOTCHAS rows for 0x9F7DC0 / 0xADD020).
// The body sits inside the unoptimised CVsyncCallbackTimer TU (0x009F7020..0x009F7F40), whose only list is
// list<CVsyncCallbackTimer::CPendingQuery> (Ego_r.pdb members FreeQueoryObjects / PendingQueories); retail has no ICF, so
// the instantiation is that TU's own.
// Callee: _List_iterator_base::_List_iterator_base @ 0x0041C780 (optimised, shared).
// Shape: `#pragma optimize("", off)`: `this` homed at [ebp-4] and returned in eax; the one argument is read from [ebp+8].
#pragma optimize("", off)

struct IDirect3DQuery9;

class CVsyncCallbackTimer {
public:
    struct CPendingQuery {
        IDirect3DQuery9* Query;
        long             Frame;
        double           TimeStamp;
    };
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x);                      // 0x0041C780
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };

template <class _Tp, class _Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
};

}

template std::_List_iterator<CVsyncCallbackTimer::CPendingQuery, std::_Nonconst_traits<CVsyncCallbackTimer::CPendingQuery> >::
    _List_iterator(const std::_List_iterator<CVsyncCallbackTimer::CPendingQuery, std::_Nonconst_traits<CVsyncCallbackTimer::CPendingQuery> >&);
