#include <cstdio>
struct _Rb_tree_node_base { bool _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right; int key; };
static const bool _S_rb_tree_red = false;
static const bool _S_rb_tree_black = true;
static int g_rot = 0;
static void _Rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
    ++g_rot;
    _Rb_tree_node_base* __y = __x->_M_right;
    __x->_M_right = __y->_M_left;
    if (__y->_M_left != 0) __y->_M_left->_M_parent = __x;
    __y->_M_parent = __x->_M_parent;
    if (__x == __root) __root = __y;
    else if (__x == __x->_M_parent->_M_left) __x->_M_parent->_M_left = __y;
    else __x->_M_parent->_M_right = __y;
    __y->_M_left = __x; __x->_M_parent = __y;
}
static void _Rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
    ++g_rot;
    _Rb_tree_node_base* __y = __x->_M_left;
    __x->_M_left = __y->_M_right;
    if (__y->_M_right != 0) __y->_M_right->_M_parent = __x;
    __y->_M_parent = __x->_M_parent;
    if (__x == __root) __root = __y;
    else if (__x == __x->_M_parent->_M_right) __x->_M_parent->_M_right = __y;
    else __x->_M_parent->_M_left = __y;
    __y->_M_right = __x; __x->_M_parent = __y;
}
static void _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
    __x->_M_color = _S_rb_tree_red;
    while (__x != __root && __x->_M_parent->_M_color == _S_rb_tree_red) {
        if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left) {
            _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_right;
            if (__y && __y->_M_color == _S_rb_tree_red) {
                __x->_M_parent->_M_color = _S_rb_tree_black;
                __y->_M_color = _S_rb_tree_black;
                __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                __x = __x->_M_parent->_M_parent;
            }
            else {
                if (__x == __x->_M_parent->_M_right) {
                    __x = __x->_M_parent;
                    _Rotate_left(__x, __root);
                }
                __x->_M_parent->_M_color = _S_rb_tree_black;
                __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                _Rotate_right(__x->_M_parent->_M_parent, __root);
            }
        }
        else {
            _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_left;
            if (__y && __y->_M_color == _S_rb_tree_red) {
                __x->_M_parent->_M_color = _S_rb_tree_black;
                __y->_M_color = _S_rb_tree_black;
                __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                __x = __x->_M_parent->_M_parent;
            }
            else {
                if (__x == __x->_M_parent->_M_left) {
                    __x = __x->_M_parent;
                    _Rotate_right(__x, __root);
                }
                __x->_M_parent->_M_color = _S_rb_tree_black;
                __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                _Rotate_left(__x->_M_parent->_M_parent, __root);
            }
        }
    }
    __root->_M_color = _S_rb_tree_black;
}
static int check(_Rb_tree_node_base* n, int lo, int hi)   // returns black height or -1
{
    if (!n) return 1;
    if (n->key < lo || n->key > hi) return -1;
    if (n->_M_color == _S_rb_tree_red && ((n->_M_left && n->_M_left->_M_color == _S_rb_tree_red) || (n->_M_right && n->_M_right->_M_color == _S_rb_tree_red))) return -1;
    int l = check(n->_M_left, lo, n->key), r = check(n->_M_right, n->key, hi);
    if (l < 0 || r < 0 || l != r) return -1;
    return l + (n->_M_color == _S_rb_tree_black ? 1 : 0);
}
int main()
{
    static _Rb_tree_node_base nodes[64];
    _Rb_tree_node_base* root = 0;
    for (int i = 0; i < 64; ++i) {
        _Rb_tree_node_base* x = &nodes[i];
        x->key = (i * 37) % 64; x->_M_left = x->_M_right = 0; x->_M_color = _S_rb_tree_black;
        if (!root) { x->_M_parent = 0; root = x; }
        else {
            _Rb_tree_node_base* p = root;
            for (;;) {
                _Rb_tree_node_base** c = x->key < p->key ? &p->_M_left : &p->_M_right;
                if (!*c) { *c = x; x->_M_parent = p; break; }
                p = *c;
            }
        }
        _Rebalance(x, root);
        if (root->_M_color != _S_rb_tree_black || check(root, -1, 1000) < 0) { std::printf("0042971d_TEST FAIL\n"); return 1; }
    }
    if (g_rot == 0 || check(root, -1, 1000) > 8) { std::printf("0042971d_TEST FAIL\n"); return 1; }
    std::printf("0042971d_TEST PASS\n");
    return 0;
}
