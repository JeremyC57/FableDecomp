// Behaviour test for 0x00515420 CLineList::AddFront(line): the copy goes to the front (before the old first node),
// the links stay consistent, and the list itself is returned for chaining.
#include <cstdio>
#include <cstdlib>

class C2DVector { public: float X, Y; };
class C2DLineF { public: C2DVector Point1, Point2; };
struct NodeBase { NodeBase* _M_next; NodeBase* _M_prev; };
struct LineNode : NodeBase { C2DLineF _M_data; };

struct line_list_model {
    NodeBase* _M_node;
    line_list_model() { _M_node = (NodeBase*)std::malloc(sizeof(LineNode)); _M_node->_M_next = _M_node->_M_prev = _M_node; }
    line_list_model& AddFront(const C2DLineF& x)
    {
        NodeBase* n = _M_node->_M_next;
        LineNode* tmp = (LineNode*)std::malloc(sizeof(LineNode));
        tmp->_M_data = x;
        NodeBase* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return *this;
    }
};

static C2DLineF line(float v) { C2DLineF l; l.Point1.X = v; l.Point1.Y = v + 1; l.Point2.X = v + 2; l.Point2.Y = v + 3; return l; }

int main()
{
    line_list_model l;
    if (&l.AddFront(line(1.0f)).AddFront(line(5.0f)) != &l) { std::printf("00515420_TEST FAIL\n"); return 1; }
    NodeBase* a = l._M_node->_M_next;
    NodeBase* b = a->_M_next;
    if (((LineNode*)a)->_M_data.Point1.X != 5.0f || ((LineNode*)b)->_M_data.Point2.Y != 4.0f) { std::printf("00515420_TEST FAIL\n"); return 1; }
    if (b->_M_next != l._M_node || l._M_node->_M_prev != b || a->_M_prev != l._M_node || b->_M_prev != a) { std::printf("00515420_TEST FAIL\n"); return 1; }
    std::printf("00515420_TEST PASS\n");
    return 0;
}