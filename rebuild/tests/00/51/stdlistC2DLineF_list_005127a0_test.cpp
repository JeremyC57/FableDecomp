// Behaviour test for std::list<C2DLineF>::~list @ 0x005127A0 (copy of the model; the node free is counted).
#include <cstdio>
#include <cstdlib>
class C2DLineF;
namespace std {
template <class _Tp> class allocator;
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
static int g_freed = 0;
static _List_node_base* g_sentinel = 0;
static bool g_sentinel_reset = false;
static void free(_List_node_base* p)
{
    if (p == g_sentinel) g_sentinel_reset = (p->_M_next == p && p->_M_prev == p);
    ++g_freed;
    ::free(p);
}
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    _List_node_base* _M_node;
    ~list();
};
template <class _Tp, class _Alloc> list<_Tp, _Alloc>::~list()
{
    _List_node_base* cur = _M_node->_M_next;
    while (cur != _M_node) { _List_node_base* tmp = cur; cur = cur->_M_next; free(tmp); }
    _M_node->_M_next = _M_node;
    _M_node->_M_prev = _M_node;
    if (_M_node != 0) free(_M_node);
}
}
int main()
{
    // three element nodes (0x18 bytes: links + a 16-byte C2DLineF) on a circular sentinel
    std::_List_node_base* s = (std::_List_node_base*)std::malloc(0x18);
    std::_List_node_base* prev = s;
    for (int i = 0; i < 3; ++i) {
        std::_List_node_base* n = (std::_List_node_base*)std::malloc(0x18);
        prev->_M_next = n; n->_M_prev = prev; prev = n;
    }
    prev->_M_next = s; s->_M_prev = prev;
    std::g_sentinel = s;
    {
        std::list<C2DLineF>* l = (std::list<C2DLineF>*)std::malloc(sizeof(std::list<C2DLineF>));
        l->_M_node = s;
        l->~list();
        std::free((void*)l);
    }
    if (std::g_freed != 4 || !std::g_sentinel_reset || sizeof(std::list<C2DLineF>) != 4) { std::printf("005127a0_TEST FAIL %d\n", std::g_freed); return 1; }
    std::printf("005127a0_TEST PASS\n");
    return 0;
}
