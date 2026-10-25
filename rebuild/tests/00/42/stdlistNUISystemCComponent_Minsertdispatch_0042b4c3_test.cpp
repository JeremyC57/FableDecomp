// test for 0042b4c3 list range insert
#include <stdio.h>
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

static int g_n; static NUISystem::CComponent* g_vals[8]; static std::_List_node_base* g_pos[8];
template <class T> typename std::list<T>::iterator std::list<T>::insert(iterator pos, const T& x) { g_pos[g_n] = pos._M_node; g_vals[g_n++] = x; return pos; }
int main()
{
    std::_List_node<NUISystem::CComponent*> n[4]; std::_List_node_base head;
    head._M_next = &n[0]; n[0]._M_next = &n[1]; n[1]._M_next = &n[2]; n[2]._M_next = &head;
    n[0]._M_data = (NUISystem::CComponent*)1; n[1]._M_data = (NUISystem::CComponent*)2; n[2]._M_data = (NUISystem::CComponent*)3;
    std::list<NUISystem::CComponent*> l; l._M_node = &head;
    std::list<NUISystem::CComponent*>::iterator pos; pos._M_node = &n[3];
    std::list<NUISystem::CComponent*>::const_iterator f; f._M_node = &n[0];
    std::list<NUISystem::CComponent*>::const_iterator e; e._M_node = &head;
    l._M_insert_dispatch(pos, f, e, std::__false_type());
    int ok = g_n == 3 && g_vals[0] == (NUISystem::CComponent*)1 && g_vals[2] == (NUISystem::CComponent*)3 && g_pos[0] == &n[3] && g_pos[2] == &n[3];
    l._M_insert_dispatch(pos, e, e, std::__false_type());
    ok = ok && g_n == 3;
    printf(ok ? "0042b4c3_TEST PASS\n" : "0042b4c3_TEST FAIL\n");
    return 0;
}