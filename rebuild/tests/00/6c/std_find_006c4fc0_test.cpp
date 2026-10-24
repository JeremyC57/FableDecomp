// behaviour test for 0x006C4FC0 (STLport find over a list of component pointers)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
namespace NUISystem { class CComponent { public: int x; }; }
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

typedef std::_ComponentListIter It;
int main()
{
    NUISystem::CComponent a, b, c;
    std::_List_node_base head; std::_List_node<NUISystem::CComponent*> n1, n2;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &head; n1._M_data = &a; n2._M_data = &b;
    NUISystem::CComponent* va = &a; NUISystem::CComponent* vb = &b; NUISystem::CComponent* vc = &c;
    It r1 = std::find(It(&n1), It(&head), vb);
    It r2 = std::find(It(&n1), It(&head), va);
    It r3 = std::find(It(&n1), It(&head), vc);
    It r4 = std::find(It(&head), It(&head), va);
    int ok = r1._M_node == &n2 && r2._M_node == &n1 && r3._M_node == &head && r4._M_node == &head;
    if (!ok) { printf("006c4fc0_TEST FAIL\n"); return 1; }
    printf("006c4fc0_TEST PASS\n");
    return 0;
}