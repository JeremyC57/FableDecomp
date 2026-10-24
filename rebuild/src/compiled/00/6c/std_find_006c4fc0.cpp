// std::find(first, last, const T& val) -- STLport linear search of a list<NUISystem::CComponent*> for a pointer value
// Leaf: find
// Retail: Fable.exe @ 0x006C4FC0, 34 bytes, `ret 8`: __fastcall with the hidden iterator result in ecx, val by
// reference in edx and the two list iterators (one node pointer each, user constructors) on the stack. Unlike the
// __find twins (0x00429418, 0x00535F38: `ret 0Ch`, a third tag slot) this body takes no category tag: its only call
// site 0x006C6318 pushes just first (head->_M_next) and last (the head node) -- STLport find() with the input-iterator
// __find inlined into it. List node: _M_next +0, _M_prev +4, data +8.
// Opt: speed TU (`last` cached in ecx, `cmp esi,ecx`; the value is loaded once after the empty-range test).
namespace NUISystem { class CComponent; }
namespace std {
template <class T> struct _Nonconst_traits {};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node == y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node != y._M_node; }
};
template <class T, class Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) {}
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) {}
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};
template <class InputIter, class T>
InputIter __fastcall find(InputIter first, InputIter last, const T& val)
{
    while (first != last && !(*first == val))
        ++first;
    return first;
}
typedef _List_iterator<NUISystem::CComponent*, _Nonconst_traits<NUISystem::CComponent*> > _ComponentListIter;
template _ComponentListIter __fastcall find(_ComponentListIter, _ComponentListIter, NUISystem::CComponent* const&);
}