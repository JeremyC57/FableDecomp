#include <stdio.h>
namespace NUISystem { class CList; }
namespace std {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* x) : _M_node(x) {}
};
template <class T, class A> class list {
public:
    _List_node_base* _M_node;
    typedef _List_iterator<T> iterator;
    iterator begin() { return iterator(_M_node->_M_next); }
};
template <class T> class allocator {};
}
int main()
{
    std::_List_node_base head, a;
    head._M_next = &a; head._M_prev = &a; a._M_next = &head; a._M_prev = &head;
    std::list<NUISystem::CList*, std::allocator<NUISystem::CList*> > l; l._M_node = &head;
    bool ok = l.begin()._M_node == &a;
    head._M_next = &head; ok = ok && l.begin()._M_node == &head;
    printf(ok ? "00429a85_TEST PASS\n" : "00429a85_TEST FAIL\n");
    return ok ? 0 : 1;
}