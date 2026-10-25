// behaviour test for 0x005791B5 _STL::list<long>::pop_back: erase the node before the sentinel
#include <cstdio>
#include <cstdlib>

namespace _STL {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Self& operator--() { this->_M_decr(); return *this; }
};

template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _Node* _M_node;
    list() { _M_node = (_Node*)std::malloc(sizeof(_Node)); _M_node->_M_next = _M_node; _M_node->_M_prev = _M_node; }
    ~list() { while (_M_node->_M_next != _M_node) pop_back(); std::free(_M_node); }
    iterator begin() { return (_Node*)_M_node->_M_next; }
    iterator end() { return _M_node; }
    void push_back(const _Tp& __x)
    {
        _Node* __n = (_Node*)std::malloc(sizeof(_Node));
        __n->_M_data = __x;
        __n->_M_next = _M_node;
        __n->_M_prev = _M_node->_M_prev;
        _M_node->_M_prev->_M_next = __n;
        _M_node->_M_prev = __n;
    }
    iterator erase(iterator __position)                  // stand-in for 0x00429B02
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        std::free(__position._M_node);
        return iterator((_Node*)__next_node);
    }
    void pop_back()
    {
        iterator __tmp = end();
        erase(--__tmp);
    }
    unsigned int size() const { unsigned int n = 0; for (_List_node_base* p = _M_node->_M_next; p != _M_node; p = p->_M_next) ++n; return n; }
    _Tp back() const { return ((_Node*)_M_node->_M_prev)->_M_data; }
    _Tp front() const { return ((_Node*)_M_node->_M_next)->_M_data; }
};

}

int main()
{
    _STL::list<long> generals;
    generals.push_back(10);
    generals.push_back(20);
    generals.push_back(30);
    generals.pop_back();
    if (generals.size() != 2 || generals.back() != 20 || generals.front() != 10) { std::printf("005791b5_TEST FAIL\n"); return 1; }
    generals.pop_back();
    generals.pop_back();
    if (generals.size() != 0) { std::printf("005791b5_TEST FAIL\n"); return 1; }
    generals.push_back(5);
    if (generals.size() != 1 || generals.back() != 5) { std::printf("005791b5_TEST FAIL\n"); return 1; }
    std::printf("005791b5_TEST PASS\n");
    return 0;
}