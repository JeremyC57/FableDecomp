// Behaviour test for 0x005152C0 std::list<C2DLineF>::_M_insert_dispatch(position, first, last, __false_type): every
// 16-byte line of [first, last) is copied, in order, into new nodes linked before position.
#include <cstdio>
#include <cstdlib>

struct C2DVector { float X, Y; };
struct C2DLineF { C2DVector Point1, Point2; };
struct NodeBase { NodeBase* _M_next; NodeBase* _M_prev; };
struct LineNode : NodeBase { C2DLineF _M_data; };

static void insert_before(NodeBase* n, const C2DLineF& x)
{
    LineNode* tmp = (LineNode*)std::malloc(sizeof(LineNode));
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
        insert_before(position, ((LineNode*)first)->_M_data);
}

static NodeBase* make_list() { NodeBase* s = (NodeBase*)std::malloc(sizeof(LineNode)); s->_M_next = s->_M_prev = s; return s; }

int main()
{
    NodeBase* src = make_list();
    for (int i = 0; i < 3; ++i) {
        C2DLineF l; l.Point1.X = (float)i; l.Point1.Y = (float)(i + 10); l.Point2.X = (float)(i + 20); l.Point2.Y = (float)(i + 30);
        insert_before(src, l);
    }
    NodeBase* dst = make_list();
    insert_dispatch_model(dst, src->_M_next, src);
    NodeBase* n = dst->_M_next;
    for (int i = 0; i < 3; ++i, n = n->_M_next) {
        const C2DLineF& l = ((LineNode*)n)->_M_data;
        if (n == dst || l.Point1.X != (float)i || l.Point1.Y != (float)(i + 10) || l.Point2.X != (float)(i + 20) || l.Point2.Y != (float)(i + 30)) { std::printf("005152c0_TEST FAIL\n"); return 1; }
    }
    if (n != dst || dst->_M_prev->_M_next != dst) { std::printf("005152c0_TEST FAIL\n"); return 1; }
    std::printf("005152c0_TEST PASS\n");
    return 0;
}