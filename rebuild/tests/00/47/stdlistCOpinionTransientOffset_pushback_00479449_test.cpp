// Behaviour test for 0x00479449 std::list<COpinionTransientOffset>::push_back (model copy).
#include <cstdio>
#include <cstdlib>
#include <cstring>

class COpinionTransientOffset { public: unsigned char _data[0x18]; };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct _Node : public _List_node_base { COpinionTransientOffset _M_data; };
struct iterator {
    _List_node_base* _M_node;
    iterator(_List_node_base* x) : _M_node(x) { }
    iterator(const iterator& x) : _M_node(x._M_node) { }
};
struct list {
    _List_node_base* _M_node;
    _Node* _M_create_node(const COpinionTransientOffset& x)
    {
        _Node* n = (_Node*)std::malloc(sizeof(_Node));
        std::memcpy(&n->_M_data, &x, sizeof(x));
        return n;
    }
    iterator end() { return iterator(_M_node); }
    iterator insert(iterator position, const COpinionTransientOffset& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_back(const COpinionTransientOffset& x) { insert(end(), x); }
};

int main()
{
    _List_node_base head;
    head._M_next = &head;
    head._M_prev = &head;
    list l;
    l._M_node = &head;
    COpinionTransientOffset a, b;
    std::memset(&a, 0x11, sizeof(a));
    std::memset(&b, 0x22, sizeof(b));
    l.push_back(a);
    l.push_back(b);
    _Node* n1 = (_Node*)head._M_next;
    _Node* n2 = (_Node*)n1->_M_next;
    int ok = n2->_M_next == &head && head._M_prev == n2 && n2->_M_prev == n1 && n1->_M_prev == &head
          && n1->_M_data._data[0] == 0x11 && n2->_M_data._data[23] == 0x22;
    std::free(n1);
    std::free(n2);
    if (!ok) { std::printf("00479449_TEST FAIL\n"); return 1; }
    std::printf("00479449_TEST PASS\n");
    return 0;
}