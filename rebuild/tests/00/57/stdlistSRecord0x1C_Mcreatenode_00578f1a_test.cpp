// behaviour test for 00578f1a: list::_M_create_node mallocs a 0x24-byte node and copies the 28-byte POD
// record into its data slot (after the two link words), leaving the source unchanged.
#include <cstdio>
#include <cstdlib>
#include <new>

struct SRecord0x1C {
    long m_words[7];
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

struct _List_node : _List_node_base {
    SRecord0x1C _M_data;
};

struct list {
    _List_node_base* _M_node;
    _List_node* _M_create_node(const SRecord0x1C& x)
    {
        _List_node* p = (_List_node*)std::malloc(sizeof(_List_node));
        new (&p->_M_data) SRecord0x1C(x);
        return p;
    }
};

int main()
{
    SRecord0x1C record;
    list l;
    _List_node* node = 0;
    long i = 0;
    int ok = 1;
    for (i = 0; i < 7; ++i)
        record.m_words[i] = 100 + i * 11;
    l._M_node = 0;
    node = l._M_create_node(record);
    if (node == 0)
        ok = 0;
    for (i = 0; ok && i < 7; ++i) {
        if (node->_M_data.m_words[i] != 100 + i * 11 || record.m_words[i] != 100 + i * 11)
            ok = 0;
    }
    std::free(node);
    if (!ok) {
        std::printf("00578f1a_TEST FAIL\n");
        return 1;
    }
    std::printf("00578f1a_TEST PASS\n");
    return 0;
}