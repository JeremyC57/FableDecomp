#include <cstdio>
namespace std {
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
template <class _Dummy> class _Rb_global {
public:
    static void __cdecl _Rotate_left(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);
};
template <class _Dummy>
void __cdecl _Rb_global<_Dummy>::_Rotate_left(_Rb_tree_node_base* x, _Rb_tree_node_base*& root)
{
    _Rb_tree_node_base* y = x->_M_right;
    x->_M_right = y->_M_left;
    if (y->_M_left != 0)
        y->_M_left->_M_parent = x;
    y->_M_parent = x->_M_parent;

    if (x == root)
        root = y;
    else if (x == x->_M_parent->_M_left)
        x->_M_parent->_M_left = y;
    else
        x->_M_parent->_M_right = y;
    y->_M_left = x;
    x->_M_parent = y;
}
template class _Rb_global<bool>;
}

typedef std::_Rb_tree_node_base N;
int main()
{
    // x is root: x(right=y), y(left=b)
    static N x, y, b, p, x2, y2;
    x._M_right = &y; y._M_left = &b; y._M_parent = &x; b._M_parent = &y; x._M_parent = 0;
    N* root = &x;
    std::_Rb_global<bool>::_Rotate_left(&x, root);
    bool ok = root == &y && y._M_left == &x && x._M_parent == &y && x._M_right == &b && b._M_parent == &x && y._M_parent == 0;
    // x2 is right child of p, y2 has no left
    p._M_right = &x2; p._M_left = 0; x2._M_parent = &p; x2._M_right = &y2; y2._M_parent = &x2; y2._M_left = 0;
    N* root2 = &p;
    std::_Rb_global<bool>::_Rotate_left(&x2, root2);
    ok = ok && root2 == &p && p._M_right == &y2 && y2._M_parent == &p && y2._M_left == &x2 && x2._M_right == 0;
    if (ok) std::printf("0042951b_TEST PASS\n");
    return 0;
}