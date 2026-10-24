// Behaviour test for 0x005155F0 CNavigationAreaUpdate::CSwitchableBlockage(): the line list owns a freshly allocated
// sentinel node linked to itself (an empty STLport list); UniqueID is not touched.
#include <cstdio>
#include <cstdlib>

struct NodeBase { NodeBase* _M_next; NodeBase* _M_prev; };
struct LineNode : NodeBase { float Point1X, Point1Y, Point2X, Point2Y; };

struct line_list_model {
    LineNode* _M_node;
    line_list_model() : _M_node(0)
    {
        LineNode* n = (LineNode*)std::malloc(sizeof(LineNode));
        n->_M_next = n;
        n->_M_prev = n;
        _M_node = n;
    }
};

struct blockage_model {
    unsigned long   UniqueIDLow, UniqueIDHigh;
    line_list_model LineList;
    blockage_model() { }
};

int main()
{
    blockage_model* b = new blockage_model;
    LineNode* s = b->LineList._M_node;
    if (s == 0 || s->_M_next != s || s->_M_prev != s) { std::printf("005155f0_TEST FAIL\n"); return 1; }
    blockage_model c;
    if (c.LineList._M_node == s || c.LineList._M_node->_M_next != c.LineList._M_node) { std::printf("005155f0_TEST FAIL\n"); return 1; }
    std::free(s); std::free(c.LineList._M_node); delete b;
    std::printf("005155f0_TEST PASS\n");
    return 0;
}