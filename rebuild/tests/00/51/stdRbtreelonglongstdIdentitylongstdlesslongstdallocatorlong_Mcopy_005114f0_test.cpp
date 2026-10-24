// Behaviour test for 0x005114F0 std::_Rb_tree<long,...>::_M_copy(x, p): the clone has the same shape, values and
// colours, every parent link points at the cloned parent, the top's parent is p, and no source node is shared.
#include <cstdio>
#include <cstdlib>

struct NodeModel {
    bool       _M_color;
    NodeModel* _M_parent;
    NodeModel* _M_left;
    NodeModel* _M_right;
    long       _M_value_field;
};

static NodeModel* clone_node(NodeModel* x)
{
    NodeModel* tmp = (NodeModel*)std::malloc(sizeof(NodeModel));
    tmp->_M_value_field = x->_M_value_field;
    tmp->_M_color = x->_M_color;
    tmp->_M_left = 0;
    tmp->_M_right = 0;
    return tmp;
}

static NodeModel* copy_model(NodeModel* x, NodeModel* p)
{
    NodeModel* top = clone_node(x);
    top->_M_parent = p;
    if (x->_M_right)
        top->_M_right = copy_model(x->_M_right, top);
    p = top;
    x = x->_M_left;
    while (x != 0) {
        NodeModel* y = clone_node(x);
        p->_M_left = y;
        y->_M_parent = p;
        if (x->_M_right)
            y->_M_right = copy_model(x->_M_right, y);
        p = y;
        x = x->_M_left;
    }
    return top;
}

static NodeModel* mk(long v, bool c, NodeModel* l, NodeModel* r)
{
    NodeModel* n = (NodeModel*)std::malloc(sizeof(NodeModel));
    n->_M_value_field = v; n->_M_color = c; n->_M_left = l; n->_M_right = r; n->_M_parent = 0;
    if (l) l->_M_parent = n;
    if (r) r->_M_parent = n;
    return n;
}

static bool same(NodeModel* a, NodeModel* b, NodeModel* bparent)
{
    if (a == 0 || b == 0) return a == b;
    if (a == b || a->_M_value_field != b->_M_value_field || a->_M_color != b->_M_color || b->_M_parent != bparent) return false;
    return same(a->_M_left, b->_M_left, b) && same(a->_M_right, b->_M_right, b);
}

int main()
{
    NodeModel* src = mk(50, false, mk(30, true, mk(10, false, mk(5, true, 0, 0), mk(15, true, 0, 0)), mk(40, false, 0, 0)),
                         mk(70, true, mk(60, false, 0, 0), mk(80, false, 0, mk(90, true, 0, 0))));
    NodeModel header;
    NodeModel* top = copy_model(src, &header);
    if (top->_M_parent != &header || !same(src, top, &header)) { std::printf("005114f0_TEST FAIL\n"); return 1; }
    NodeModel* leaf = copy_model(mk(7, true, 0, 0), &header);
    if (leaf->_M_left != 0 || leaf->_M_right != 0 || leaf->_M_value_field != 7 || leaf->_M_color != true) { std::printf("005114f0_TEST FAIL\n"); return 1; }
    std::printf("005114f0_TEST PASS\n");
    return 0;
}