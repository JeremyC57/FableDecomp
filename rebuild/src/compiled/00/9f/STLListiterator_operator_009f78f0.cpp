// _STL::_List_iterator::operator++() -- STLport prefix operator++ of list<CVsyncCallbackTimer::CPendingQuery>'s mutable iterator: step to the next node, return *this
// Leaf: ??E?$_List_iterator@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009f78f0, 22 bytes (+ int3 padding), __fastcall, plain `ret`, returns `this`.
// Identity: the body sits in the unoptimised CVsyncCallbackTimer TU (0x009F7020..0x009F7F40, landed relatives: the iterator
//   copy ctor 0x009F7760, list begin/end/back/size/erase/push_* around it), whose only list is
//   list<CVsyncCallbackTimer::CPendingQuery>. It calls the optimised shared _List_iterator_base::_M_incr @ 0x0041C7A0
//   (`_M_node = _M_node->_M_next`) and hands back `this`: STLport 4.6 `_Self& operator++() { this->_M_incr(); return *this; }`.
//   No direct callers (the TU's members are all emitted; the landed back() 0x009F7C20 also has none). The catalog label
//   "list<CMessageEvent const*> constructor" is propagated. Const vs mutable traits cannot be told from the bytes; the mutable
//   iterator is the one the landed copy ctor 0x009F7760 instantiates.
// Pragma: #pragma optimize("", off) -- `this` homed at [ebp-4], reloaded into ecx for the call and into eax for the return.
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

namespace _STL {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    void _M_incr();                                     // 0x0041C7A0
    void _M_decr();                                     // 0x0041C7B0
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };

template <class _Tp, class _Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    _Self& operator++() { this->_M_incr(); return *this; }
};

}

template _STL::_List_iterator<CVsyncCallbackTimer::CPendingQuery, _STL::_Nonconst_traits<CVsyncCallbackTimer::CPendingQuery> >&
    _STL::_List_iterator<CVsyncCallbackTimer::CPendingQuery, _STL::_Nonconst_traits<CVsyncCallbackTimer::CPendingQuery> >::operator++();