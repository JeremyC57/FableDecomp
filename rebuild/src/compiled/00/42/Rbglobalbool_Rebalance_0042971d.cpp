// CMemoryAllocatorVariableSize::BalanceTree(_Rb_tree_node_base* x, _Rb_tree_node_base*& root) -- STLport red-black tree insert fix-up (_Rb_global<bool>::_Rebalance): recolour or rotate up from the new red node, then blacken the root
// Leaf: _Rebalance
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042971D, length 190 bytes (`ret`, static __cdecl).
// Callees: _Rb_global<bool>::_Rotate_left @ 0x0042951B and _Rotate_right @ 0x0042955B (both __cdecl (node, root&); the manifest labels
// both "CMemoryAllocatorVariableSize::RotateLeft").
// Label correction: the catalog name is propagated; the body is STLport's `_Rb_global<bool>::_Rebalance` (node: colour byte +0 with
// red = false, parent +4, left +8, right +0xC).
// Shape: STLport source verbatim; size TU (`#pragma optimize("s", on)`: `pop ecx` argument cleanup, one zero register for red/compare);
// the recolour block of the left case is tail-shared with the right case.
#pragma optimize("s", on)
struct _Rb_tree_node_base {
    bool                _M_color;               // +0x00
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};
static const bool _S_rb_tree_red = false;
static const bool _S_rb_tree_black = true;
template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);    // 0x0042951B
    static void __cdecl _Rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042955B
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);
};

template <class _Dummy> void __cdecl
_Rb_global<_Dummy>::_Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
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
template struct _Rb_global<bool>;