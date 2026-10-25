// std::list<long>::pop_back() -- STLport: erase the last element (the node before the sentinel)
// Leaf: ?pop_back@?$list@JV?$allocator@J@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005791B5, 23 bytes, __fastcall, `ret`; size-optimised TU (no alignment padding).
// Identity: a folded COMDAT shared by every POD list. It sits in the CTCInventoryExperience TU, and its first callers
//   (0x0056F572..0x0056F588) pop the four list<long> members of SExpSpendUndo (Ego_r.pdb: Generals +0x00, Strengths
//   +0x0C, Skills +0x18, Wills +0x24 -- 4-byte lists in retail); 0x006463EA tail-jumps to it. Label correction: the
//   catalog's "list<CVsyncCallbackTimer::CPendingQuery>::pop_back" names one of the folded instantiations; the TU's own
//   one is list<long>. The bytes do not depend on the element type.
// Callees: std::list<long>::erase(iterator) @ 0x00429B02 (out of line, size TU: unlink, free the node, return the
//   next; hidden result pointer + the by-value iterator, `ret 8`).
// Shape: STLport 4.6 verbatim `iterator __tmp = end(); erase(--__tmp);`: `--__tmp` is the inline _M_decr (the
//   sentinel's _M_prev), the by-value iterator argument (a user copy ctor) is built in its stack slot
//   (`push ecx ; mov eax,esp ; mov [eax],edx`) and the dropped result lands in the frame word reserved first.
#pragma optimize("s", on)

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

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

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
};

template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Self& operator--() { this->_M_decr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator end() { return this->_M_node._M_data; }
    iterator erase(iterator __position);                 // 0x00429B02
    void pop_back()
    {
        iterator __tmp = end();
        erase(--__tmp);
    }
};

template void list<long>::pop_back();

}