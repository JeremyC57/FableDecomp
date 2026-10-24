#include <cstdio>
// Behaviour test: the range insert puts copies of [first, last) before position, in order.
struct _List_node { _List_node* _M_next; _List_node* _M_prev; int _M_data; };
struct _List_iterator {
    _List_node* _M_node;
    _List_iterator(_List_node* n) : _M_node(n) { }
    _List_iterator(const _List_iterator& o) : _M_node(o._M_node) { }
    const int& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
};
static _List_node g_pool[16];
static int g_used = 0;
struct list {
    _List_node* _M_node;
    _List_iterator insert(_List_iterator position, const int& x)
    {
        _List_node* n = &g_pool[g_used++];
        n->_M_data = x;
        n->_M_next = position._M_node; n->_M_prev = position._M_node->_M_prev;
        position._M_node->_M_prev->_M_next = n; position._M_node->_M_prev = n;
        return _List_iterator(n);
    }
    void insert(_List_iterator position, _List_iterator first, _List_iterator last)
    {
        for (; first != last; ++first)
            insert(position, *first);
    }
};
int main()
{
    _List_node srcHead, a, b, c;
    srcHead._M_next = &a; a._M_next = &b; b._M_next = &c; c._M_next = &srcHead;
    a._M_data = 1; b._M_data = 2; c._M_data = 3;
    _List_node head; head._M_next = &head; head._M_prev = &head; head._M_data = -1;
    list dst; dst._M_node = &head;
    dst.insert(_List_iterator(&head), _List_iterator(&a), _List_iterator(&srcHead));
    int expect[3] = { 1, 2, 3 };
    _List_node* n = head._M_next;
    int i = 0;
    for (i = 0; i < 3; ++i) { if (n == &head || n->_M_data != expect[i]) { std::printf("0062f650_TEST FAIL\n"); return 1; } n = n->_M_next; }
    if (n != &head || head._M_prev->_M_data != 3) { std::printf("0062f650_TEST FAIL\n"); return 1; }
    dst.insert(_List_iterator(head._M_next), _List_iterator(&b), _List_iterator(&b));   // empty range
    if (g_used != 3) { std::printf("0062f650_TEST FAIL\n"); return 1; }
    std::printf("0062f650_TEST PASS\n");
    return 0;
}