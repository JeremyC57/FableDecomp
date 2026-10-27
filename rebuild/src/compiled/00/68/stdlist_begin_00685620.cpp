// std::list::begin() -- an iterator on the first element (the sentinel node's next pointer), returned by hidden pointer
// Leaf: ?begin@?$list@VCUnprovenValue_00685620@@V?$allocator@VCUnprovenValue_00685620@@@std@@@std@@QAE?AU?$_List_iterator@VCUnprovenValue_00685620@@U?$_Nonconst_traits@VCUnprovenValue_00685620@@@std@@@2@XZ
// Retail: Fable.exe @ 0x00685620, 13 bytes: `mov eax,[ecx] ; mov ecx,[eax] ; mov eax,[esp+4] ; mov [eax],ecx ; ret 4`.
// Identity (descriptive element type): STLport 4.6 `list::begin()` -- the 4-byte list holds the sentinel node pointer
// (`_STLP_alloc_proxy` over the empty allocator), `begin()` builds `iterator(_M_node._M_data->_M_next)`; the iterator has
// user constructors, so it comes back through the hidden result pointer (`ret 4`), the shape of the landed
// std::list<NUISystem::CList*>::begin 0x00429A85. The row is an unreferenced gap-scan COMDAT (the four "abs" xrefs at
// 0x00F216A4.. are false hits in instruction bytes) between out-of-line STL pieces of the CTCCreatureOpinion area, so the
// element type is not provable: CUnprovenValue_00685620.
class CUnprovenValue_00685620 { public: long _value; };     // descriptive: element type unproven
namespace std {
template <class _Tp> class allocator { public: };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
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
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp  value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) {}
};
template <class _Tp, class _Alloc> class _List_base { public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, _Alloc> _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator((_List_node<_Tp>*)(this->_M_node._M_data->_M_next)); }
};
template class list<CUnprovenValue_00685620>;
}