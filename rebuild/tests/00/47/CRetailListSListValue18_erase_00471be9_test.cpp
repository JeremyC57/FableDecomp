#include <cstdio>
#include <cstdlib>

static int g_frees = 0;
static void FreeNode(void* p) { ++g_frees; std::free(p); }

struct SListValue18 { long m_words[6]; };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
struct _List_node : _List_node_base { SListValue18 _M_data; };
struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* n) : _M_node(n) { }
    _List_iterator(const _List_iterator& o) : _M_node(o._M_node) { }
    bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
    _List_iterator operator++(int) { _List_iterator tmp = *this; _M_node = _M_node->_M_next; return tmp; }
};
struct CRetailList_SListValue18 {
    _List_node_base* _M_node;
    typedef _List_iterator iterator;
    iterator erase(iterator position)
    {
        _List_node_base* next = position._M_node->_M_next;
        _List_node_base* prev = position._M_node->_M_prev;
        _List_node* n = (_List_node*)position._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        FreeNode(n);
        return iterator(next);
    }
    iterator erase(iterator first, iterator last)
    {
        while (first != last)
            erase(first++);
        return last;
    }
};

int main()
{
    _List_node_base head;
    head._M_next = head._M_prev = &head;
    _List_node* nodes[5];
    for (int i = 0; i < 5; ++i) {                       // push_back five nodes
        _List_node* n = (_List_node*)std::malloc(sizeof(_List_node));
        n->_M_data.m_words[0] = i;
        n->_M_prev = head._M_prev; n->_M_next = &head;
        head._M_prev->_M_next = n; head._M_prev = n;
        nodes[i] = n;
    }
    CRetailList_SListValue18 list;
    list._M_node = &head;
    _List_iterator r = list.erase(_List_iterator(nodes[1]), _List_iterator(nodes[4]));   // erase [1, 4)
    bool ok = r._M_node == nodes[4] && g_frees == 3 && head._M_next == nodes[0] && nodes[0]->_M_next == nodes[4]
              && nodes[4]->_M_prev == nodes[0] && nodes[4]->_M_next == &head && head._M_prev == nodes[4];
    r = list.erase(_List_iterator(nodes[4]), _List_iterator(nodes[4]));                    // empty range
    if (r._M_node != nodes[4] || g_frees != 3) ok = false;
    r = list.erase(_List_iterator(head._M_next), _List_iterator(&head));                  // erase the rest
    if (r._M_node != &head || g_frees != 5 || head._M_next != &head || head._M_prev != &head) ok = false;
    if (!ok) { std::printf("00471be9_TEST FAIL\n"); return 1; }
    std::printf("00471be9_TEST PASS\n");
    return 0;
}