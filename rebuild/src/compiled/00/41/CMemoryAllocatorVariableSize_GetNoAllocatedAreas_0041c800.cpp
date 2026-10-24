// _Rb_tree_node_base::_S_maximum(_Rb_tree_node_base* __x) -- STLport red-black tree: rightmost node of a subtree
// Retail 0x0041C800, 26 bytes (`ret`, static __cdecl, one argument). Called by the landed
// _Rb_global<bool>::_Rebalance_for_erase (0x0042A446) and three other tree members (0x0042CECB,
// 0x0053CBF6, a jump stub at 0x0042936B). The catalog label "CMemoryAllocatorVariableSize::
// GetNoAllocatedAreas" is propagated; the body is STLport 4.x verbatim:
//   while (__x->_M_right != 0) __x = __x->_M_right; return __x;
// (node: colour byte +0, parent +4, left +8, right +0xC). The O2 loop head is 16-aligned
// (`jmp ; lea ecx,[ecx]`). Twin of _S_minimum 0x0041C7E0 (walks +8).
// Leaf: _S_maximum

struct _Rb_tree_node_base {
    bool                _M_color;               // +0x00
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
    static _Rb_tree_node_base* __cdecl _S_minimum(_Rb_tree_node_base* __x);   // 0x0041C7E0
    static _Rb_tree_node_base* __cdecl _S_maximum(_Rb_tree_node_base* __x);   // 0x0041C800
};

_Rb_tree_node_base* __cdecl _Rb_tree_node_base::_S_maximum(_Rb_tree_node_base* __x)
{
    while (__x->_M_right != 0)
        __x = __x->_M_right;
    return __x;
}
