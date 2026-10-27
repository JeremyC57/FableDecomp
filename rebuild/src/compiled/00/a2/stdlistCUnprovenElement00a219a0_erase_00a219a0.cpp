// std::list<CUnprovenElement_00a219a0>::erase(iterator first, iterator last) -- STLport range erase: unlink and free every node from first up to last, return last
// Leaf: ?erase@?$list@VCUnprovenElement_00a219a0@@V?$allocator@VCUnprovenElement_00a219a0@@@std@@@std@@QAE?AU?$_List_iterator@VCUnprovenElement_00a219a0@@U?$_Nonconst_traits@VCUnprovenElement_00a219a0@@@std@@@2@U32@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a219a0, 54 bytes, thiscall, iterator result through the hidden pointer, `ret 0Ch`.
// Identity (descriptive): no call, jump or data reference reaches it (unreferenced COMDAT); the neighbouring list constructor 0x00A21980 allocates 20-byte nodes (a 12-byte element), and its own bytes are the trivially-destroyed list erase(first, last); the element type is unproven.
// Callee: free 0x00BFEA14.
// Shape (family L034): `while (first != last) erase(first++); return last;` with the single-node erase inline; the
//   element's destructor is trivial.
class CUnprovenElement_00a219a0 {                         // descriptive: 12 bytes, trivially destroyed
public:
    unsigned long Words[3];                               // +0x00
};
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;                             // +0x0
    _List_node_base* _M_prev;                             // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                          // +0x8
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Self operator++(int)
    {
        _Self __tmp = *this;
        this->_M_incr();
        return __tmp;
    }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, allocator<_Node> > _M_node;   // +0x00: the sentinel node
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
    iterator erase(iterator __first, iterator __last);
};
template <class _Tp, class _Alloc>
typename list<_Tp, _Alloc>::iterator list<_Tp, _Alloc>::erase(iterator __first, iterator __last)
{
    while (__first != __last)
        erase(__first++);
    return __last;
}
}
template std::list<CUnprovenElement_00a219a0 >::iterator std::list<CUnprovenElement_00a219a0 >::erase(std::list<CUnprovenElement_00a219a0 >::iterator, std::list<CUnprovenElement_00a219a0 >::iterator);