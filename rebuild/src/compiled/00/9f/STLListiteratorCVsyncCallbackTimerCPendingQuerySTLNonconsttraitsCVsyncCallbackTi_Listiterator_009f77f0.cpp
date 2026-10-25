// std::_List_iterator<CPendingQuery, _Nonconst_traits<CPendingQuery> >::_List_iterator(_Node* x) -- the list iterator's node constructor (unoptimised TU): hand the node to the shared _List_iterator_base constructor
// Leaf: ??0?$_List_iterator@UCPendingQuery@CVsyncCallbackTimer@@U?$_Nonconst_traits@UCPendingQuery@CVsyncCallbackTimer@@@std@@@std@@QAE@PAU?$_List_node@UCPendingQuery@CVsyncCallbackTimer@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F77F0, 28 bytes (+ int3 padding), thiscall `ret 4`, returns `this`: `push ebp ;
//   mov ebp,esp ; push ecx ; mov [ebp-4],ecx ; mov eax,[ebp+8] ; push eax ; mov ecx,[ebp-4] ; call 0x0041C780 ;
//   mov eax,[ebp-4]` -- the node pointer itself is passed (the copy constructor 0x009F7760 passes `x._M_node` instead).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 455 "0x9F77F0/0x9F7810 landed as operator&&<CPredicate_And<...>> are /Od
//   list-iterator ctors calling 0x41C780"): the body sits in the unoptimised CVsyncCallbackTimer TU (0x009F7020..0x009F7F40),
//   whose only list is list<CVsyncCallbackTimer::CPendingQuery> (landed relatives: the iterator copy ctor 0x009F7760,
//   operator-- / operator++ 0x009F7780 / 0x009F78F0, begin/end/back/erase/pop_back); the callee 0x0041C780 is
//   _List_iterator_base(_List_node_base*) (reloc_bindings ??0_List_iterator_base@std@@QAE@PAU_List_node_base@1@@Z from the
//   landed CVsyncCallbackTimer::ClearAllQueries 0x009F7250; renamed this pass). STLport 4.6 _list.h:
//   `_List_iterator(_Node* __x) : _List_iterator_base(__x) {}`. No code references reach it (the TU's members are all emitted).
// Traits: the twins 0x009F77F0 / 0x009F7810 are the mutable and const iterators' node constructors; the bytes cannot tell
//   which is which, so this row takes _Nonconst_traits by address order and its twin 0x009F7810 takes _Const_traits.
// Namespace: std as the TU's landed operator-- / operator++ rows and the 0x0041C780 binding.
// Pragma: #pragma optimize("", off) -- `this` homed at [ebp-4], reloaded for the call and the return.
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
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; };

template <class _Tp, class _Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
};

}

template std::_List_iterator<CVsyncCallbackTimer::CPendingQuery, std::_Nonconst_traits<CVsyncCallbackTimer::CPendingQuery> >::
    _List_iterator(std::_List_node<CVsyncCallbackTimer::CPendingQuery>*);
