// Behaviour test for the STLport list iterator prefix operator-- (0x009f7780): one step along _M_prev, returns *this.
#include <cstdio>

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
};

struct _List_iterator : _List_iterator_base {
    _List_iterator& operator--() { this->_M_decr(); return *this; }
};

int main()
{
    _List_node_base head, a, b;
    head._M_next = &a; a._M_next = &b; b._M_next = &head;
    head._M_prev = &b; b._M_prev = &a; a._M_prev = &head;
    _List_iterator it;
    it._M_node = &a;
    _List_iterator& r = -- it;
    _List_node_base* want1 = a._M_prev;
    if (&r != &it || it._M_node != want1) { std::printf("009f7780_TEST FAIL\n"); return 1; }
    -- -- it;
    if (it._M_node != &a) { std::printf("009f7780_TEST FAIL\n"); return 1; }
    std::printf("009f7780_TEST PASS\n");
    return 0;
}