// behaviour test for 0x0042B945
#include <stdio.h>
namespace NUISystem { class CActionParam { public: int v; }; }
namespace std {
struct __false_type { __false_type() {} };
template <class T> class allocator { public: allocator() {} };
template <class T> struct _Nonconst_traits {};
template <class T> struct _Const_traits {};
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
    bool operator!=(const _List_iterator_base& y) const { return _M_node != y._M_node; }
};
template <class T, class Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) {}
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) {}
};
template <class T, class A = allocator<T> > class list {
public:
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    typedef _List_iterator<T, _Const_traits<T> >    const_iterator;
    _List_node_base* _M_node;
    iterator insert(iterator pos, const T& x)
    {
        _List_node<T>* n = new _List_node<T>; n->_M_data = x;
        n->_M_next = pos._M_node; n->_M_prev = pos._M_node->_M_prev;
        pos._M_node->_M_prev->_M_next = n; pos._M_node->_M_prev = n;
        return iterator(n);
    }
    template <class InputIter>
    void _M_insert_dispatch(iterator pos, InputIter first, InputIter last, const __false_type&)
    {
        for (; first != last; first._M_node = first._M_node->_M_next)
            insert(pos, ((_List_node<T>*)first._M_node)->_M_data);
    }
    template <class InputIter>
    void insert(iterator pos, InputIter first, InputIter last)
    {
        _M_insert_dispatch(pos, first, last, __false_type());
    }
};
template void list<NUISystem::CActionParam*>::insert<list<NUISystem::CActionParam*>::const_iterator>(
    list<NUISystem::CActionParam*>::iterator, list<NUISystem::CActionParam*>::const_iterator,
    list<NUISystem::CActionParam*>::const_iterator);
}

typedef std::list<NUISystem::CActionParam*> L;
int main()
{
    std::_List_node_base src; src._M_next = src._M_prev = &src;
    std::_List_node_base dst; dst._M_next = dst._M_prev = &dst;
    L a; a._M_node = &src; L b; b._M_node = &dst;
    NUISystem::CActionParam p1, p2;
    a.insert(L::iterator(&src), &p1); a.insert(L::iterator(&src), &p2);
    b.insert(L::iterator(dst._M_next), L::const_iterator(src._M_next), L::const_iterator(&src));
    std::_List_node_base* n1 = dst._M_next; std::_List_node_base* n2 = n1->_M_next;
    int ok = ((std::_List_node<NUISystem::CActionParam*>*)n1)->_M_data == &p1
          && ((std::_List_node<NUISystem::CActionParam*>*)n2)->_M_data == &p2 && n2->_M_next == &dst;
    printf(ok ? "0042b945_TEST PASS\n" : "0042b945_TEST FAIL\n");
    return ok ? 0 : 1;
}