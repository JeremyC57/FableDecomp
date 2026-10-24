// Behaviour test for 0x004294E2 std::_Rb_global<bool>::_M_decrement: predecessor in a red-black tree (header -> rightmost, left subtree's rightmost, else first ancestor reached from a right child)
#include <cstdio>
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
static const bool _S_rb_tree_red = false;
static const bool _S_rb_tree_black = true;
_Rb_tree_node_base* _M_decrement(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_color == _S_rb_tree_red &&
        _M_node->_M_parent->_M_parent == _M_node)
        _M_node = _M_node->_M_right;
    else if (_M_node->_M_left != 0) {
        _Rb_tree_node_base* __y = _M_node->_M_left;
        while (__y->_M_right != 0)
            __y = __y->_M_right;
        _M_node = __y;
    }
    else {
        _Rb_tree_node_base* __y = _M_node->_M_parent;
        while (_M_node == __y->_M_left) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        _M_node = __y;
    }
    return _M_node;
}
static void Link(_Rb_tree_node_base& n, bool colour, _Rb_tree_node_base* parent, _Rb_tree_node_base* left, _Rb_tree_node_base* right)
{
    n._M_color = colour; n._M_parent = parent; n._M_left = left; n._M_right = right;
}
int main()
{
    // in order: a(10) b(20) c(25) d(30) e(40); d is the root
    _Rb_tree_node_base header, a, b, c, d, e;
    Link(header, _S_rb_tree_red, &d, &a, &e);
    Link(d, _S_rb_tree_black, &header, &b, &e);
    Link(b, _S_rb_tree_red, &d, &a, &c);
    Link(a, _S_rb_tree_black, &b, 0, 0);
    Link(c, _S_rb_tree_black, &b, 0, 0);
    Link(e, _S_rb_tree_black, &d, 0, 0);
    if (_M_decrement(&header) != &e || _M_decrement(&e) != &d || _M_decrement(&d) != &c || _M_decrement(&c) != &b ||
        _M_decrement(&b) != &a) { std::printf("004294e2_TEST FAIL\n"); return 1; }
    std::printf("004294e2_TEST PASS\n");
    return 0;
}