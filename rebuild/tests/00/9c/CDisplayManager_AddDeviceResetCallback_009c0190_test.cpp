#include <cstdio>
#include <cstdlib>
#include <new>

class CDeviceResetCallback { public: int Id; };

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};
template <class _Tp> class list {
public:
    typedef _List_node<_Tp>     _Node;
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    iterator end() { return iterator(_M_node); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)std::malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}

class CDisplayManager {
public:
    void AddDeviceResetCallback(CDeviceResetCallback* callback) { DeviceResetCallbacks.push_back(callback); }
    unsigned char _pad000[0x264];
    std::list<CDeviceResetCallback*> DeviceResetCallbacks;
};

int main()
{
    CDisplayManager dm;
    std::_List_node_base sentinel;
    sentinel._M_next = &sentinel;
    sentinel._M_prev = &sentinel;
    dm.DeviceResetCallbacks._M_node = &sentinel;
    CDeviceResetCallback a, b, c;
    a.Id = 1; b.Id = 2; c.Id = 3;
    dm.AddDeviceResetCallback(&a);
    dm.AddDeviceResetCallback(&b);
    dm.AddDeviceResetCallback(&c);
    CDeviceResetCallback* expect[3] = { &a, &b, &c };
    std::_List_node_base* n = sentinel._M_next;
    for (int i = 0; i < 3; ++i) {
        if (n == &sentinel || ((std::_List_node<CDeviceResetCallback*>*)n)->_M_data != expect[i] || n->_M_next->_M_prev != n) {
            std::printf("009c0190_TEST FAIL\n"); return 1;
        }
        n = n->_M_next;
    }
    if (n != &sentinel || sentinel._M_prev->_M_next != &sentinel ||
        ((std::_List_node<CDeviceResetCallback*>*)sentinel._M_prev)->_M_data != &c) {
        std::printf("009c0190_TEST FAIL\n"); return 1;
    }
    while (sentinel._M_next != &sentinel) { n = sentinel._M_next; sentinel._M_next = n->_M_next; std::free(n); }
    std::printf("009c0190_TEST PASS\n");
    return 0;
}