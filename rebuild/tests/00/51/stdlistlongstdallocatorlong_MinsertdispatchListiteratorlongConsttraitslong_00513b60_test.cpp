// Behaviour test for 0x00513B60 std::list<long>::_M_insert_dispatch(position, first, last, __false_type): every element
// of [first, last) is copied, in order, into new nodes linked before position; an empty range inserts nothing.
#include <cstdio>
#include <cstdlib>

struct NodeBase { NodeBase* _M_next; NodeBase* _M_prev; };
struct LongNode : NodeBase { long _M_data; };

static void insert_before(NodeBase* n, long x)
{
    LongNode* tmp = (LongNode*)std::malloc(sizeof(LongNode));
    tmp->_M_data = x;
    NodeBase* p = n->_M_prev;
    tmp->_M_next = n;
    tmp->_M_prev = p;
    p->_M_next = tmp;
    n->_M_prev = tmp;
}

static void insert_dispatch_model(NodeBase* position, NodeBase* first, NodeBase* last)
{
    for (; first != last; first = first->_M_next)
        insert_before(position, ((LongNode*)first)->_M_data);
}

static NodeBase* make_list() { NodeBase* s = (NodeBase*)std::malloc(sizeof(LongNode)); s->_M_next = s->_M_prev = s; return s; }

int main()
{
    NodeBase* src = make_list();
    insert_before(src, 10); insert_before(src, 20); insert_before(src, 30);
    NodeBase* dst = make_list();
    insert_before(dst, 1); insert_before(dst, 2);
    // insert the whole source before the second destination element
    insert_dispatch_model(dst->_M_next->_M_next, src->_M_next, src);
    static const long want[5] = { 1, 10, 20, 30, 2 };
    NodeBase* n = dst->_M_next;
    for (int i = 0; i < 5; ++i, n = n->_M_next)
        if (n == dst || ((LongNode*)n)->_M_data != want[i] || n->_M_next->_M_prev != n) { std::printf("00513b60_TEST FAIL\n"); return 1; }
    if (n != dst || dst->_M_prev->_M_next != dst) { std::printf("00513b60_TEST FAIL\n"); return 1; }
    insert_dispatch_model(dst, src, src);   // empty range
    if (dst->_M_prev != n->_M_prev || ((LongNode*)dst->_M_prev)->_M_data != 2) { std::printf("00513b60_TEST FAIL\n"); return 1; }
    std::printf("00513b60_TEST PASS\n");
    return 0;
}