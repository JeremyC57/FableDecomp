// std::list<CClass*>::_M_insert_dispatch<std::_List_iterator<CClass*, _Const_traits<CClass*> > >(iterator, const_iterator, const_iterator, const __false_type&) -- insert a copy of each element of [first, last) before pos
// Leaf: ??$_M_insert_dispatch@
// Retail: Fable.exe @ 0x0042B4C3, 48 bytes, `ret 10h` (pos, first, last and the __false_type tag); 12 callers (0x0042B95A,
// 0x005325F6, 0x0053268D, ...). STLport's range-insert loop: while first != last, insert(pos, *first) (0x0042AB8A, which
// returns an iterator through a hidden pointer -- the dead `first` slot is reused for it) and ++first (node +0 next,
// value at node +8). The catalog's element type CClass* is kept; the 4-byte pointer element does not reach the bytes.
// Layout: STLport list (one sentinel-node pointer), node {next +0, prev +4, data +8}; iterators are one node pointer
// with a user copy constructor (the pos copy is built in its argument slot: `push ecx ; mov eax,esp ; mov [eax],ecx`).
// Opt: size TU with frame pointers (`optimize("y", off)`: push ebp / mov ebp,esp; unaligned entry).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CClass;
namespace std {
struct __false_type {};
struct _List_node_base {
    _List_node_base* _M_next;                            // +0x00
    _List_node_base* _M_prev;                            // +0x04
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;                                           // +0x08
};
template <class T> struct _Nonconst_traits { typedef T& reference; };
template <class T> struct _Const_traits { typedef const T& reference; };
template <class T, class Traits> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator() {}
    _List_iterator(const _List_iterator& x) : _M_node(x._M_node) {}
    template <class Tr2> _List_iterator(const _List_iterator<T, Tr2>& x) : _M_node(x._M_node) {}
    typename Traits::reference operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class T> class list {
public:
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    typedef _List_iterator<T, _Const_traits<T> >    const_iterator;
    _List_node_base* _M_node;
    iterator insert(iterator pos, const T& x);           // 0x0042AB8A
    template <class InputIter>
    void _M_insert_dispatch(iterator pos, InputIter first, InputIter last, const __false_type&)
    {
        for (; first != last; ++first)
            insert(pos, *first);
    }
};
}
template void std::list<CClass*>::_M_insert_dispatch<std::list<CClass*>::const_iterator>(std::list<CClass*>::iterator, std::list<CClass*>::const_iterator, std::list<CClass*>::const_iterator, const std::__false_type&);