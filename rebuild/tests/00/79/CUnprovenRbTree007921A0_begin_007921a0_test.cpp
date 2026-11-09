// Behaviour test for 0x007921A0 CUnprovenRbTree_007921A0::begin(): the iterator lands on the header node's leftmost link.
#include <stdio.h>
struct CUnprovenRbTreeNode_007921A0 {                  // STLport _Rb_tree_node_base layout
    bool _M_color;
    CUnprovenRbTreeNode_007921A0* _M_parent;
    CUnprovenRbTreeNode_007921A0* _M_left;
    CUnprovenRbTreeNode_007921A0* _M_right;
};
class CUnprovenRbTree_007921A0 {
public:
    struct iterator {
        CUnprovenRbTreeNode_007921A0* _M_node;
        iterator(CUnprovenRbTreeNode_007921A0* node) : _M_node(node) { }
    };
    CUnprovenRbTreeNode_007921A0* _M_header;           // _M_header._M_data: the header node
    iterator begin();
};
CUnprovenRbTree_007921A0::iterator CUnprovenRbTree_007921A0::begin()
{
    return iterator(_M_header->_M_left);
}
int main()
{
    CUnprovenRbTreeNode_007921A0 header, a, b;
    header._M_color = false; header._M_parent = &b; header._M_left = &a; header._M_right = &b;
    CUnprovenRbTree_007921A0 tree; tree._M_header = &header;
    CUnprovenRbTree_007921A0::iterator it = tree.begin();
    int ok = it._M_node == &a;
    header._M_left = &header;                         // empty tree: begin() == the header (end())
    ok = ok && tree.begin()._M_node == &header;
    puts(ok ? "007921a0_TEST PASS" : "007921a0_TEST FAIL");
    return ok ? 0 : 1;
}