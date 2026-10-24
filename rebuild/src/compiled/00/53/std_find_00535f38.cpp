// std::__find(first, last, const T& val, const input_iterator_tag&) -- STLport linear search of a list<NUISystem::CComponent*> for a pointer value
// Leaf: __find
// Retail: Fable.exe @ 0x00535F38, 34 bytes, __fastcall (hidden return slot in ecx, val by reference in edx, first/last/tag on
// the stack, `ret 0Ch`). STLport __find for input iterators: `while (first != last && !(*first == val)) ++first;` over list
// nodes (_M_next +0, data +8). Only call site 0x0053C8EC, inside the STLport find(first, last, val) at 0x0053C8D0 (manifest
// label CTCCameraPointScripted::GetLookDirection is propagated) that copies both iterators and passes the tag by reference.
// Byte-identical to the landed 0x00429418 / 0x004294AC / 0x0042A77F (same source, reused verbatim); the catalog's
// NUISystem::CComponent* element is kept.
// Opt: size TU (`last` compared in memory, `cmp esi,[esp+0Ch]`).
#pragma optimize("s", on)
namespace NUISystem { class CComponent; }
namespace std {
struct input_iterator_tag {};
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
InputIter __fastcall __find(InputIter first, InputIter last, const T& val, const input_iterator_tag&)
{
    while (first != last && !(*first == val))
        ++first;
    return first;
}
typedef _List_iterator<NUISystem::CComponent*, _Nonconst_traits<NUISystem::CComponent*> > _ComponentListIter;
template _ComponentListIter __fastcall __find(_ComponentListIter, _ComponentListIter, NUISystem::CComponent* const&,
                                              const input_iterator_tag&);
}