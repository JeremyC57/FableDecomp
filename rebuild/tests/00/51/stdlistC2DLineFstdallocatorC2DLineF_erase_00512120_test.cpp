// Behaviour test for 0x00512120 std::list<C2DLineF>::erase(first, last): the nodes of [first, last) are unlinked and
// freed, the neighbours are joined, `last` is returned; an empty range changes nothing.
#include <cstdio>
#include <cstdlib>

struct NodeBase { NodeBase* _M_next; NodeBase* _M_prev; };
struct LineNode : NodeBase { float v[4]; };

static int g_freed = 0;

static NodeBase* erase_range_model(NodeBase* first, NodeBase* last)
{
    while (first != last) {
        NodeBase* n = first;
        first = first->_M_next;
        NodeBase* next_node = n->_M_next;
        NodeBase* prev_node = n->_M_prev;
        prev_node->_M_next = next_node;
        next_node->_M_prev = prev_node;
        std::free(n);
        ++g_freed;
    }
    return last;
}

int main()
{
    NodeBase* s = (NodeBase*)std::malloc(sizeof(LineNode));
    s->_M_next = s->_M_prev = s;
    NodeBase* nodes[5];
    for (int i = 0; i < 5; ++i) {
        LineNode* n = (LineNode*)std::malloc(sizeof(LineNode));
        n->v[0] = (float)i;
        n->_M_next = s; n->_M_prev = s->_M_prev; s->_M_prev->_M_next = n; s->_M_prev = n;
        nodes[i] = n;
    }
    if (erase_range_model(nodes[1], nodes[4]) != nodes[4] || g_freed != 3) { std::printf("00512120_TEST FAIL\n"); return 1; }
    if (nodes[0]->_M_next != nodes[4] || nodes[4]->_M_prev != nodes[0] || s->_M_next != nodes[0] || s->_M_prev != nodes[4]) { std::printf("00512120_TEST FAIL\n"); return 1; }
    if (erase_range_model(nodes[4], nodes[4]) != nodes[4] || g_freed != 3) { std::printf("00512120_TEST FAIL\n"); return 1; }
    if (erase_range_model(s->_M_next, s) != s || g_freed != 5 || s->_M_next != s || s->_M_prev != s) { std::printf("00512120_TEST FAIL\n"); return 1; }
    std::printf("00512120_TEST PASS\n");
    return 0;
}