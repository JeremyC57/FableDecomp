// std::list::_M_insert_dispatch<const_iterator>(iterator pos, const_iterator first, const_iterator last, const __false_type&) -- list<NUISystem::CComponent*>: insert a copy of each element of [first, last) before pos
// Leaf: ??$_M_insert_dispatch@U?$_List_iterator@PAVCComponent@NUISystem@@U?$_Const_traits@PAVCComponent@NUISystem@@@std@@@std@@@?$list@PAVCComponent@NUISystem@@
// Retail: Fable.exe @ 0x0042B4C3, 48 bytes, `ret 10h` (pos, first, last and the __false_type tag); 12 callers (0x0042B95A,
// 0x005325F6, 0x0053268D, ...). STLport's range-insert loop: while first != last, insert(pos, *first) (0x0042AB8A, which
// returns an iterator through a hidden pointer -- the dead `first` slot is reused for it) and ++first (node +0 next,
// value at node +8). Element type (sweep2): NUISystem::CComponent* -- callers are list<CComponent*>::operator= 0x00535800 and the range insert 0x0042B945 under CDeletion's list copy ctor (Ego_r CDeletion::AssociatedParents list<CComponent*>); was the placeholder NUISystem::CComponent*.
// Layout: STLport list (one sentinel-node pointer), node {next +0, prev +4, data +8}; iterators are one node pointer
// with a user copy constructor (the pos copy is built in its argument slot: `push ecx ; mov eax,esp ; mov [eax],ecx`).
// Opt: size TU with frame pointers (`optimize("y", off)`: push ebp / mov ebp,esp; unaligned entry).
#pragma optimize("s", on)
#pragma optimize("y", off)
namespace NUISystem { class CComponent; }
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
template void std::list<NUISystem::CComponent*>::_M_insert_dispatch<std::list<NUISystem::CComponent*>::const_iterator>(std::list<NUISystem::CComponent*>::iterator, std::list<NUISystem::CComponent*>::const_iterator, std::list<NUISystem::CComponent*>::const_iterator, const std::__false_type&);