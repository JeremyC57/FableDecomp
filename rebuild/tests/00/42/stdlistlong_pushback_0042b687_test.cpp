#include <cstdio>
#include <cstdlib>
namespace std {
struct _List_node_long { _List_node_long* _M_next; _List_node_long* _M_prev; long _M_data; };
template <class T> struct allocator { };
template <class T, class A> class list {
public:
    struct iterator { _List_node_long* _M_node; iterator(_List_node_long* node) : _M_node(node) { } iterator(const iterator& other) : _M_node(other._M_node) { } };
    _List_node_long* _M_node;
    iterator end() { return iterator(_M_node); }
    iterator insert(iterator position, const T& value);
    void push_back(const T& value);
};
template <class T, class A> void list<T, A>::push_back(const T& value) { insert(end(), value); }
template <class T, class A> typename list<T, A>::iterator list<T, A>::insert(iterator position, const T& value)
{
    _List_node_long* n = (_List_node_long*)std::malloc(sizeof(_List_node_long));
    n->_M_data = value; n->_M_next = position._M_node; n->_M_prev = position._M_node->_M_prev;
    position._M_node->_M_prev->_M_next = n; position._M_node->_M_prev = n;
    return iterator(n);
}
template class list<long, allocator<long> >;
}
int main()
{
    typedef std::list<long, std::allocator<long> > L;
    std::_List_node_long sentinel; sentinel._M_next = sentinel._M_prev = &sentinel; sentinel._M_data = 0;
    L l; l._M_node = &sentinel;
    l.push_back(7); l.push_back(9);
    bool ok = sentinel._M_next->_M_data == 7 && sentinel._M_next->_M_next->_M_data == 9 && sentinel._M_prev->_M_data == 9
           && sentinel._M_next->_M_next->_M_next == &sentinel && sentinel._M_prev->_M_prev->_M_data == 7 && sentinel._M_next->_M_prev == &sentinel;
    std::free(sentinel._M_next->_M_next); std::free(sentinel._M_next);
    std::printf(ok ? "0042b687_TEST PASS\n" : "0042b687_TEST FAIL\n");
    return ok ? 0 : 1;
}
