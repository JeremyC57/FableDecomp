// behaviour test for 0x00429418
#include <stdio.h>
namespace NUISystem { class CComponent { public: int x; }; }
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

typedef std::_ComponentListIter It;
int main()
{
    NUISystem::CComponent a, b, c;
    std::_List_node_base head; std::_List_node<NUISystem::CComponent*> n1, n2;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &head; n1._M_data = &a; n2._M_data = &b;
    std::input_iterator_tag tag;
    NUISystem::CComponent* va = &a; NUISystem::CComponent* vb = &b; NUISystem::CComponent* vc = &c;
    It r1 = std::__find(It(&n1), It(&head), vb, tag);
    It r2 = std::__find(It(&n1), It(&head), va, tag);
    It r3 = std::__find(It(&n1), It(&head), vc, tag);
    It r4 = std::__find(It(&head), It(&head), va, tag);
    int ok = r1._M_node == &n2 && r2._M_node == &n1 && r3._M_node == &head && r4._M_node == &head;
    printf(ok ? "00429418_TEST PASS\n" : "00429418_TEST FAIL\n");
    return ok ? 0 : 1;
}