// Behaviour test for _Rb_tree_node_base::_S_maximum (0x0041C800): returns the rightmost node of
// the subtree rooted at the argument, following only right links.
#include <stdio.h>

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
    static _Rb_tree_node_base* __cdecl _S_maximum(_Rb_tree_node_base* __x);
};

_Rb_tree_node_base* __cdecl _Rb_tree_node_base::_S_maximum(_Rb_tree_node_base* __x)
{
    while (__x->_M_right != 0)
        __x = __x->_M_right;
    return __x;
}

int main()
{
    _Rb_tree_node_base n[5];
    int i;
    for (i = 0; i < 5; ++i) { n[i]._M_color = false; n[i]._M_parent = 0; n[i]._M_left = 0; n[i]._M_right = 0; }
    // n0 -> right n1 -> right n2 ; n2 has a left child n3 (ignored); n4 alone
    n[0]._M_right = &n[1]; n[1]._M_right = &n[2]; n[2]._M_left = &n[3]; n[0]._M_left = &n[4];
    if (_Rb_tree_node_base::_S_maximum(&n[0]) != &n[2]) { printf("FAIL chain\n"); return 1; }
    if (_Rb_tree_node_base::_S_maximum(&n[4]) != &n[4]) { printf("FAIL leaf\n"); return 1; }
    if (_Rb_tree_node_base::_S_maximum(&n[3]) != &n[3]) { printf("FAIL left-only\n"); return 1; }
    printf("PASS_0041c800\n");
    return 0;
}
